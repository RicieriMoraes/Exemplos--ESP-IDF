#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Declaração de handler
static TaskHandle_t tasksHandler = NULL;

void Task1(void * params)
{
    while (1) {

        // Delay para possibilitar visualização
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Função de envio de evento, parâmetro inteiro e ação (abaixo escrita)
        xTaskNotify(tasksHandler, 0b00001, eSetValueWithoutOverwrite);

        // Delay para possibilitar visualização
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Função de envio de evento, parâmetro inteiro e ação (abaixo escreve)
        xTaskNotify(tasksHandler, 0b00010, eSetValueWithoutOverwrite);

        // Delay para possibilitar visualização
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Função de envio de evento, parâmetro inteiro e ação (abaixo escreve)
        xTaskNotify(tasksHandler, 0b00100, eSetValueWithoutOverwrite);

        // Delay para possibilitar visualização
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Função de envio de evento, parâmetro inteiro e ação (abaixo seta bit "zero")
        xTaskNotify(tasksHandler, 0b01000, eSetBits);

        // Delay para possibilitar visualização
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Função de envio de evento, parâmetro inteiro e ação (abaixo seta bit "zero")
        xTaskNotify(tasksHandler, 0b10000, eSetBits);

        // Delay para possibilitar visualização
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void Task2(void * params)
{

    // Variável que recebe valores do handler
    uint32_t estado = 0;

    while (1) {

        // Função que aguarda notificações
        // Primeiro parâmetro refere-se a máscara (ou limpeza/reset) de entrada, enquanto o segundo é de saída
        // em seguida há o valor que está recebendo
        // Por fim, o tempo de aguardo das notificações
        xTaskNotifyWait(estado, 0, &estado, portMAX_DELAY);

        // Exibe o texto e variável que recebe as notificações através do handler
        printf("Notificação recebida %ld \n", estado); // estado pode ser utilizada como estado de máquina de estado
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
