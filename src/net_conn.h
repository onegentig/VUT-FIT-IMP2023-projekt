/**
 * @file net_conn.h
 * @author Onegen Something (xkrame00)
 * @brief Hlavičkový súbor pre net_conn.cpp
 * @date 2023-11-29
 */

#ifndef IMPLIGHT_NETCONN_H
#define IMPLIGHT_NETCONN_H

/* === Importy === */

#include "common.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"

/* === Definície a globals === */

#define NET_SSID "onegen-uy.23"
#define NET_PASS "kreativne-heslo"

/* === Metódy === */

/**
 * @brief Inicializácia WiFi pripojenia
 */
void init_wifi();

/**
 * @brief Event handler pre WiFi eventy
 * @param arg Argumenty eventu
 * @param event_base Základ eventu
 * @param event_id ID eventu
 * @param event_data Dáta eventu
 */
void wifi_handler(void *arg, esp_event_base_t event_base, int32_t event_id,
                       void *event_data);

#endif