#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#define PIN 10 // pino de saída (LED)
#define BT_PIN 9 // pino de entrada (botão)

void app_main(void)
{
    gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(BT_PIN, GPIO_MODE_INPUT);

    bool status = 0;

    while (1) {
        // Leitura digital do estado do pino
        status = gpio_get_level(BT_PIN);

        // Seta/escreve estado do LED através do nível inverso lido do botão
        gpio_set_level(PIN, !status);

        // Delay necessário, pois tem também a função de resetar watchdog timer
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
