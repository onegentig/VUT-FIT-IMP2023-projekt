/**
 * @file main.c
 * @author Onegen Something (xkrame00)
 * @brief Hlavný zdrojový súbor projektu
 * @date 2023-11-22
 */

#include "main.h"

/* === Globals === */

const char* PROJNAME = "imp-light";

/* === Metódy === */

/* == Hlavné metódy == */

void setup() {
     /* Inicializácia I²C kontroléra */
     i2c_config_t i2c_ctrl_cfg = {
          // Kontrolér (master) mód
          .mode = I2C_MODE_MASTER,
          // Štandardné porty SDA (data) a SCL (clock)
          .sda_io_num = PORT_I2C_DATA,
          .scl_io_num = PORT_I2C_CLOCK,
          // Povolenie pull-up rezistorov
          .sda_pullup_en = GPIO_PULLUP_ENABLE,
          .scl_pullup_en = GPIO_PULLUP_ENABLE,
          // Frekvencia I²C zbernice
          .master.clk_speed = I2C_FREQ_HZ
     };

     ESP_ERROR_CHECK(i2c_param_config(I2C_NUM, &i2c_ctrl_cfg));
     ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM, I2C_MODE_MASTER, 0, 0, 0));

     /* Komponenty */
     init_led();

     if (!test_sensor() || !start_sensor()) {
          ESP_LOGE(PROJNAME,
                   "Senzor svetla nebolo možné otestovať alebo zapnúť!");
          return;
     }

     ESP_LOGI(PROJNAME, "Hardvér inicializovaný!");
}

void loop() {
     uint16_t light_lux = read_sensor_light();
     uint8_t light_pct = lux_to_pct(light_lux);
     ESP_LOGI(PROJNAME, "Úroveň svetla: %u lux (%u%%)", light_lux, light_pct);
     fade_led_bright(light_pct);
     vTaskDelay(500 / portTICK_PERIOD_MS);
}
