#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

// Biblioteca para acesso de funções heap
#include "esp_heap_caps.h"

void MemoryView();

void app_main(void)
{
    MemoryView();

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Função de verificação de memória
void MemoryView() 
{
    printf("Verificação de memória\n");

    // Armazena o retorno da função que verifica quanto de memória está disponível
    // dentro do bloco HEAP (responsável pelo carregamento de funções, tasks em tempo de execução)
    int Heap = (xPortGetFreeHeapSize())/1024;

    // Armazena o retorno do DataRam agrupado em blocos de 8 bits
    int DRam = (heap_caps_get_free_size(MALLOC_CAP_8BIT))/1024;

    // Armazena o retorno do IRam agrupado em blocos de 32 bits excluindo DataRam
    int IRam = (heap_caps_get_free_size(MALLOC_CAP_32BIT) - heap_caps_get_free_size(MALLOC_CAP_8BIT))/1024;

    // Armazena o retorno do máximo bloco contínuo disponível de 8 bits
    int freeBlock = (heap_caps_get_largest_free_block(MALLOC_CAP_8BIT))/1024;

    printf("Heap %d kB\n", Heap);
    printf("DRam %d kB\n", DRam);
    printf("IRam %d kB\n", IRam);
    printf("Maximum Free Block %d kB\n", freeBlock);
}
