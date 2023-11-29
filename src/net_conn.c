/**
 * @file net_conn.c
 * @author Onegen Something (xkrame00)
 * @brief Kód pre operácie s WiFi a MQTT
 * @date 2023-11-29
 */

#include "net_conn.h"

/* === Globals === */

bool is_mqtt_connected = false;
esp_mqtt_client_handle_t mqtt_client = NULL;

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
 * @brief Init MQTT pripojenia
 */
void init_mqtt() {
     esp_mqtt_client_config_t mqtt_cfg = {
         .broker.address.uri = MQTT_HOST,
     };

     mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
     if (mqtt_client == NULL) {
          ESP_LOGE(PROJNAME, "Chyba pri inicializácii MQTT klienta!");
          return;
     }

     esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_handler,
                                    mqtt_client);
     esp_mqtt_client_start(mqtt_client);
}

/**
 * @brief Odoslanie stavu LED na MQTT xkrame00/imp-light/led
 * @param pct Svietivosť LED v %
 * @param duty 10b strieda LEDky
 */
void mqtt_send_led(uint8_t pct, uint32_t duty) {
     if (!is_mqtt_connected) {
          return;
     }

     char topic[64], msg[100];

     // xkrame00/led/brightness (aktuálna svietivosť LED v %)
     snprintf(topic, sizeof(topic), "%sled/brightness", MQTT_TOPIC);
     snprintf(msg, sizeof(msg), "%u", pct);
     esp_mqtt_client_publish(mqtt_client, topic, msg, 0, 1, 0);

     // xkrame00/led/duty (aktuálna strieda LED)
     snprintf(topic, sizeof(topic), "%sled/duty", MQTT_TOPIC);
     snprintf(msg, sizeof(msg), "%lu", duty);
     esp_mqtt_client_publish(mqtt_client, topic, msg, 0, 1, 0);
}

/**
 * @brief Odoslanie stavu senzora na MQTT xkrame00/imp-light/sensor
 * @param lux Úroveň svetla v luxoch
 */
void mqtt_send_sensor(uint16_t lux) {
     if (!is_mqtt_connected) {
          return;
     }

     char topic[64], msg[100];

     // xkrame00/sensor (aktuálna úroveň svetla v luxoch)
     snprintf(topic, sizeof(topic), "%ssensor", MQTT_TOPIC);
     snprintf(msg, sizeof(msg), "%u", lux);
     esp_mqtt_client_publish(mqtt_client, topic, msg, 0, 1, 0);
}

/* == Handlery == */

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
               init_mqtt();
               break;

          case WIFI_EVENT_STA_DISCONNECTED:
               ESP_LOGI(PROJNAME, "Sieť odpojená, skúšam znova...");
               esp_wifi_connect();
               break;

          default:
               break;
     }
}

/**
 * @brief Event handler pre MQTT eventy
 * @param handler_args Argumenty handlera
 * @param event_base Základ eventu
 * @param event_id ID eventu
 * @param event_data Dáta eventu
 */
void mqtt_handler(void *handler_args, esp_event_base_t base, int32_t event_id,
                  void *event_data) {
     switch (event_id) {
          case MQTT_EVENT_CONNECTED:
               ESP_LOGI(PROJNAME, "MQTT: Pripojený!");
               is_mqtt_connected = true;
               break;

          case MQTT_EVENT_DISCONNECTED:
               ESP_LOGI(PROJNAME, "MQTT: Odpojený!");
               is_mqtt_connected = false;
               break;

          default:
               break;
     }
}
