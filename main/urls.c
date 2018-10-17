#include <stdlib.h>
#include <stdbool.h>

#include <esp_log.h>
#include <esp_system.h>
#include <esp_ota_ops.h>

#include <http_server.h>

#include "config.h"

#define HTTPD_302      "302 Found"
#define max_resp_size 50


esp_err_t reboot_handler(httpd_req_t *req)
{
    const char *resp_str = "RESTARTING THE SYSTEM.";
    int socket = httpd_req_to_sockfd(req);

    ESP_LOGW(TAG, "Received request for restart.\n");
    httpd_resp_set_status(req, HTTPD_200);
    httpd_resp_send(req, resp_str, strlen(resp_str));
    httpd_trigger_sess_close(req->handle, socket);
    // ESP_LOGI(TAG, "Waiting 5 secs to process http request.\n");
    // vTaskDelay(5*SECOND);
    ESP_LOGW(TAG, "Restarting now.\n");
    fflush(stdout);
    esp_restart();

    return ESP_OK;
}

esp_err_t redirect_handler(httpd_req_t *req)
{
    char resp_str[50];
    const char *moved = "Moved to ";

    httpd_resp_set_status(req, HTTPD_302);
    // const char* redir_str = (const char*) req->user_ctx;
    httpd_resp_set_hdr(req, "Location", (const char*) req->user_ctx);
    strcpy(resp_str, moved);
    strcat(resp_str, req->user_ctx);
    httpd_resp_send(req, resp_str, strlen(resp_str));

    return ESP_OK;
}

esp_err_t upload_firmware_handler(httpd_req_t *req)
{
    int remaining, data_read;
    char resp[max_resp_size] = ""; // empty response means everything is OK

    static char ota_write_data[BUFFSIZE + 1] = { 0 };
    esp_err_t err;
    /* update handle : set by esp_ota_begin(), must be freed via esp_ota_end() */
    esp_ota_handle_t update_handle = 0 ;
    const esp_partition_t *update_partition = NULL;
    const esp_partition_t *configured = esp_ota_get_boot_partition();
    const esp_partition_t *running = esp_ota_get_running_partition();

    /* Preparation of the OTA update
    *   - get the partition to update
    *   - empty that partition
    */
    if (configured != running) {
        ESP_LOGW(TAG, "Configured OTA boot partition at offset 0x%08x, but running from offset 0x%08x",
                 configured->address, running->address);
        ESP_LOGW(TAG, "(This can happen if either the OTA boot data or preferred boot image become corrupted somehow.)");
    }
    ESP_LOGI(TAG, "Running partition type %d subtype %d (offset 0x%08x)",
             running->type, running->subtype, running->address);

    update_partition = esp_ota_get_next_update_partition(NULL);
    ESP_LOGI(TAG, "Writing to partition subtype %d at offset 0x%x",
             update_partition->subtype, update_partition->address);
    assert(update_partition != NULL);

    // Try to empty the partition
    err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &update_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_begin failed (%s)", esp_err_to_name(err));
        snprintf(resp, max_resp_size, "esp_ota_begin failed (%s)", esp_err_to_name(err));
    }
    ESP_LOGI(TAG, "esp_ota_begin succeeded");

    // Now get the chunks of data and write them to the flash
    remaining = req->content_len;
    ESP_LOGI(TAG, "File to be sent has %i bytes\r\n", remaining);
    while (remaining > 0) {
        // Read the chunks of data from the request and write them to flash
        data_read = httpd_req_recv(req, ota_write_data, BUFFSIZE);
        ESP_LOGD(TAG, "Received %i bytes\r\n", data_read);
        if (data_read < 0) return ESP_FAIL;

        err = esp_ota_write( update_handle, (const void *)ota_write_data, data_read);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Flash write failed (%s)", esp_err_to_name(err));
            snprintf(resp, max_resp_size, "Flash write failed (%s)", esp_err_to_name(err));
        }
        remaining -= data_read;
        if (!(remaining % 100000)) ESP_LOGI(TAG, "written %d bytes. Remaining %d", req->content_len-remaining, remaining);
        ESP_LOGD(TAG, "Data written. Bytes to write remaining: %d", remaining);
    }

    err = esp_ota_end(update_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_end failed (%s)", esp_err_to_name(err));
        snprintf(resp, max_resp_size, "esp_ota_end failed (%s)", esp_err_to_name(err));
    } else {
            ESP_LOGI(TAG, "esp_ota_end succeeded");
    }

    if (esp_partition_check_identity(esp_ota_get_running_partition(), update_partition) == true) {
        ESP_LOGI(TAG, "The current running firmware is same as the firmware just downloaded");
    }

    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_ota_set_boot_partition failed (%s)!", esp_err_to_name(err));
        snprintf(resp, max_resp_size, "esp_ota_set_boot_partition failed (%s)!", esp_err_to_name(err));
    } else {
            ESP_LOGI(TAG, "esp_ota_set_boot_partition succeeded");
    }

    // Send response with outcome
    httpd_resp_send(req, resp, strlen(resp));

    return ESP_OK;
}


esp_err_t static_html_get_handler(httpd_req_t *req)
{
    char*  buf;
    size_t buf_len;

    ESP_LOGI(TAG, "Handling URI: %s", req->uri);
    // Get header values
    // allocate memory for header string length + 1 extra byte for null termination
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        /* Copy null terminated value string into buffer */
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Host: %s", buf);
        }
        free(buf);
    }
/*     buf_len = httpd_req_get_hdr_value_len(req, "Test-Header-1") + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Test-Header-1", buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found header => Test-Header-1: %s", buf);
        }
        free(buf);
    }
 */
    // Read URL query string length and allocate memory for length + 1 extra byte for null termination
    buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query => %s", buf);
            char param[32];
            /* Get value of expected key from query string */
            if (httpd_query_key_value(buf, "query1", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => query1=%s", param);
            }
            if (httpd_query_key_value(buf, "query3", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => query3=%s", param);
            }
            if (httpd_query_key_value(buf, "query2", param, sizeof(param)) == ESP_OK) {
                ESP_LOGI(TAG, "Found URL query parameter => query2=%s", param);
            }
        }
        free(buf);
    }

    /* Set some custom headers */
    // httpd_resp_set_hdr(req, "Custom-Header-1", "Custom-Value-1");
    // httpd_resp_set_hdr(req, "Custom-Header-2", "Custom-Value-2");

    httpd_resp_set_type(req, HTTPD_TYPE_TEXT);

    /* Send response with custom headers and body set as the
     * string passed in user context*/
    const char* page_content = (const char*) req->user_ctx;
    // ESP_LOGI(TAG, "odkaz na text: %p\r\n\r\nText:\r\n%s", page_content, page_content);
    httpd_resp_send(req, page_content, strlen(page_content));
    // After sending the HTTP response the old HTTP request headers are lost

    return ESP_OK;
}

esp_err_t static_html_post_handler(httpd_req_t *req)
{
    /* Read request content */
    char content[100];

    ESP_LOGI(TAG, "Handling URI: %s", req->uri);
    /* Truncate if content length larger than the buffer */
    size_t recv_size = MIN(req->content_len, sizeof(content));

    int ret = httpd_req_recv(req, content, recv_size);
    if (ret < 0) {
        /* In case of recv error, returning ESP_FAIL will
         * ensure that the underlying socket is closed */
        return ESP_FAIL;
    }

    /* Send a simple response */
    const char resp[] = "URI POST Response";
    httpd_resp_send(req, resp, strlen(resp));
    return ESP_OK;
}
