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
     // ESP_LOGI(PROJNAME, "Pomaly zapínam LEDku...");
     fade_led_bright(100);

     ESP_LOGI(PROJNAME, "Čítam úroveň svetla...");
     uint16_t light_level = read_sensor_light();
     ESP_LOGI(PROJNAME, "Úroveň svetla: %u lux", light_level);

     // ESP_LOGI(PROJNAME, "Pomaly vypínam LEDku...");
     fade_led_bright(0);
}
