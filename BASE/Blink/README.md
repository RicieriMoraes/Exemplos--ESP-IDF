# _Blink_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor         | Descrição          |
|--------|------------|---------------|--------------------|
| 1.0.0  | 29/07/2026 | Ricieri Juan  | Início do projeto  |

---

## Resumo

Este projeto demonstra a configuração e o controle de uma porta de entrada e saída de uso geral (GPIO) do ESP32 utilizando o ESP-IDF. Um pino digital é configurado como saída para controlar um LED, que tem seu estado alternado continuamente, produzindo o efeito de pisca-pisca (Blink). O projeto apresenta a utilização das funções de configuração de GPIO, escrita em pinos digitais e temporização com FreeRTOS.

---

## Objetivo

- Demonstrar a configuração de uma GPIO como saída digital.
- Controlar o acionamento de um LED utilizando o ESP32.
- Alternar periodicamente o estado lógico de um pino digital.
- Utilizar a função `gpio_set_level()` para escrita em GPIO.
- Utilizar o FreeRTOS para implementar temporização da aplicação.
- Servir como exemplo básico para aplicações que utilizam saídas digitais no ESP32.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Funções padrão de entrada e saída da linguagem C. |
| `freertos/FreeRTOS.h` | Definições principais do sistema operacional FreeRTOS. |
| `freertos/task.h` | Gerenciamento de tarefas e temporizações (`vTaskDelay`). |
| `esp_system.h` | Recursos gerais do sistema ESP32. |
| `driver/gpio.h` | Configuração e controle das portas GPIO do ESP32. |

---

## Configuração do firmware

| Parâmetro | Configuração |
|-----------|--------------|
| Linguagem | C |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Função principal | `app_main()` |
| GPIO utilizada | GPIO10 |
| Modo da GPIO | Saída digital (`GPIO_MODE_OUTPUT`) |
| Estado inicial | Baixo (LOW) |
| Período de comutação | 1000 ms |
| Frequência do LED | 0,5 Hz (1 segundo ligado e 1 segundo desligado) |
| Saída | LED conectado à GPIO10 |

---

## Funcionamento

1. Configura a GPIO10 como saída digital.
2. Inicializa uma variável booleana responsável por armazenar o estado do LED.
3. Entra em um laço infinito (`while`).
4. Inverte o estado da variável utilizando o operador lógico `!`.
5. Escreve o novo estado na GPIO através da função `gpio_set_level()`.
6. Aguarda 1000 ms utilizando `vTaskDelay()`.
7. Repete continuamente o processo, fazendo o LED piscar.

---

## Pinos utilizados

| Nome | GPIO |
|------|------|
| LED  | GPIO10 |

---

## Informações

| Info | Modelo |
|------|--------|
| uC | ESP32-C3 |
| Placa | ESP32-C3 DevKitM-1 |
| Arquitetura | RISC-V (32 bits) |
| IDE | ESP-IDF v5.4.2 |

---

## Estrutura do projeto

```text
Blink/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```