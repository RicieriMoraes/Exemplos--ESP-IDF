/*
 * NOME: Ricieri Juan
 * DATA: 13/08/2026
 * PROJETO: Event-Group
 * VERSÃO: 1.0.0
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// Handler de grupo de evento
EventGroupHandle_t grupoEvt;

// Sinalizadores (são binários)
const EventBits_t goCom = BIT0; // 0b01
const EventBits_t goSensor = BIT1; // 0b010

// Task de envio de comunicação
void comTask(void * params)
{
    while (1) {
        printf("Dado recebido \n");

        // Envia notificação (bit setado) a partir do handle para sinalização
        xEventGroupSetBits(grupoEvt, goCom);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Task de envio do sinalizador do sensor
void sensorTask(void * params)
{
    while (1) {
        printf("Leitura realizada \n");

        // Envia notificação (bit setado) a partir do handle para sinalização
        xEventGroupSetBits(grupoEvt, goSensor);

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

// Task de processamento e recebimento
void procTask(void * params)
{
    while (1) {

        // Aguarda handler com os sinalizadores agrupados (menos siginificativo para mais significativo),
        // reseta no final (true), aguarda o envio de todos (true), e tempo de espera (máxima) 
        xEventGroupWaitBits(grupoEvt, goCom | goSensor, true, true, portMAX_DELAY);

        printf("Recebido a requisição e leitura do sensor \n");
    }
}

void app_main(void)
{
    // Criação de evento
    grupoEvt = xEventGroupCreate();

    xTaskCreate(&comTask, "Task de comunicação", 2048, NULL, 1, NULL);
    xTaskCreate(&sensorTask, "Task de sensor", 2048, NULL, 1, NULL);
    xTaskCreate(&procTask, "Task de execução", 2048, NULL, 1, NULL);
}
