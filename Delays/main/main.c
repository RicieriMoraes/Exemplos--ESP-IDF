#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#define PIN 10

void app_main(void)
{
    gpio_set_direction(PIN, GPIO_MODE_OUTPUT);

    bool status = 0;

    while (1) {
        gpio_set_level(PIN, 0);
        gpio_set_level(PIN, 1);

        // Delay de alta precisão independente
        // Utilizado para obter um tempo de pulso preciso e pequeno (microssegundos)
        esp_rom_delay_us(1000);

        gpio_set_level(PIN, 0);

        // Delay de menor precisão na task principal
        // Quando sozinho, pode acabar resetando o microcontrolador, devido tempo muito pequeno para execução
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
