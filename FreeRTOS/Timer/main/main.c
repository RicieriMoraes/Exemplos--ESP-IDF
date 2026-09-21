#include <stdio.h>
#include "freertos/FreeRTOS.h"

// Biblioteca para utilização de recursos de timer
#include "esp_timer.h"

// Biblioteca para retorno do tempo de execução
#include "esp_system.h"

// Função de disparo do timer no momento de estouro (entrada tipo TimerHandle_t)
void disparo(TimerHandle_t xTimer)
{
    printf("Disparado em %lld \n", esp_timer_get_time()/1000);
}

void app_main(void)
{
    // Printa texto com variável tipo long (tempo recebido do sistema)
    printf("Sistema iniciado em %lld \n", esp_timer_get_time()/1000);

    // Declaração de variável que recebe o resultado da saída da função xTimerCreate
    // contendo os campos nome, período de disparo, se é periódico ou único (false), ID e nome da função chamada quando o timer disparar
    TimerHandle_t xTimer = xTimerCreate("timer disparo", pdMS_TO_TICKS(500), false, NULL, disparo);
    
    // Início de execução do timer com campos nome e tempo de espera para inicialização do timer
    xTimerStart(xTimer, 0);
}
