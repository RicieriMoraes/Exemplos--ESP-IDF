/*
 * NOME: Ricieri Juan
 * DATA: 29/07/2026
 * PROJETO: PWM
 * VERSÃO: 1.0.0
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"

// Biblioteca LEDC para PWM
#include "driver/ledc.h"

// Função de inicialização e configuração do PWM
void pwm_init(void) {
    
    // Estrutura de configuração do timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE, // Modo de velocidade
        .timer_num = LEDC_TIMER_0, // Número do timer
        .duty_resolution = LEDC_TIMER_13_BIT, // Resolução duty cycle
        .freq_hz = 5000, // Frequência
        .clk_cfg = LEDC_AUTO_CLK // Configuração de clock
    };
    ledc_timer_config(&ledc_timer); // Função que contém estrutura

    // Estrutura de configuração do canal
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE, // Modo de velocidade
        .channel = LEDC_CHANNEL_0, // Número do canal
        .timer_sel = LEDC_TIMER_0, // Número do timer
        .intr_type = LEDC_INTR_DISABLE, // Tipo de interrupção (está desligada)
        .gpio_num = 25, // Pino utilizado para PWM
        .duty = 0, // Duty cycle inicial
        .hpoint = 0 // 
    };
    ledc_channel_config(&ledc_channel); // Função que contém estrutura
}

void app_main(void)
{
    // Chamada da função de configuração do PWM
    pwm_init();

    for (int duty = 0; duty < 8192; duty++) {

        // Seta/escreve o modo de velocidade, canal e duty cycle (máx = 2^13) - enviando para o barramento
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 4095);

        // Atualiza o duty cycle com modo de velocidade e canal - aplicando no timer
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        // Necessário para "atualização" do loop de informações na task
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}