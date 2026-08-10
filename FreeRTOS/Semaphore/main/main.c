/*
 * NOME: Ricieri Juan
 * DATA: 10/08/2026
 * PROJETO: Semaphore
 * VERSÃO: 1.0.0
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Biblioteca necessário para utilização de mutex e semáforo
#include "freertos/semphr.h"

// Handler de semafóro criado
SemaphoreHandle_t semaforo;

// Task de envio de dado/notificação (texto serial)
// Envia notificação no término do processamento
void comTask(void * params)
{
    while (1) {
        printf("Dado recebido \n");

        // Envia notificação a partir do handle
        xSemaphoreGive(semaforo);

        printf("Novo processamento \n");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

// Task de recebidmento de dado/notificação (texto serial)
void procTask(void * params)
{
    while (1) {

        // Recebe notificação a partir do handle
        xSemaphoreTake(semaforo, portMAX_DELAY);

        printf("Iniciando processamento \n");
    }
}

void app_main(void)
{
    // Handle iniciado
    semaforo = xSemaphoreCreateBinary();

    // Verificação de alocação de inicialização do handle
    if (semaforo == NULL) {
        printf("Erro ao criar semáforo \n");
        return;
    }

    // Criação de task recebendo em ponteiro o nome da task (função)
    // Em seguida o nome da task, memória necessária alocada, parâmetros de entrada,
    // prioridade da task, handle
    xTaskCreate(&comTask, "Task de comunicação", 2048, NULL, 2, NULL);

    // Criação de task recebendo em ponteiro o nome da task (função)
    // Em seguida o nome da task, memória necessária alocada, parâmetros de entrada,
    // prioridade da task, handle
    xTaskCreate(&procTask, "Task de execução", 2048, NULL, 1, NULL);
}