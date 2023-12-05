/**
 * @file serial_config.h
 * @author Onegen Something (xkrame00)
 * @brief Hlavičkový súbor pre serial_config.cpp
 * @date 2023-12-05
 */

#ifndef SERIAL_CONFIG_H
#define SERIAL_CONFIG_H

/* === Importy === */

#include "nvs.h"
#include "nvs_flash.h"

/* === Definície a globals === */

#define NVS_DATA_NAMESPACE "store"
#define NVS_UNDEFINED ESP_ERR_NVS_NOT_FOUND  // skratka

/* === Metódy === */

/* == NVS (Non-Volatile Storage) ==*/

/**
 * @brief Inicializácia NVS pamäte
 */
void init_nvs();

/**
 * @brief Uloženie u16 hodnoty do NVS pamäte
 * @param key Kľúč hodnoty na uloženie
 * @param val Hodnota na uloženie
 * @return ESP_OK ak bolo uloženie úspešné,
 * @return ESP_FAIL ak nie
 */
esp_err_t write_nvs_u16(const char *key, const uint16_t val);

/**
 * @brief Prečítanie u16 hodnoty z NVS pamäte
 * @param key Kľúč požadovanej hodnoty
 * @param val Ukazateľ na uloženie hodnoty
 * @return ESP_OK ak bolo čítanie úspešné,
 * @return ESP_FAIL ak nie
 */
esp_err_t read_nvs_u16(const char *key, uint16_t *val);

#endif