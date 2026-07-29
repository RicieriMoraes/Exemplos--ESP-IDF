/*
 * NOME: Ricieri Juan
 * DATA: 29/07/2026
 * PROJETO: DAC
 * VERSÃO: 1.0.0
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Biblioteca para configuração DAC
#include "driver/dac.h"

void app_main(void)
{
    // Habilitando DAC1
    dac_output_enable(DAC_CHANNEL_1);

    // Fórmula que converte tensão em PCM
    // volt = Vref * digi_val / 255;
    
    // Vref é o valor de referência (3,3V)
    // 255 * volt = Vref * digi_val;

    // Diz o valor de tensão e obtém na saída
    // (255*volt)/3.3 = digi_val;

    while (1) {
        for (int i = 0; i < 255; i++) {

            // Escreve o valor de tensão em PCM
            dac_output_voltage(DAC_CHANNEL_1,i);

            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }
}
