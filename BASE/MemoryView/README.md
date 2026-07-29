# *MemoryView*

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
| ------ | ---------- | ------------ | ----------------- |
| 1.0.0  | 29/07/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra como monitorar a utilização da memória do ESP32 utilizando o framework ESP-IDF. São realizadas consultas às principais regiões de memória disponíveis, incluindo Heap, DRAM, IRAM e o maior bloco contínuo livre (Largest Free Block), permitindo avaliar a disponibilidade de recursos para execução da aplicação.

---

## Objetivo

* Demonstrar a consulta da memória disponível no ESP32.
* Monitorar a quantidade de memória Heap livre.
* Verificar a disponibilidade de DRAM e IRAM.
* Determinar o maior bloco contínuo de memória disponível.
* Auxiliar na análise de consumo de memória durante o desenvolvimento.
* Servir como exemplo para diagnóstico e otimização de aplicações embarcadas.

---

## Bibliotecas utilizadas

| Biblioteca            | Finalidade                                                                      |
| --------------------- | ------------------------------------------------------------------------------- |
| `stdio.h`             | Funções padrão de entrada e saída da linguagem C.                               |
| `sdkconfig.h`         | Acesso às configurações geradas pelo ESP-IDF.                                   |
| `freertos/FreeRTOS.h` | Definições principais do sistema operacional FreeRTOS.                          |
| `freertos/task.h`     | Gerenciamento de tarefas e temporizações (`vTaskDelay`).                        |
| `esp_system.h`        | Recursos gerais do sistema ESP32.                                               |
| `esp_spi_flash.h`     | Interface com a memória Flash interna.                                          |
| `esp_heap_caps.h`     | Consulta e gerenciamento das diferentes regiões de memória (Heap, DRAM e IRAM). |

---

## Configuração do firmware

| Parâmetro           | Configuração               |
| ------------------- | -------------------------- |
| Linguagem           | C                          |
| Framework           | ESP-IDF                    |
| Sistema operacional | FreeRTOS                   |
| Função principal    | `app_main()`               |
| Função auxiliar     | `MemoryView()`             |
| Intervalo da tarefa | 1000 ms                    |
| Saída               | Terminal serial (`printf`) |
| Unidade de exibição | kB                         |

---

## Funcionamento

1. A função `app_main()` chama a função `MemoryView()`.
2. A função realiza consultas às diferentes regiões de memória disponíveis no ESP32.
3. Os valores retornados são convertidos de bytes para quilobytes (kB).
4. As informações são exibidas no terminal serial.
5. Após a exibição, a aplicação permanece em execução em um laço infinito, apenas mantendo a tarefa ativa através do FreeRTOS.

---

## Informações de memória monitoradas

| Informação             | Descrição                                                                                    |
| ---------------------- | -------------------------------------------------------------------------------------------- |
| **Heap**               | Quantidade total de memória Heap livre disponível para alocação dinâmica.                    |
| **DRAM**               | Quantidade de memória RAM destinada ao armazenamento de dados (Data RAM).                    |
| **IRAM**               | Quantidade de memória destinada ao armazenamento e execução de instruções (Instruction RAM). |
| **Maximum Free Block** | Maior bloco contínuo de memória livre disponível para uma nova alocação.                     |

---

## Funções utilizadas

| Função                               | Finalidade                                                                        |
| ------------------------------------ | --------------------------------------------------------------------------------- |
| `xPortGetFreeHeapSize()`             | Retorna a quantidade total de Heap livre.                                         |
| `heap_caps_get_free_size()`          | Retorna a quantidade de memória livre para uma determinada capacidade de memória. |
| `heap_caps_get_largest_free_block()` | Retorna o maior bloco contínuo disponível para alocação.                          |

---

## Capacidades de memória utilizadas

| Capacidade         | Descrição                                                                   |
| ------------------ | --------------------------------------------------------------------------- |
| `MALLOC_CAP_8BIT`  | Memória acessível em blocos de 8 bits (DRAM).                               |
| `MALLOC_CAP_32BIT` | Memória acessível em blocos de 32 bits (IRAM e outras regiões compatíveis). |

---

## Exemplo de saída

```text
Verificação de memória

Heap 314 kB
DRam 179 kB
IRam 126 kB
Maximum Free Block 108 kB
```

Os valores apresentados variam de acordo com:

* Modelo do microcontrolador.
* Versão do ESP-IDF.
* Quantidade de tarefas em execução.
* Bibliotecas utilizadas.
* Memória já alocada pela aplicação.

---

## Observações

* A memória **Heap** é utilizada para alocação dinâmica durante a execução do firmware.
* A **DRAM** é destinada principalmente ao armazenamento de variáveis e estruturas de dados.
* A **IRAM** é utilizada para armazenar instruções que necessitam de acesso rápido pelo processador.
* O **Largest Free Block** é um importante indicador de fragmentação da memória. Mesmo existindo bastante memória livre, um bloco contínuo pequeno pode impedir grandes alocações dinâmicas.
* O monitoramento dessas informações é bastante útil durante o desenvolvimento de aplicações que utilizam múltiplas tarefas, buffers, filas (Queues), semáforos e alocações dinâmicas.

---

## Informações

| Info        | Modelo             |
| ----------- | ------------------ |
| uC          | ESP32-C3           |
| Placa       | ESP32-C3 DevKitM-1 |
| Arquitetura | RISC-V (32 bits)   |
| IDE         | ESP-IDF v5.4.2     |

---

## Estrutura do projeto

```text
MemoryView/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```
