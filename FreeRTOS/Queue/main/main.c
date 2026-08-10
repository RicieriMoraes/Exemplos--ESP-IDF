/*
 * NOME: Ricieri Juan
 * DATA: 10/08/2026
 * PROJETO: Queue
 * VERSÃO: 1.0.0
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Biblioteca necessário para utilização de fila (queue)
#include "freertos/queue.h"

// Handler de fila criado
QueueHandle_t queue;

// Task de envio de dado/notificação (texto serial)
void comTask(void * params)
{
    int counter = 0;

    while (1) {
        counter++;
        printf("Dado recebido \n");

        // Variável que recebe função que recebe informação da fila, elemento de memória da informação e tempo máximo de espera
        long result = xQueueSend(queue, &counter, 1000 / portTICK_PERIOD_MS);

        // Verifica se a inserção na fila foi realizada com sucesso
        if (result) {
            printf("Dado inserido na fila com sucesso \n");
        } else {
            printf("Erro ao inserir dado na fila \n");
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

// Task de recebidmento de dado/notificação (texto serial)
void procTask(void * params)
 {
     while (1) {
        int receive = 0;

        // Variável que recebe função que envia informação da fila, elemento de memória da informação e tempo máximo de espera
        long result = xQueueReceive(queue, &receive, 2000 / portTICK_PERIOD_MS);
        
        // Verifica se o recebimento na fila foi realizada com sucesso
        if (result) {
            printf("Iniciando processamento - Dado recebido: %d \n", receive);
        }
    }
}

void app_main(void)
{
    // Handle iniciado da fila com tamanho/posições de elementos e tamanho básico unitário
    queue = xQueueCreate(4, sizeof(int));

    // Verificação de alocação de inicialização do handle
    if (queue == NULL) {
        printf("Erro ao criar fila \n");
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