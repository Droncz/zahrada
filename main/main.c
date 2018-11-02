#include <time.h>
#include <sys/time.h>

#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_ota_ops.h"
#include "esp_event_loop.h"
#include "esp_flash_partitions.h"

#include "nvs_flash.h"
#include "http_server.h"
#include "driver/gpio.h"
#include "lwip/apps/sntp.h"

#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "config.h"
#include "httpd.h"
#include "nvs_funcs.h"

#define HASH_LEN 32 /* SHA-256 digest length */

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;

const int relays_gpios[] = RELAYS_GPIOS;
char* relays_names[] = RELAYS_NAMES;
relay_t relays[RELAYS_USED];

/* FreeRTOS event group to signal when we are connected & ready to make a request */
static EventGroupHandle_t wifi_event_group;


static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    httpd_handle_t *webserver = (httpd_handle_t *) ctx;

    switch(event->event_id) {
    case SYSTEM_EVENT_STA_START:
        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_START");
        ESP_ERROR_CHECK(esp_wifi_connect());
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        // Let the other tasks know about the connection state

        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_GOT_IP");
        ESP_LOGI(TAG, "Got IP: '%s'",
                 ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));

        /* Start the web server */
        if (*webserver == NULL) {
            *webserver = start_webserver();
        }
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);

        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        // Let the other tasks know about the connection state
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);

        ESP_LOGI(TAG, "SYSTEM_EVENT_STA_DISCONNECTED");
        ESP_ERROR_CHECK(esp_wifi_connect());

        /* Stop the web server */
        if (*webserver) {
            stop_webserver(*webserver);
            *webserver = NULL;
        }

        break;
    default:
        break;
    }
    return ESP_OK;
}


void print_sha256 (const uint8_t *image_hash, const char *label)
{
    char hash_print[HASH_LEN * 2 + 1];
    hash_print[HASH_LEN * 2] = 0;
    for (int i = 0; i < HASH_LEN; ++i) {
        sprintf(&hash_print[i * 2], "%02x", image_hash[i]);
    }
    ESP_LOGI(TAG, "%s: %s", label, hash_print);
}


void print_partition_info()
{
    uint8_t sha_256[HASH_LEN] = { 0 };
    esp_partition_t partition;
    const esp_partition_t *running = esp_ota_get_running_partition();

    ESP_LOGI(TAG, "Running partition type %d subtype %d (offset 0x%08x)",
             running->type, running->subtype, running->address);


    // get sha256 digest for the partition table
    partition.address   = ESP_PARTITION_TABLE_OFFSET;
    partition.size      = ESP_PARTITION_TABLE_MAX_LEN;
    partition.type      = ESP_PARTITION_TYPE_DATA;
    esp_partition_get_sha256(&partition, sha_256);
    print_sha256(sha_256, "SHA-256 for the partition table: ");

    // get sha256 digest for bootloader
    partition.address   = ESP_BOOTLOADER_OFFSET;
    partition.size      = ESP_PARTITION_TABLE_OFFSET;
    partition.type      = ESP_PARTITION_TYPE_APP;
    esp_partition_get_sha256(&partition, sha_256);
    print_sha256(sha_256, "SHA-256 for bootloader: ");

    // get sha256 digest for running partition
    esp_partition_get_sha256(esp_ota_get_running_partition(), sha_256);
    print_sha256(sha_256, "SHA-256 for current firmware: ");

}


void print_time() 
{
    time_t now;
    struct tm timeinfo;
    char strftime_buf[64];

    setenv("TZ", "CET-1CEST,M3.5.0/3,M10.5.0/2", 1);
    tzset();
    while (1) {
        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
        printf("The current date/time in Prague is: %s\r\n", strftime_buf);
        vTaskDelay(600 * SECOND);
    }
}


