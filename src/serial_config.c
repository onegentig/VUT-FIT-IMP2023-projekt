/**
 * @file serial_config.c
 * @author Onegen Something (xkrame00)
 * @brief Kód pre operácie s NVS a UART
 * @date 2023-12-05
 */

#include "serial_config.h"

/* === Globals === */

char serial_buf[UART_BUF_SIZE];
int serial_buf_pos = 0;
int64_t serial_last_recv = 0;
bool is_config = false;

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

/* == UART == */

/**
 * @brief Inicializácia UARTu
 */
void init_uart() {
     /* Konfigurácia UART (8-N-1) */
     const uart_config_t uart_config = {
         // Rýchlosť prenosu (pulzy/s)
         .baud_rate = 115200,
         // Veľkosť dátového bloku/slova
         .data_bits = UART_DATA_8_BITS,
         // Bez parity
         .parity = UART_PARITY_DISABLE,
         // Jeden stop-bit
         .stop_bits = UART_STOP_BITS_1,
         // Vypnutý hardware flow control (std)
         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
     };

     ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));
     ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE,
                                  UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE,
                                  UART_PIN_NO_CHANGE));
     ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0));
}

/**
 * @brief Neblokujúci handling SERIAL komunikácie cez UART.
 * @return ESP_OK ak prebehlo úspešne,
 * @return ESP_FAIL ak nie
 */
esp_err_t handle_serial() {
     char data[128];
     int len = uart_read_bytes(UART_NUM_0, data, sizeof(data) - 1,
                               100 / pdMS_TO_TICKS(100));
     int64_t time_now = esp_timer_get_time();

     /* Počas písania príkazu (aka neprázdny buffer) sú logy vypnuté */
     if (serial_buf_pos > 0) {
          esp_log_level_set("*", ESP_LOG_NONE);
     } else {
          esp_log_level_set("*", PROJ_LOG_LEVEL);
     }

     if (len < 0) return ESP_FAIL;         // Chyba pri čítaní
     if (len == 0) return ESP_WOULDBLOCK;  // Nie je čo spracovať

     uart_write_bytes(UART_NUM_0, data, len);  // Echo linky
     serial_last_recv = time_now;

     /* Spracovanie vstupu */
     for (int i = 0; i < len; i++) {
          if (data[i] == '\n') {
               // Koniec riadku => koniec príkazu
               serial_buf[serial_buf_pos] = '\0';
               handle_command(serial_buf);  // Spracovať príkaz
               serial_buf_pos = 0;          // Buffer reset
               continue;
          }

          // Ignorovať CR
          if (data[i] == '\r') continue;

          // Nie je koniec riadku => pridať do bufferu
          if (serial_buf_pos < sizeof(serial_buf) - 1) {
               serial_buf[serial_buf_pos++] = data[i];
          }
     }

     return ESP_OK;
}

/**
 * @brief Pošle error na sériovú linku (pomocná funkcia pre handle_command)
 * @param msg Správa
 */
void send_uart_err(const char *msg) {
     char out_buf[64];
     snprintf(out_buf, sizeof(out_buf), "err: %s", msg);
     uart_write_bytes(UART_NUM_0, out_buf, strlen(out_buf));
     uart_write_bytes(UART_NUM_0, "\r\n", 2);  // CR LF
}

/**
 * @brief Spracuje prijaý príkaz na sériovú linku
 * @param command Príkaz
 */
void handle_command(const char *command) {
     char cmd[16], arg[16];
     uint8_t n_parts = sscanf(command, "%15s %15s", cmd, arg);

     if (n_parts == 0 || n_parts == EOF)
          return send_uart_err("no cmd/parse err");

     char out_buf[64];  // Výstupný buffer

     /* Handler príkazu */
     if (strcmp(cmd, "min") == 0) {
          if (n_parts == 2) {
               /* Nastavenie nového MIN_LUX */
               // Parse a kontrola hodnoty
               uint16_t val;
               if (sscanf(arg, "%hu", &val) != 1)
                    return send_uart_err("arg is nan");
               if (val > project_config.max_lux)
                    return send_uart_err("min > max");

               // Zápis
               esp_err_t status = write_nvs_u16("min_lux", val);
               if (status != ESP_OK)
                    return send_uart_err(esp_err_to_name(status));

               snprintf(out_buf, sizeof(out_buf), "ok");
               project_config.min_lux = val;
          } else {
               /* Výpis aktuálneho MIN_LUX */
               snprintf(out_buf, sizeof(out_buf), "%d", project_config.min_lux);
          }
     } else if (strcmp(cmd, "max") == 0) {
          if (n_parts == 2) {
               /* Nastavenie nového MAX_LUX */
               // Parse a kontrola hodnoty
               uint16_t val;
               if (sscanf(arg, "%hu", &val) != 1)
                    return send_uart_err("arg is nan");
               if (val < project_config.min_lux)
                    return send_uart_err("max < min");

               // Zápis
               esp_err_t status = write_nvs_u16("max_lux", val);
               if (status != ESP_OK)
                    return send_uart_err(esp_err_to_name(status));

               snprintf(out_buf, sizeof(out_buf), "ok");
               project_config.max_lux = val;
          } else {
               /* Výpis aktuálneho MAX_LUX */
               snprintf(out_buf, sizeof(out_buf), "%d", project_config.max_lux);
          }
     } else {
          snprintf(out_buf, sizeof(out_buf), "???");
     }

     /* Odpoveď na sériovú linku */
     uart_write_bytes(UART_NUM_0, out_buf, strlen(out_buf));
     uart_write_bytes(UART_NUM_0, "\r\n", 2);  // CR LF
}