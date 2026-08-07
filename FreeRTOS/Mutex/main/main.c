#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Biblioteca necessário para utilização de mutex e semáforo
#include "freertos/semphr.h"

// Função handle (Mutex criado)
SemaphoreHandle_t mutexBus;

// Função de aliteração de alocação de recurso (neste caso, monitor serial)
// Recurso único
void writeBus(char *mgs)
{
    printf(mgs);
}

// Estrutura simples de Task
void Task1(void * param) // Cria entrada vazia de parâmetro em ()
{
    while (1) {

        // Estrutura de decisão para mitigar envio e erro
        // Primeiro campo sendo handle
        // Segundo campo como tempo máximo para aguardar liberação do recurso
        if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS)) {
            
            // Escreve no serial
            writeBus("Realizando leitura de temperatura \n");

            // Libera recurso (serial)
            xSemaphoreGive(mutexBus);

        } else {

            // Ainda não foi possível liberar o acesso ao serial
            writeBus("Timeout na leitura de temperatura \n");
        }
        
        // Delay necessário para liberar janela de tempo
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Estrutura simples de Task
void Task2(void * param) // Cria entrada vazia de parâmetro em ()
{
    while (1) {

        // Estrutura de decisão para mitigar envio e erro
        // Primeiro campo sendo handle
        // Segundo campo como tempo máximo para aguardar liberação do recurso
        if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS)) {

            // Escreve no serial
            writeBus("Realizando leitura de umidade \n");

            // Libera recurso (serial)
            xSemaphoreGive(mutexBus);

        } else {

            // Ainda não foi possível liberar o acesso ao serial
            writeBus("Timeout na leitura de umidade \n");
        }

        // Delay necessário para liberar janela de tempo
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    // Inicialização do handle na main()
    mutexBus = xSemaphoreCreateMutex();

    // Criação de task recebendo em ponteiro o nome da task (função)
    // Em seguida o nome da task, memória necessária alocada, parâmetros de entrada,
    // prioridade da task, handler (efetua atualização, modificação ou deleta)
    xTaskCreate(&Task1, "Leitura de temperatura", 4096, "Task 1", 2, NULL);

    // Criação de task recebendo em ponteiro o nome da task (função)
    // Em seguida o nome da task, memória necessária alocada, parâmetros de entrada,
    // prioridade da task, handler (efetua atualização, modificação ou deleta)
    xTaskCreate(&Task2, "Leitura de umidade", 4096, "Task 2", 2, NULL);
}
