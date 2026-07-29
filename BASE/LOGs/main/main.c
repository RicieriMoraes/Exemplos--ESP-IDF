/*
 * NOME: Ricieri Juan
 * DATA: 29/07/2026
 * PROJETO: LOGs
 * VERSÃO: 1.0.0
*/

#include <stdio.h>

// Biblioteca para tipos de logs definidos
#include "esp_log.h"

void app_main(void)
{
    int var = 0;

    // LOGs pré-definidos recebem TAG de identificação e mensagem
    // A configuração de exibição dos logs é realizada no menuconfig

    ESP_LOGE("LOG1", "LOG DE ERRO %d", var++); // LOG mais prioritário
    ESP_LOGW("LOG1", "LOG DE WARNING/AVISO %d", var++);
    ESP_LOGI("LOG1", "LOG DE INFORMAÇÃO %d", var++);
    ESP_LOGD("LOG1", "LOG DE DEBUG %d", var++);
    ESP_LOGV("LOG1", "LOG DE TEXTO COMUM/VERBOSE %d", var++); // LOG menos prioritário

    // Seta até qual nível devem ser exibidos logs de uma TAG específica
    esp_log_level_set("LOG2", ESP_LOG_WARN);

    ESP_LOGE("LOG2", "LOG DE ERRO");
    ESP_LOGW("LOG2", "LOG DE WARNING/AVISO");
    ESP_LOGI("LOG2", "LOG DE INFORMAÇÃO");
    ESP_LOGD("LOG2", "LOG DE DEBUG");
    ESP_LOGV("LOG2", "LOG DE TEXTO COMUM/VERBOSE");
}
