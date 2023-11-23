/**
 * @file common.h
 * @author Onegen Something (xkrame00)
 * @brief Hlavičkový súbor pre zdieľané importy a definície
 */

#ifndef IMPLIGHT_COMMON_H
#define IMPLIGHT_COMMON_H

/* === Importy === */

#include <stdbool.h>
#include <stdio.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* === Definície === */

extern const char *PROJNAME;

/* == Časové konštanty == */
// inšpirované npm modulom "@sapphire/time-utilities"

#define TIME_MS portTICK_PERIOD_MS
#define TIME_SECOND (1000 / portTICK_PERIOD_MS)
#define TIME_MINUTE (60 * TIME_SECOND)
#define TIME_HOUR (60 * TIME_MINUTE)

#endif
