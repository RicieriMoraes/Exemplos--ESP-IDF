/*
 * NOME: Ricieri Juan
 * DATA: 29/07/2026
 * PROJETO: ADC
 * VERSÃO: 1.0.0
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

// Biblioteca de configuração do ADC
#include "driver/adc.h"

void app_main(void)
{
    // Definição da resoluação do ADC
    adc1_config_width(ADC_WIDTH_BIT_12);

    // Configuração de canal ADC e atenuação do ganho
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11);

    // Configuração de canal ADC e atenuação do ganho
    adc2_config_channel_atten(ADC2_CHANNEL_0, ADC_ATTEN_DB_11);

    while (true) {
        int val1 = 0; // Receberá leitura ADC1
        int val2 = 0; // Receberá leitura ADC2

        // Leitura e retorno do ADC1
        val1 = adc1_get_raw(ADC1_CHANNEL_4);

        // ADC2 retorna somente status (ok ou erro)
        // Para retornar valor é necessário ponteiro como último argumento, além do canal e precisão
        adc2_get_raw(ADC2_CHANNEL_0, ADC_WIDTH_BIT_12, &val2);

        // Multiplica-se a leitura pelo quociente entre a tensão máxima dos pinos (3,3V) pelo número de bits (4096)
        printf("ADC1: %f ADC2: %f \n", val1*(3.3/4095), val2*(3.3/4095));
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
