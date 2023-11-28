/**
 * @file main.h
 * @author Onegen Something (xkrame00)
 * @brief Hlavný hlavičkový súbor projektu
 * @date 2023-11-22
 */

#ifndef IMPLIGHT_MAIN_H
#define IMPLIGHT_MAIN_H

/* === Importy === */

#include "common.h"
#include "led.h"
#include "sensor.h"

/* === Definície a globals === */

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

#endif
