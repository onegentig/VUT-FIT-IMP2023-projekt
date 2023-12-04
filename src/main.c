/**
 * @file main.c
 * @author Onegen Something (xkrame00)
 * @brief Hlavný zdrojový súbor projektu
 * @date 2023-11-22
 */

#include "main.h"

/* === Globals === */

const char* PROJNAME = "implight";

/* === Metódy === */

/**
 * @brief Task pre publikovanie logov na MQTT broker
 *        a logging.
 */
void publish_log_task() {
     while (true) {
          uint32_t led_duty = get_led_bright();
          uint8_t led_pct = duty_to_pct(led_duty);
          uint16_t light_lux = read_sensor_light();

          /* Log */
          ESP_LOGI(PROJNAME, "Úroveň svetla: %u lux (%u%%)", light_lux,
                   led_pct);

          /* MQTT */
          if (is_mqtt_connected) {
               mqtt_send_led(led_pct, led_duty);
               mqtt_send_sensor(light_lux);
          }

          // Odoslať každé 2 sekundy
          vTaskDelay(2000 / portTICK_PERIOD_MS);
     }
}

/* == Hlavné metódy == */

void setup() {
     /* Inicializácia NVC */
     esp_err_t nvs_status = nvs_flash_init();
     if (nvs_status == ESP_ERR_NVS_NO_FREE_PAGES
         || nvs_status == ESP_ERR_NVS_NEW_VERSION_FOUND) {
          ESP_ERROR_CHECK(nvs_flash_erase());
          nvs_status = nvs_flash_init();
     }
     ESP_ERROR_CHECK(nvs_status);

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
     if (!test_sensor() || !start_sensor()) {
          ESP_LOGE(PROJNAME,
                   "Senzor svetla nebolo možné otestovať alebo zapnúť!");
          return;
     }

     init_led();
     init_wifi();

     ESP_LOGI(PROJNAME, "Hardvér inicializovaný!");

     /* MQTT publishing task */
     xTaskCreate(publish_log_task, "publish_log_task", 4096, NULL, 1, NULL);
}

void loop() {
     /* Nastaviť LED podľa senzora */
     uint16_t light_lux
         = read_sensor_light();  // Získať vonkajšie svetlo zo senzora

     uint8_t light_pct
         = 100 - lux_to_pct(light_lux);  // Prepočítať na svietivosť LEDky

     fade_led_bright(light_pct);  // Linearizovaná zmena jasu LEDky

     vTaskDelay(500 / portTICK_PERIOD_MS);
}
