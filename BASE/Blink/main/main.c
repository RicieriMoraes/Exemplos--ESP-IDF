/*
 * NOME: Ricieri Juan
 * DATA: 29/07/2026
 * PROJETO: Blink
 * VERSÃO: 1.0.0
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

// Biblioteca para acesso a GPIO's
#include "driver/gpio.h"

// Definição de pino de controle do LED
#define PIN 10

void app_main(void)
{
    // Configuração de sentido do pino do LED
    gpio_set_direction(PIN, GPIO_MODE_OUTPUT);

    // Variável booleana 
    bool status = 0;

    // Define loop do programa
    while (1) {

        // Inverte estado da variável booleana
        status = !status;

        // Seta/escreve status do pino
        gpio_set_level(PIN, status);

        // Define delay dividido pelo ciclo de máquina em milissegundos
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Tempo em que o core estará executando outra task
    }
}
