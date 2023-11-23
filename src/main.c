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
     init_led();

     ESP_LOGI(PROJNAME, "Hardvér inicializovaný!");
}

void loop() {
     ESP_LOGI(PROJNAME, "Pomaly zapínam LEDku...");
     fade_led_bright(100);

     ESP_LOGI(PROJNAME, "Pomaly vypínam LEDku...");
     fade_led_bright(0);
}
