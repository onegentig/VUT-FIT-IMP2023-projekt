/**
 * @file main.h
 * @author Onegen Something (xkrame00)
 * @brief Hlavný hlavičkový súbor projektu
 * @date 2023-11-22
 */

/* === Importy === */

#include "common.h"

/* === Definície a globals === */

#define BLINK_PORT GPIO_NUM_5

/* === Metódy === */

/* == Hlavná metóda == */
// inšpirované Arduinom

void setup(void);
void loop(void);

void app_main(void) {
     setup();
     while (true) {
          loop();
     }
}
