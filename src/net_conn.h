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
#include "mqtt_client.h"

/* === Definície a globals === */

#define NET_SSID "onegen-uy.23"
#define NET_PASS "kreativne-heslo"
#define MQTT_HOST "mqtt://broker.hivemq.com:1883"
#define MQTT_TOPIC "xkrame00/"

extern bool is_mqtt_connected;
extern esp_mqtt_client_handle_t mqtt_client;

/* === Metódy === */

/**
 * @brief Inicializácia WiFi pripojenia
 */
void init_wifi();

/**
 * @brief Init MQTT pripojenia
 */
void init_mqtt();

/**
 * @brief Odoslanie stavu LED na MQTT xkrame00/imp-light/led
 * @param pct Svietivosť LED v %
 * @param duty 10b strieda LEDky
 */
void mqtt_send_led(uint8_t pct, uint32_t duty);

/**
 * @brief Odoslanie stavu senzora na MQTT xkrame00/imp-light/sensor
 * @param lux Úroveň svetla v luxoch
 */
void mqtt_send_sensor(uint16_t lux);

/* == Handlery == */

/**
 * @brief Event handler pre WiFi eventy
 * @param arg Argumenty eventu
 * @param event_base Základ eventu
 * @param event_id ID eventu
 * @param event_data Dáta eventu
 */
void wifi_handler(void *arg, esp_event_base_t event_base, int32_t event_id,
                  void *event_data);

/**
 * @brief Event handler pre MQTT eventy
 * @param handler_args Argumenty handlera
 * @param event_base Základ eventu
 * @param event_id ID eventu
 * @param event_data Dáta eventu
 */
void mqtt_handler(void *handler_args, esp_event_base_t event_base,
                  int32_t event_id, void *event_data);

#endif