/*
 * NOME: Ricieri Juan
 * DATA: 01/09/2026
 * PROJETO: Custom-Config
 * VERSÃO: 1.0.0
*/

#include <stdio.h>
#include "esp_log.h"
#include <stdbool.h> // Biblioteca para utilização de variável booleana

void app_main(void)
{
    // Log utilizando a TAG definida na configuração específica
    ESP_LOGI(CONFIG_ADS1115_TAG, "SDA PIN: %d", CONFIG_ADS1115_SDA);
    ESP_LOGI(CONFIG_ADS1115_TAG, "SCL PIN: %d", CONFIG_ADS1115_SCL);

    bool ADS1115_ENABLE = 0;

    // Verifica e força define de configuração específica
    #ifdef CONFIG_ADS1115_ENABLE
        ADS1115_ENABLE = 1;
    #else
        ADS1115_ENABLE = 0;
    #endif

    ESP_LOGI(CONFIG_ADS1115_TAG, "ADS 1115 STATUS: %s", ADS1115_ENABLE ? "yes" : "no");

    int OP_SAMPLER = 0;

    #ifdef CONFIG_OP_1
        OP_SAMPLER = 1;
    #elif CONFIG_OP_2
        OP_SAMPLER = 2;
    #elif CONFIG_OP_3
        OP_SAMPLER = 3;
    #else CONFIG_OP_4
        OP_SAMPLER = 4;
    #endif

    ESP_LOGI(CONFIG_ADS1115_TAG, "SAMPLE RATE OPTION %d", OP_SAMPLER);

}
