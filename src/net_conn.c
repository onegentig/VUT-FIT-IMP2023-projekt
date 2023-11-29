/**
 * @file net_conn.c
 * @author Onegen Something (xkrame00)
 * @brief Kód pre operácie s WiFi a MQTT
 * @date 2023-11-29
 */

#include "net_conn.h"

/* === Metódy === */

/**
 * @brief Inicializácia WiFi pripojenia
 */
void init_wifi() {
     ESP_ERROR_CHECK(esp_netif_init());  // Inicializovať IP stack
     ESP_ERROR_CHECK(esp_event_loop_create_default());  // Vytvoriť event loop
     esp_netif_create_default_wifi_sta();  // Vytvoriť WiFi rozhranie

     esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_handler,
                                NULL);
     esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_handler,
                                NULL);

     /* Konfigurácia */
     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
     ESP_ERROR_CHECK(esp_wifi_init(&cfg));  // Inicializovať WiFi

     wifi_config_t wifi_cfg = {
          .sta = {
               .ssid = NET_SSID,
               .password = NET_PASS,
               .threshold.authmode = WIFI_AUTH_WPA2_PSK,
          },
     };

     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
     ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_cfg));
     ESP_ERROR_CHECK(esp_wifi_start());
}

/**
 * @brief Event handler pre WiFi eventy
 * @param arg Argumenty eventu
 * @param event_base Základ eventu
 * @param event_id ID eventu
 * @param event_data Dáta eventu
 */
void wifi_handler(void *arg, esp_event_base_t event_base, int32_t event_id,
                       void *event_data) {
     switch (event_id) {
          case WIFI_EVENT_STA_START:
               ESP_LOGI(PROJNAME, "Pripájam sa...");
               esp_wifi_connect();
               break;

          case WIFI_EVENT_STA_CONNECTED:
               ESP_LOGI(PROJNAME, "Sieť pripojená!");
               break;

          case IP_EVENT_STA_GOT_IP:
               ESP_LOGI(PROJNAME, "IP pridelená, zapínam MQTT...");
               // mqtt_start();
               break;

          case WIFI_EVENT_STA_DISCONNECTED:
               ESP_LOGI(PROJNAME, "Sieť odpojená, skúšam znova...");
               esp_wifi_connect();
               break;

          default:
               break;
     }
}

/** @see https://github.com/ESP32Tutorials/esp32-mqtt-pub-sub-esp-idf */
