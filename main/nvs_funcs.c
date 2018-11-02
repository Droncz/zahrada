
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"

#include "config.h"


esp_err_t nvs_read_int32(int32_t *num, char *num_name)
{
    nvs_handle my_handle;
    esp_err_t err;

    ESP_LOGD(TAG, "Opening Non-Volatile Storage (NVS) handle... ");
    err = nvs_open(TAG, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGD(TAG, "Done\n");

        // Read
        ESP_LOGI(TAG, "Reading %s from NVS ... ", num_name);
        *num = -1; // value will default to -1, if not set yet in NVS
        err = nvs_get_i32(my_handle, num_name, num);
        switch (err) {
            case ESP_OK:
                ESP_LOGI(TAG, "Done\n");
                ESP_LOGD(TAG, "%s = %d\n", num_name, *num);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                ESP_LOGI(TAG, "The value is not initialized yet!\n");
                err = ESP_OK;
                break;
            default :
                ESP_LOGE(TAG, "Error (%s) reading!\n", esp_err_to_name(err));
        }

        // Close
        nvs_close(my_handle);
    }
    return err;
}

esp_err_t nvs_save_int32(int32_t num, char *num_name)
{
    nvs_handle my_handle;
    esp_err_t err;

    ESP_LOGD(TAG, "Opening Non-Volatile Storage (NVS) handle... ");
    err = nvs_open(TAG, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGD(TAG, "Done\n");

        // Write
        ESP_LOGI(TAG, "Updating %s in NVS ... ", num_name);
        err = nvs_set_i32(my_handle, num_name, num);
        //// ESP_LOGI(TAG, (err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Commit written value.
        // After setting any values, nvs_commit() must be called to ensure changes are written
        // to flash storage. Implementations may write to storage at other times,
        // but this is not guaranteed.
        ESP_LOGI(TAG, "Committing updates in NVS ... ");
        err = nvs_commit(my_handle);
        //// ESP_LOGI(TAG, (err != ESP_OK) ? "Failed!\n" : "Done\n");

        // Close
        nvs_close(my_handle);
    }
    return err;
}
