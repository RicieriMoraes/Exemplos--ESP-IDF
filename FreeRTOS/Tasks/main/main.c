#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Estrutura simples de Task
void Task1(void * param) // Cria entrada vazia de parâmetro em ()
{
    while (1) {
        
        // Printa no serial texto seguido da recuperação do parâmetro/informação (ponteiro) de entrada
        printf("Realizando leitura de temperatura %s \n", (char *) param);
        
        // Delay necessário para liberar janela de tempo
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Estrutura simples de Task
void Task2(void * param) // Cria entrada vazia de parâmetro em ()
{
    while (1) {

        // Printa no serial texto seguido da recuperação do parâmetro/informação (ponteiro) de entrada
        printf("Realizando leitura de umidade %s \n", (char *) param);

        // Delay necessário para liberar janela de tempo
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Função loop principal
void app_main(void)
{
    // Criação de task recebendo em ponteiro o nome da task (função)
    // Em seguida o nome da task, memória necessária alocada, parâmetros de entrada,
    // prioridade da task, handler (efetua atualização, modificação ou deleta)
    xTaskCreate(&Task1, "Leitura de temperatura", 2048, "Task 1", 2, NULL);


    // Criação de task recebendo em ponteiro o nome da task (função)
    // Em seguida o nome da task, memória necessária alocada, parâmetros de entrada,
    // prioridade da task, handler (efetua atualização, modificação ou deleta)
    xTaskCreate(&Task2, "Leitura de umidade", 2048, "Task 2", 2, NULL);
}
