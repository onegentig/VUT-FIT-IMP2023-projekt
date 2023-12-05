/**
 * @file serial_config.c
 * @author Onegen Something (xkrame00)
 * @brief Kód pre operácie s NVS a UART
 * @date 2023-12-05
 */

#include "serial_config.h"

/* === Metódy === */

/* == NVS (Non-Volatile Storage) ==*/

/**
 * @brief Inicializácia NVS pamäte
 */
void init_nvs() {
     esp_err_t err = nvs_flash_init();
     if (err == ESP_ERR_NVS_NO_FREE_PAGES
         || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
          ESP_ERROR_CHECK(nvs_flash_erase());
          err = nvs_flash_init();
     }

     ESP_ERROR_CHECK(err);
}

/**
 * @brief Uloženie u16 hodnoty do NVS pamäte
 * @param key Kľúč hodnoty na uloženie
 * @param val Hodnota na uloženie
 * @return ESP_OK ak bolo uloženie úspešné,
 * @return ESP_FAIL ak nie
 */
esp_err_t write_nvs_u16(const char *key, const uint16_t val) {
     nvs_handle_t nvs_handle;
     esp_err_t status;

     status = nvs_open(NVS_DATA_NAMESPACE, NVS_READWRITE, &nvs_handle);
     if (status != ESP_OK) return status;

     status = nvs_set_u16(nvs_handle, key, val);
     if (status != ESP_OK) return status;

     status = nvs_commit(nvs_handle);
     nvs_close(nvs_handle);
     return status;
}

/**
 * @brief Prečítanie u16 hodnoty z NVS pamäte
 * @param key Kľúč požadovanej hodnoty
 * @param val Ukazateľ na uloženie hodnoty
 * @return ESP_OK ak bolo čítanie úspešné,
 * @return ESP_FAIL ak nie
 */
esp_err_t read_nvs_u16(const char *key, uint16_t *val) {
     nvs_handle_t nvs_handle;
     esp_err_t status;

     status = nvs_open(NVS_DATA_NAMESPACE, NVS_READWRITE, &nvs_handle);
     if (status != ESP_OK) return status;

     status = nvs_get_u16(nvs_handle, key, val);
     if (status != ESP_OK) return status;

     nvs_close(nvs_handle);
     return status;
}
