/**
 * @file serial_config.h
 * @author Onegen Something (xkrame00)
 * @brief Hlavičkový súbor pre serial_config.cpp
 * @date 2023-12-05
 */

#ifndef SERIAL_CONFIG_H
#define SERIAL_CONFIG_H

/* === Importy === */

#include <string.h>

#include "common.h"
#include "driver/uart.h"
#include "esp_timer.h"
#include "nvs.h"
#include "nvs_flash.h"

/* === Definície a globals === */

#define UART_BUF_SIZE 256
#define NVS_DATA_NAMESPACE "store"
#define NVS_UNDEFINED ESP_ERR_NVS_NOT_FOUND  // alias
#define ESP_WOULDBLOCK ESP_ERR_TIMEOUT       // BSD-style alias
#define QUIET_TIMEO 30000

extern char serial_buf[UART_BUF_SIZE];
extern int serial_buf_pos;
extern int64_t serial_last_recv;
extern bool is_config;

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

/* == UART == */

/**
 * @brief Inicializácia UARTu
 */
void init_uart();

/**
 * @brief Neblokujúci handling SERIAL komunikácie cez UART.
 * @return ESP_OK ak prebehlo úspešne,
 * @return ESP_FAIL ak nie
 */
esp_err_t handle_serial();

/**
 * @brief Spracuje prijatí príkaz
 * @param command Prijatý príkaz
 */
void handle_command(const char *command);

#endif