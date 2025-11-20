#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "TEMP";

void app_main(void)
{
    int temperature = 25;  // start temperature

    while (1) {
        // Simulate change (increase, then reset)
        temperature++;
        if (temperature > 40) {
            temperature = 25;
        }

        ESP_LOGI(TAG, "Temperature: %d °C", temperature);

        vTaskDelay(pdMS_TO_TICKS(1000));  // 1-second delay
    }
}


