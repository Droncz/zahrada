#ifndef __NVS_FUNCS_H__
#define __NVS_FUNCS_H__

esp_err_t nvs_read_int32(int32_t *num, char *num_name);
esp_err_t nvs_save_int32(int32_t num, char *num_name);

#endif // __NVS_FUNCS_H__