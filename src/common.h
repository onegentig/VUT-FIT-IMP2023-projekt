/**
 * @file common.h
 * @author Onegen Something (xkrame00)
 * @brief Hlavičkový súbor pre zdieľané importy a definície
 */

#ifndef IMPLIGHT_COMMON_H
#define IMPLIGHT_COMMON_H

/* === Importy === */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* === Definície === */

#define PORT_I2C_DATA GPIO_NUM_21
#define PORT_I2C_CLOCK GPIO_NUM_22
#define I2C_FREQ_HZ 400000
#define I2C_NUM I2C_NUM_0
#define PROJ_LOG_LEVEL ESP_LOG_INFO

extern const char *PROJNAME;

/**
 * @brief Štrukt konfigurácie projektu
 * @param min_lux Minimálna hodnota svetla (pod => 0%)
 * @param max_lux Maximálna hodnota svetla (nad => 100%)
 */
typedef struct {
     uint16_t min_lux;
     uint16_t max_lux;
} project_config_t;

extern project_config_t project_config;

/* == Časové konštanty == */
// inšpirované npm modulom "@sapphire/time-utilities"

#define TIME_SECOND (1000 * (1 / portTICK_PERIOD_MS))
#define TIME_MINUTE (60 * TIME_SECOND)
#define TIME_HOUR (60 * TIME_MINUTE)

#endif
