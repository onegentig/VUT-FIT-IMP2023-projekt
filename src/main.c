/**
 * @file main.c
 * @author Onegen Something (xkrame00)
 * @brief Hlavný zdrojový súbor projektu
 * @date 2023-11-22
 */

#include "main.h"

/* === Globals === */

bool led_state = true;

/* === Hlavné metódy === */

void setup() {
     gpio_reset_pin(BLINK_PORT);
     gpio_set_direction(BLINK_PORT, GPIO_MODE_OUTPUT);

     ESP_LOGI(PROJNAME, "Hardvér inicializovaný!");
}

void loop() {
     ESP_LOGI(PROJNAME, "%s LEDku!", led_state == true ? "ZAPÍNAM" : "VYPÍNAM");
     gpio_set_level(BLINK_PORT, led_state);
     vTaskDelay(1000 / portTICK_PERIOD_MS);
     led_state = !led_state;
}
