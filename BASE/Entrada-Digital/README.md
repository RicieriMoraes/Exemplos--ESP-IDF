# *Entrada Digital*

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
| ------ | ---------- | ------------ | ----------------- |
| 1.0.0  | 29/07/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a configuração e a utilização de uma entrada digital (GPIO) no ESP32 utilizando o framework ESP-IDF. Um botão é configurado como entrada digital e seu estado é lido continuamente. De acordo com a leitura realizada, um LED conectado a uma GPIO de saída é acionado ou desligado, permitindo compreender o funcionamento básico de entradas e saídas digitais.

---

## Objetivo

* Demonstrar a configuração de uma GPIO como entrada digital.
* Demonstrar a configuração de uma GPIO como saída digital.
* Realizar a leitura do estado lógico de um botão.
* Controlar um LED a partir da leitura de uma entrada digital.
* Utilizar as funções `gpio_get_level()` e `gpio_set_level()`.
* Servir como exemplo básico para aplicações que utilizam sensores digitais, botões e chaves no ESP32.

---

## Bibliotecas utilizadas

| Biblioteca            | Finalidade                                                                       |
| --------------------- | -------------------------------------------------------------------------------- |
| `stdio.h`             | Funções padrão de entrada e saída da linguagem C.                                |
| `freertos/FreeRTOS.h` | Definições principais do sistema operacional FreeRTOS.                           |
| `freertos/task.h`     | Gerenciamento de tarefas e temporizações (`vTaskDelay`).                         |
| `esp_system.h`        | Recursos gerais do sistema ESP32.                                                |
| `driver/gpio.h`       | Configuração, leitura e escrita das portas GPIO do ESP32.                        |

---

## Configuração do firmware

| Parâmetro            | Configuração        |
| -------------------- | ------------------- |
| Linguagem            | C                   |
| Framework            | ESP-IDF             |
| Sistema operacional  | FreeRTOS            |
| Função principal     | `app_main()`        |
| GPIO de saída        | GPIO10              |
| GPIO de entrada      | GPIO9               |
| Modo da saída        | `GPIO_MODE_OUTPUT`  |
| Modo da entrada      | `GPIO_MODE_INPUT`   |
| Tempo entre leituras | 10 ms               |
| Entrada              | Botão (Push Button) |
| Saída                | LED                 |

---

## Funcionamento

1. Configura a GPIO10 como saída digital para o LED.
2. Configura a GPIO9 como entrada digital para o botão.
3. Entra em um laço infinito de execução.
4. Realiza continuamente a leitura do estado lógico do botão utilizando `gpio_get_level()`.
5. Inverte o valor lido utilizando o operador lógico `!`, considerando que o botão é acionado em nível baixo (ativo em LOW).
6. Escreve o estado resultante na GPIO do LED através da função `gpio_set_level()`.
7. Aguarda 10 ms antes da próxima leitura utilizando `vTaskDelay()`, evitando o acionamento do Watchdog Timer e reduzindo o consumo de processamento.

---

## Pinos utilizados

| Nome  | GPIO   |
| ----- | ------ |
| LED   | GPIO10 |
| Botão | GPIO9  |

---

## Observações

* Neste projeto, o LED é acionado utilizando o valor **invertido** da leitura do botão (`!status`).
* Considera-se que o botão está configurado em lógica **ativa em nível baixo (Active LOW)**, configuração bastante utilizada em placas de desenvolvimento e no simulador Wokwi.
* O atraso de **10 ms** também auxilia na redução dos efeitos de bouncing do botão, embora não implemente um algoritmo completo de debounce.

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
Entrada-Digital/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```