void initialise_sntp()
{
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    char strftime_buf[64];

    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Waiting for WiFi connection...\r\n");
        xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
        ESP_LOGI(TAG, "Getting time over NTP...\r\n");
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, "pool.ntp.org");
        sntp_init();

        // wait for time to be set
        while(timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
            ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
            vTaskDelay(2 * SECOND);
            time(&now);
            localtime_r(&now, &timeinfo);
        }

        // time(&now);
        // localtime_r(&now, &timeinfo);
    }


    // Set timezone to China Standard Time
    setenv("TZ", "CST-8", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);

    // Set timezone to Central European Standard Time and print local time
    setenv("TZ", "CET-1CEST,M3.5.0/3,M10.5.0/2", 1);
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in Prague is: %s", strftime_buf);

    /*
    const int deep_sleep_sec = 10;
    ESP_LOGI(TAG, "Entering deep sleep for %d seconds", deep_sleep_sec);
    esp_deep_sleep(1000000LL * deep_sleep_sec);
    */
   
    TaskHandle_t task_Handle = NULL;
    xTaskCreate( print_time, "print_time", 2048, NULL, tskIDLE_PRIORITY, &task_Handle );
    configASSERT( task_Handle );
}


void blink()
{
    int cnt = 0;

    while(1) {
        vTaskDelay(SECOND);
        cnt++;
        // printf("Blink count: %d\n", cnt);
        if (relays[0].enabled) {
            gpio_set_level(relays[0].gpio_num, cnt % 2);
            relays[0].state = cnt % 2;
        }
        if (relays[1].enabled) {
            gpio_set_level(relays[1].gpio_num, cnt % 2);
            relays[1].state = cnt % 2;
        }

        gpio_set_level(relays[6].gpio_num, cnt % 2); // onboard LED blink
        relays[6].state = cnt % 2;
    }
    
}


void initialise_relays()
{
    gpio_config_t io_conf;
    int i;

    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 1;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    for(i = 0; i < RELAYS_USED; i++) {
        relays[i].name = relays_names[i];
        relays[i].gpio_num = relays_gpios[i];
        relays[i].state = 0;
        ESP_ERROR_CHECK(nvs_read_int32(&relays[i].enabled, relays_names[i]));
    }
    
    // Just try to blink with RELAY_1_0 and RELAY_1_1 for now
    TaskHandle_t task_Handle = NULL;
    xTaskCreate( blink, "blink", 2048, NULL, tskIDLE_PRIORITY, &task_Handle );
    configASSERT( task_Handle );

}

static void initialise_nvs(void)
{
    esp_err_t err;
    int32_t restart_counter;

    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // OTA app partition table has a smaller NVS partition size than the non-OTA
        // partition table. This size mismatch may cause NVS initialization to fail.
        // If this happens, we erase NVS partition and initialize NVS again.
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    ESP_ERROR_CHECK(nvs_read_int32(&restart_counter, "restart_counter"));
    restart_counter++;
    ESP_LOGI(TAG, "Boot count: %d", restart_counter);
    ESP_ERROR_CHECK(nvs_save_int32(restart_counter, "restart_counter"));

    printf("\n");

}

static void initialise_wifi(void)
{
    static httpd_handle_t webserver = NULL;

    tcpip_adapter_init();
    
    wifi_event_group = xEventGroupCreate();
    
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, &webserver));
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };
    ESP_LOGI(TAG, "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}


void app_main()
{
    /************************************************
    1. Prints basic chip info
    3. Runs basic preparation for the OTA function
    ************************************************/ 

   printf("\r\n\r\n======================================================\r\n");

    // Print basic chip info
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    printf("silicon revision %d, ", chip_info.revision);
    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    // Initialize NVS.
    initialise_nvs();

    // Call for the Wi-Fi initialization, which in addition sets up system event handler to
    //  1) on the event of reception of the IP address:
    //      1.1) starts the webserver
    //      1.2) sets "CONNECTED_BIT" using the FreeRTOS "Event Group" to signal other tasks
    initialise_wifi();

    print_partition_info();
    
    // After the Wi-fi connects, try to receive and set the system time using the NTP protocol
    // Setup a task which will print the time every few minutes
    initialise_sntp();

    // Setup GPIOs for the requested function
    initialise_relays();
    
}
