/**
 * @file common.h
 * @author Onegen Something (xkrame00)
 * @brief Hlavičkový súbor pre zdieľané importy a definície
 */

/* === Importy === */

#include <stdbool.h>
#include <stdio.h>

#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* === Definície === */

/* == Časové konštanty == */
// inšpirované npm modulom "@sapphire/time-utilities"

#define TIME_MS portTICK_PERIOD_MS
#define TIME_SECOND (1000 / portTICK_PERIOD_MS)
#define TIME_MINUTE (60 * TIME_SECOND)
#define TIME_HOUR (60 * TIME_MINUTE)

/* === Konštanty === */

const char *PROJNAME = "imp-light";
