/*
 * NOME: Ricieri Juan
 * DATA: 10/08/2026
 * PROJETO: Interrupção-Externa
 * VERSÃO: 1.0.0
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include"driver/gpio.h"

// Handler de interrupção (fila)
QueueHandle_t interruptQueue;

// Pino (botão)
#define PIN 9

// Variável contador
int counter;

// Função de interrupção que não retorna nada (static void) e é carregada diretamente na memória
static void IRAM_ATTR gpio4_isr_handler(void *params)
{
    // Variável com parâmetros do serviço de interrupção (obrigatório)
    int pinNumber = (int)params;

    // Envio de sinal para a fila a partir da interrupção, indicando a queue,
    // ponteiro da variável com parâmetros do serviço de interrupção, e parâmetro de retorno da interrupção
    xQueueSendFromISR(interruptQueue, &pinNumber, NULL);
}

// Task que recebe a fila de processamento
void signalTriggered(void *params)
{
    int pinNumber;

    while (1) {
        // Condição de recebimento da fila e interrupção (contador acrescenta a partir do evento)
        if (xQueueReceive(interruptQueue, &pinNumber, portMAX_DELAY)) {
            counter++;

            printf("Contador: %d \n", counter);
        }
    }
}

void app_main(void)
{
    // Configuração de pino como entrada
    gpio_set_direction(PIN, GPIO_MODE_INPUT);

    // Desabilitando pull down
    gpio_pulldown_dis(PIN);

    // Desabilitando pull up
    gpio_pullup_dis(PIN);

    // Configurando interrupção passando o pino e tipo de interrupção (ciclo positivo do sinal no exemplo)
    gpio_set_intr_type(PIN, GPIO_INTR_POSEDGE);

    // Configuração da queue (fila) com 10 espaços, inteiros
    interruptQueue = xQueueCreate(10, sizeof(int));

    xTaskCreate(signalTriggered, "signalTriggered", 4096, NULL, 1, NULL);

    // Instalação do serviço de interrupção
    gpio_install_isr_service(0);

    // Função que configura handler passando o pino, nome do handler (função) e argumento (vazio no exemplo)
    gpio_isr_handler_add(PIN, gpio4_isr_handler, (void *)PIN);
}
