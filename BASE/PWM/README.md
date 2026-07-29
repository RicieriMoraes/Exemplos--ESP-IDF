# *PWM*

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
| ------ | ---------- | ------------ | ----------------- |
| 1.0.0  | 29/07/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a geração de sinais PWM (Pulse Width Modulation) utilizando o periférico LEDC do ESP32 através do framework ESP-IDF. São configurados um timer e um canal PWM para gerar um sinal de 5 kHz com resolução de 13 bits, permitindo o controle do duty cycle aplicado ao pino de saída.

---

## Objetivo

* Demonstrar a configuração do periférico LEDC do ESP32.
* Configurar um timer para geração de sinais PWM.
* Configurar um canal PWM associado ao timer.
* Controlar o duty cycle utilizando as funções do ESP-IDF.
* Compreender o funcionamento básico da modulação por largura de pulso (PWM).
* Servir como exemplo para aplicações de controle de LEDs, motores, servomotores e conversores de potência.

---

## Bibliotecas utilizadas

| Biblioteca            | Finalidade                                                                         |
| --------------------- | ---------------------------------------------------------------------------------- |
| `stdio.h`             | Funções padrão de entrada e saída da linguagem C.                                  |
| `freertos/FreeRTOS.h` | Definições principais do sistema operacional FreeRTOS.                             |
| `freertos/task.h`     | Gerenciamento de tarefas e temporizações (`vTaskDelay`).                           |
| `driver/gpio.h`       | Configuração das GPIOs do ESP32.                                                   |
| `esp_system.h`        | Recursos gerais do sistema ESP32.                                                  |
| `driver/ledc.h`       | Configuração e controle do periférico LEDC responsável pela geração de sinais PWM. |

---

## Configuração do firmware

| Parâmetro           | Configuração          |
| ------------------- | --------------------- |
| Linguagem           | C                     |
| Framework           | ESP-IDF               |
| Sistema operacional | FreeRTOS              |
| Função principal    | `app_main()`          |
| Função auxiliar     | `pwm_init()`          |
| Periférico          | LEDC                  |
| Modo de velocidade  | `LEDC_LOW_SPEED_MODE` |
| Timer               | `LEDC_TIMER_0`        |
| Canal               | `LEDC_CHANNEL_0`      |
| Resolução           | 13 bits               |
| Frequência          | 5000 Hz               |
| Clock               | `LEDC_AUTO_CLK`       |
| GPIO de saída       | GPIO25                |
| Duty Cycle aplicado | 4095 (≈ 50%)          |

---

## Funcionamento

1. A função `pwm_init()` configura o timer responsável pela geração do sinal PWM.
2. Em seguida, configura o canal PWM associado ao timer.
3. O sinal PWM é direcionado para a GPIO25.
4. Durante a execução do programa, o duty cycle é configurado utilizando `ledc_set_duty()`.
5. A função `ledc_update_duty()` aplica efetivamente o novo duty cycle ao hardware.
6. A tarefa aguarda 10 ms antes da próxima atualização.

---

## Configuração do Timer PWM

| Parâmetro  | Valor                        |
| ---------- | ---------------------------- |
| Timer      | `LEDC_TIMER_0`               |
| Frequência | 5000 Hz                      |
| Resolução  | 13 bits                      |
| Clock      | Automático (`LEDC_AUTO_CLK`) |
| Speed Mode | Low Speed                    |

---

## Configuração do Canal PWM

| Parâmetro       | Valor            |
| --------------- | ---------------- |
| Canal           | `LEDC_CHANNEL_0` |
| GPIO            | GPIO25           |
| Timer associado | `LEDC_TIMER_0`   |
| Interrupção     | Desabilitada     |
| Duty inicial    | 0                |
| HPoint          | 0                |

---

## Pinos utilizados

| Nome      | GPIO   |
| --------- | ------ |
| Saída PWM | GPIO25 |

---

## Funções utilizadas

| Função                  | Finalidade                                              |
| ----------------------- | ------------------------------------------------------- |
| `ledc_timer_config()`   | Configura o timer responsável pela geração do PWM.      |
| `ledc_channel_config()` | Configura o canal PWM.                                  |
| `ledc_set_duty()`       | Define um novo duty cycle para o canal.                 |
| `ledc_update_duty()`    | Atualiza o hardware aplicando o duty cycle configurado. |
| `vTaskDelay()`          | Realiza a temporização da tarefa.                       |

---

## Conceitos importantes

### Frequência

É a quantidade de ciclos do sinal PWM gerados por segundo.

Neste projeto:

* **5000 Hz (5 kHz)**

---

### Resolução

Define a quantidade de níveis possíveis para o duty cycle.

Com resolução de **13 bits**:

```text
2¹³ = 8192 níveis
```

Assim, o duty cycle pode variar entre:

```text
0 até 8191
```

---

### Duty Cycle

Representa a porcentagem do tempo em que o sinal permanece em nível lógico alto.

Neste projeto é utilizado:

```text
Duty = 4095
```

O que corresponde aproximadamente a:

```text
50 % do período
```

---

## Observações

* O periférico **LEDC** é dedicado à geração de sinais PWM no ESP32.
* A combinação entre frequência e resolução possui limitações impostas pelo hardware. Frequências muito elevadas reduzem a resolução disponível.
* O sinal PWM pode ser utilizado em diversas aplicações, como:

  * Controle de brilho de LEDs;
  * Controle de velocidade de motores CC;
  * Acionamento de servomotores;
  * Conversores DC-DC;
  * Fontes chaveadas;
  * Controle de potência.

> **Observação sobre este exemplo:** embora exista um laço `for` incrementando a variável `duty`, o código aplica sempre o valor **4095** ao PWM. Dessa forma, o duty cycle permanece constante em aproximadamente 50%. Para gerar uma rampa de PWM, bastaria substituir `4095` pela variável `duty` na função `ledc_set_duty()`.

* Para melhor visualização do resultado: monitorar o GPIO25 com um sociloscópio.

---

## Informações

| Info        | Modelo             |
| ----------- | ------------------ |
| uC          | ESP32-D0WDQ6                   |
| Placa       | ESP32 DevKit V1 / ESP32 Module |
| Arquitetura | Xtensa LX6 / RISC de 32 bits   |
| IDE         | ESP-IDF v5.4.2     |

---

## Estrutura do projeto

```text
PWM/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```
