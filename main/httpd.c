
#include <esp_wifi.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>

#include <http_server.h>

#include "config.h"
#include "httpd.h"
#include "urls.h"

void register_handlers(httpd_handle_t webserver)
{
    int i;

    int handlers_no = sizeof(handlers)/sizeof(httpd_uri_t);
    ESP_LOGI(TAG, "Registering handlers");
    ESP_LOGI(TAG, "No of handlers = %d", handlers_no);
    for (i = 0; i < handlers_no; i++) {
        if (httpd_register_uri_handler(webserver, &handlers[i]) != ESP_OK) {
            ESP_LOGW(TAG, "register uri failed for %d", i);
            return;
        }
    }
    ESP_LOGI(TAG, "Success");
}

httpd_handle_t start_webserver(void)
{
    httpd_handle_t webserver = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // Start the httpd server
    ESP_LOGI(TAG, "Starting webserver on port: '%d'", config.server_port);
    if (httpd_start(&webserver, &config) == ESP_OK) {
        register_handlers(webserver);
        return webserver;
    }

    ESP_LOGI(TAG, "Error starting server!");
    return NULL;
}

void stop_webserver(httpd_handle_t webserver)
{
    // Stop the httpd server
    httpd_stop(webserver);
}
