/**
 * @file sensor.c
 * @author Onegen Something (xkrame00)
 * @brief Kód pre ovládanie sveteľného senzora
 * @date 2023-11-28
 */

#include "sensor.h"

/* === Metódy === */

/**
 * @brief Zapne senzor svetla na kontinuálny režim
 * @see https://www.laskakit.cz/user/related_files/bh1750fvi-e-186247.pdf#page=5
 * @return true Ak sa podarilo zapnúť,
 * @return false ak nie
 */
bool start_sensor() {
     /* Vytvorenie I²C linky */
     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
     if (cmd == NULL) {
          ESP_LOGE(PROJNAME, "Chyba (cmd == NULL) pri vytváraní I²C linky!");
          return false;
     }

     /* Sekvencia príkazov */
     i2c_master_start(cmd);  // Začiatok komunikácie
     i2c_master_write_byte(cmd, (I2C_SENSOR_ADDR << 1) | I2C_MASTER_WRITE,
                           true);  // MASTER_WRITE na adresu
     i2c_master_write_byte(cmd, BH1750FVI_CONT_HRES,
                           true);  // Nastavenie CONT. H-RES módu
     i2c_master_stop(cmd);         // Koniec komunikácie

     /* Spustenie príkazov */
     esp_err_t status = i2c_master_cmd_begin(I2C_NUM, cmd, TIME_SECOND);
     i2c_cmd_link_delete(cmd);  // Uvoľnenie linky

     return (status == ESP_OK);
}

/**
 * @brief Otestuje pripojenie senzora svetla poslaním POWERON inštrukcie
 * @return true Ak je senzor pripojený,
 * @return false ak nie
 */
bool test_sensor() {
     /* Vytvorenie I²C linky */
     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
     if (cmd == NULL) {
          ESP_LOGE(PROJNAME, "Chyba (cmd == NULL) pri vytváraní I²C linky!");
          return false;
     }

     /* Sekvencia príkazov */
     i2c_master_start(cmd);  // Začiatok komunikácie
     i2c_master_write_byte(cmd, (I2C_SENSOR_ADDR << 1) | I2C_MASTER_WRITE,
                           true);  // MASTER_WRITE na adresu
     i2c_master_write_byte(cmd, BH1750FVI_POWERON, 0x1);  // Zapnutie
     i2c_master_stop(cmd);                                // Koniec komunikácie

     /* Spustenie príkazov */
     esp_err_t status = i2c_master_cmd_begin(I2C_NUM, cmd, TIME_SECOND);
     i2c_cmd_link_delete(cmd);  // Uvoľnenie linky

     return (status == ESP_OK);
}

/**
 * @brief Prečíta úroveň svetla z senzora
 * @see https://www.laskakit.cz/user/related_files/bh1750fvi-e-186247.pdf#page=5
 * @return uint16_t Úroveň svetla
 */
uint16_t read_sensor_light() {
     uint8_t data_h, data_l;  // BH1750FVI posiela 16b úroveň svetla po dvoch 8b
                              // častiach (H:L)

     /* Vytvorenie I²C linky */
     i2c_cmd_handle_t cmd = i2c_cmd_link_create();
     if (cmd == NULL) {
          ESP_LOGE(PROJNAME, "Chyba (cmd == NULL) pri vytváraní I²C linky!");
          return 0;
     }

     /* Sekvencia príkazov */
     i2c_master_start(cmd);  // Začiatok komunikácie
     i2c_master_write_byte(cmd, (I2C_SENSOR_ADDR << 1) | I2C_MASTER_READ,
                           true);  // MASTER_READ z adresy
     i2c_master_read_byte(cmd, &data_h, I2C_MASTER_ACK);        // High byte
     i2c_master_read_byte(cmd, &data_l, I2C_MASTER_LAST_NACK);  // Low byte
     i2c_master_stop(cmd);  // Koniec komunikácie

     /* Spustenie príkazov */
     esp_err_t status = i2c_master_cmd_begin(I2C_NUM, cmd, TIME_SECOND);
     i2c_cmd_link_delete(cmd);  // Uvoľnenie linky

     if (status != ESP_OK) {
          ESP_LOGE(PROJNAME, "Chyba pri čítaní úrovne svetla!");
          return 0;
     }

     return (data_h << 8) | data_l;  // Spojenie dvoch 8b častí
}

/**
 * @brief Prekonvertuje úroveň svetla na percentá
 * @return uint8_t Úroveň svetla v percentách
 */
uint8_t lux_to_pct(uint16_t lux) {
     if (lux <= project_config.min_lux) return 0;
     if (lux >= project_config.max_lux) return 100;

     uint16_t range
         = project_config.max_lux - project_config.min_lux;  // Rozsah hodnôt
     uint16_t scaled_lux
         = lux - project_config.min_lux;  // Hodnota lux posunutá na MIN=0

     uint8_t pct = (uint8_t)((scaled_lux * 100) / range);
     return (pct > 100) ? 100 : pct;
}
