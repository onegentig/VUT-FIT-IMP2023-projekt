/**
 * @file led.h
 * @author Onegen Something (xkrame00)
 * @brief Hlavičkový súbor pre led.cpp
 * @date 2023-11-23
 */

#ifndef IMPLIGHT_LED_H
#define IMPLIGHT_LED_H

/* === Importy === */

#include "common.h"
#include "driver/ledc.h"

/* === Definície a globals === */

#define PORT_LED GPIO_NUM_5
#define LED_RAMPUP_TIME (int)(1.5 * 1000)  // Čas prechodu z 0% na 100% v ms

extern uint32_t led_bright;

/* === Metódy === */

/**
 * @brief Nakonfiguruje LEDku (časovač, PWM)
 */
void init_led();

/**
 * @brief Okamžitá zmena jasu LEDky na danú striadu
 * @param duty 10bitová striada (0 až 1023)
 */
void set_led_bright(uint32_t duty);

/**
 * @brief Skonvertuje percentáž na 10-bitovú striadu
 * @param percent Percentáž jasu (0 až 100)
 * @return 10-bitová striada (0 až 1023)
 */
uint32_t pct_to_duty(uint8_t percent);

/**
 * @brief Skonvertuje 10-bitovú striadu na percentáž
 * @param duty 10-bitová striada (0 až 1023)
 * @return Percentáž jasu (0 až 100)
 */
uint8_t duty_to_pct(uint32_t duty);

/**
 * @brief Postupná/linearizovaná zmena jasu LEDky na danú percentáž
 * @param percent Percentáž jasu (0 až 100)
 */
void fade_led_bright(uint8_t percent);

#endif