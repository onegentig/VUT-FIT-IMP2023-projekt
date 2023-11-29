/**
 * @file led.c
 * @author Onegen Something (xkrame00)
 * @brief Kód pre ovládanie LEDky
 * @date 2023-11-23
 */

#include "led.h"

/* === Globals === */

uint32_t led_bright = 0;

/* === Metódy === */

/**
 * @brief Nakonfiguruje LEDku (časovač, PWM)
 */
void init_led() {
     gpio_reset_pin(PORT_LED);
     gpio_set_direction(PORT_LED, GPIO_MODE_OUTPUT);
     gpio_set_level(PORT_LED, 0);

     ledc_timer_config_t led_timer_data = {
         // Rýchly mód (hsch) => rýchla aplikácia zmeny
         .speed_mode = LEDC_HIGH_SPEED_MODE,
         // ID časovača (h_timer0)
         .timer_num = LEDC_TIMER_0,
         // 10-bitové rozlíšenie (0 do 1023)
         .duty_resolution = LEDC_TIMER_10_BIT,
         // 5 kHz frekvencia
         .freq_hz = 5000,
         // Automatický výber zdroja hodín
         .clk_cfg = LEDC_AUTO_CLK,
     };

     ledc_channel_config_t led_channel_data = {
         // Kanál časovača na GPIO5
         .gpio_num = PORT_LED,
         // Rýchly mód
         .speed_mode = LEDC_HIGH_SPEED_MODE,
         // ID kanálu
         .channel = LEDC_CHANNEL_0,
         // Vypnutie prerušení (nepotrebujeme)
         .intr_type = LEDC_INTR_DISABLE,
         // Použitie časovača h_timer0
         .timer_sel = LEDC_TIMER_0,
         // Počiatočná hodnota striedy
         .duty = 0,
     };

     ESP_ERROR_CHECK(ledc_timer_config(&led_timer_data));
     ESP_ERROR_CHECK(ledc_channel_config(&led_channel_data));
}

/**
 * @brief Okamžitá zmena jasu LEDky na danú striadu
 * @param duty 10bitová striada (0 až 1023)
 */
void set_led_bright(uint32_t duty) {
     ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
     ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
     led_bright = duty;
}

/**
 * @brief Skonvertuje percentáž na 10-bitovú striadu
 * @param percent Percentáž jasu (0 až 100)
 * @return 10-bitová striada (0 až 1023)
 */
uint32_t pct_to_duty(uint8_t percent) {
     uint32_t duty = (uint32_t)((percent * 1024) / 100);
     return (duty > 1023) ? 1023 : duty;
}

/**
 * @brief Skonvertuje 10-bitovú striadu na percentáž
 * @param duty 10-bitová striada (0 až 1023)
 * @return Percentáž jasu (0 až 100)
 */
uint8_t duty_to_pct(uint32_t duty) {
     uint8_t percent = (uint8_t)((duty * 100) / 1024);
     return (percent > 100) ? 100 : percent;
}

/**
 * @brief Postupná/linearizovaná zmena jasu LEDky na danú percentáž
 * @param percent Percentáž jasu (0 až 100)
 */
void fade_led_bright(uint8_t percent) {
     uint8_t current_pct = duty_to_pct(led_bright);
     uint8_t step = (current_pct > percent) ? -1 : 1; // Smer zmeny
     int t_step = LED_RAMPUP_TIME / 100; // Čas jedného kroku v ms
     bool mqtt_publish_next = true;

     while (current_pct != percent) {
          current_pct += step;
          set_led_bright(pct_to_duty(current_pct));
          vTaskDelay(t_step / portTICK_PERIOD_MS);

          /* MQTT update (každý druhý krok) */
          if (is_mqtt_connected && mqtt_publish_next) {
               mqtt_send_led(current_pct, led_bright);
          }

          mqtt_publish_next = !mqtt_publish_next;
     }
}