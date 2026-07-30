#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Declaração de handler
static TaskHandle_t tasksHandler = NULL;

// Declaração de array;
int DATA[5] = {0};

void Task1(void * params)
{
    while (1) {

        // Preenchimento do array
        DATA[0] = 10;

        // Função de envio de evento
        xTaskNotifyGive(tasksHandler);

        // Preenchimento do array
        DATA[1] = 20;

        // Função de envio de evento
        xTaskNotifyGive(tasksHandler);
        
        // Preenchimento do array
        DATA[2] = 30;

        // Função de envio de evento
        xTaskNotifyGive(tasksHandler);
        
        // Preenchimento do array
        DATA[3] = 40;

        // Função de envio de evento
        xTaskNotifyGive(tasksHandler);
        
        // Preenchimento do array
        DATA[4] = 50;

        // Função de envio de evento
        xTaskNotifyGive(tasksHandler);

        // Delay para possibilitar visualização
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void Task2(void * params)
{
    while (1) {

        // Função que acumula número de notificações recebdidas através de um contador
        // Primeiro parâmetro define se o contador será resetado ou mantido,
        // em seguida há o tempo de aguardo das notificações (tempo para incremento)
        // Para obter o retorno do contador, necessário atrelar esta função a uma variável (counter)
        int counter = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);

        // Exibe o texto e array com contador que recebe as notificações
        printf("Notificação recebida %d \n", DATA[counter-1]); // Elimina a contagem 0 sem conteúdo (-1)
    }
}

void app_main(void)
{
    // Criação de task recebendo em ponteiro o nome da task (função)
    // Em seguida o nome da task, memória necessária alocada, parâmetros de entrada,
    // prioridade da task, handler (efetua atualização, modificação ou deleta)
    xTaskCreate(&Task1, "Task1", 2048, "Task 1", 2, &tasksHandler);


    // Criação de task (com seleção de Core para execução) recebendo em ponteiro o nome da task (função)
    // Em seguida o nome da task, memória necessária alocada, parâmetros de entrada,
    // prioridade da task, handler (efetua atualização, modificação ou deleta)
    xTaskCreate(&Task2, "Task2", 2048, "Task 2", 2, &tasksHandler);
}
