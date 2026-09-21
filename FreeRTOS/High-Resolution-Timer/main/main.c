#include <stdio.h>
#include "esp_timer.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Função de disparo do timer
void disparoTimer1(void *arg)
{
    // Variável estática de status do pino
    static bool status;

    // Inversão de status
    status = !status;

    // Inversão do nível lógico do pino
    gpio_set_level(GPIO_NUM_21, status);
}

void app_main(void)
{
    // Configuração e definição de pino
    gpio_set_direction (GPIO_NUM_21, GPIO_MODE_OUTPUT);

    // Variável (estrutura) de configuração do timer com callback e nome
    const esp_timer_create_args_t timer1Conf = {
        .callback = disparoTimer1,
        .name = "Timer1"
    };

    // Handle do timer
    esp_timer_handle_t timer1Handle;

    // Criação do timer com configuração do timer (ponteiro) e handle (ponteiro)
    esp_timer_create(&timer1Conf, &timer1Handle);

    // Inicialização do timer com handle e período
    esp_timer_start_periodic(timer1Handle, 50);

    // Laço de repetição do timer 5 vezes
    for (int i = 0; i < 5; i++) {

        // Dump de informações atualizadas do timer como saída padrão
        esp_timer_dump(stdout);

        // A cada 1 segundo
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Para/pausa o timer
    esp_timer_stop(timer1Handle);

    // Reconfigura o tempo do timer
    //esp_timer_start_periodic(timer1Handle, 100);

    // Deleta o timer
    esp_timer_delete(timer1Handle);
}