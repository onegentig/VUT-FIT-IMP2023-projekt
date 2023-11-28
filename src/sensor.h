/**
 * @file sensor.h
 * @author Onegen Something (xkrame00)
 * @brief Hlavičkový súbor pre sensor.cpp
 * @date 2023-11-28
 */

#ifndef IMPLIGHT_SENSOR_H
#define IMPLIGHT_SENSOR_H

/* === Importy === */

#include "common.h"

/* === Definície a globals === */

#define I2C_SENSOR_ADDR 0x23  // Adresa senzora svetla (ADDR pin na GND)

/** @see https://www.laskakit.cz/user/related_files/bh1750fvi-e-186247.pdf#page=5 */
#define BH1750FVI_SHUTDOWN 0x00    /**< Kód inštr. pre vypnutie senzora */
#define BH1750FVI_POWERON 0x01	/**< Kód inštr. pre zapnutie senzora */
#define BH1750FVI_RESET 0x07	     /**< Kód inštr. pre reset senzora */
#define BH1750FVI_CONT_HRES 0x10   /**< Kód inštr. pre kontinuálny režim s vysokým rozl. */
#define BH1750FVI_CONT_HRES2 0x11  /**< Kód inštr. pre alt. kontinuálny režim s vysokým rozl. */
#define BH1750FVI_CONT_LRES 0x13   /**< Kód inštr. pre kontinuálny režim s nízkym rozl. */
#define BH1750FVI_ONE_HRES 0x20	/**< Kód inštr. pre jednorázový režim s vysokým rozl. */
#define BH1750FVI_ONE_HRES2 0x21   /**< Kód inštr. pre alt. jednorázový režim s vysokým rozl. */
#define BH1750FVI_ONE_LRES 0x23	/**< Kód inštr. pre jednorázový režim s nízkym rozl. */

/* === Metódy === */

/**
 * @brief Zapne senzor svetla na kontinuálny režim
 * @see https://www.laskakit.cz/user/related_files/bh1750fvi-e-186247.pdf#page=5
 * @return true Ak sa podarilo zapnúť,
 * @return false ak nie
 */
bool start_sensor();

/**
 * @brief Otestuje pripojenie senzora svetla
 * @return true Ak je senzor pripojený,
 * @return false ak nie
 */
bool test_sensor();

/**
 * @brief Prečíta úroveň svetla z senzora
 * @see https://www.laskakit.cz/user/related_files/bh1750fvi-e-186247.pdf#page=5
 * @return uint16_t Úroveň svetla, NULL ak sa nepodarilo prečítať
 */
uint16_t read_sensor_light();

#endif