# *Delays*

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
| ------ | ---------- | ------------ | ----------------- |
| 1.0.0  | 29/07/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a geração de pulsos digitais em uma porta GPIO do ESP32 utilizando o framework ESP-IDF.

A GPIO10 é configurada como saída digital e acionada em nível lógico alto durante aproximadamente 1.000 microssegundos. Em seguida, o pino retorna ao nível lógico baixo e permanece nesse estado durante aproximadamente 10 milissegundos antes da geração de um novo pulso.

O firmware utiliza duas formas diferentes de temporização: `esp_rom_delay_us()` para produzir um pulso curto com duração em microssegundos e `vTaskDelay()` para controlar o intervalo entre os pulsos por meio do FreeRTOS.

---

## Objetivo

* Demonstrar a configuração de uma GPIO como saída digital.
* Gerar pulsos digitais periódicos utilizando o ESP32.
* Utilizar temporização em microssegundos para definir a largura do pulso.
* Comparar a utilização de `esp_rom_delay_us()` e `vTaskDelay()`.
* Demonstrar a diferença entre uma espera bloqueante e uma temporização baseada no FreeRTOS.
* Produzir um pulso de aproximadamente 1 ms na GPIO10.
* Controlar o intervalo entre os pulsos.
* Servir como exemplo básico para aplicações que exigem sinais digitais temporizados.

---

## Bibliotecas utilizadas

| Biblioteca            | Finalidade                                                                                                 |
| --------------------- | ---------------------------------------------------------------------------------------------------------- |
| `stdio.h`             | Disponibiliza funções padrão de entrada e saída da linguagem C. Não é utilizada diretamente neste projeto. |
| `freertos/FreeRTOS.h` | Disponibiliza as definições principais do sistema operacional FreeRTOS.                                    |
| `freertos/task.h`     | Permite utilizar funções de controle de tarefas e temporização, como `vTaskDelay()`.                       |
| `driver/gpio.h`       | Disponibiliza as funções necessárias para configurar e controlar as portas GPIO.                           |

Para utilizar diretamente a função `esp_rom_delay_us()`, recomenda-se também incluir:

```c
#include "esp_rom_sys.h"
```

---

## Configuração do firmware

| Parâmetro                     | Configuração         |
| ----------------------------- | -------------------- |
| Linguagem                     | C                    |
| Framework                     | ESP-IDF              |
| Sistema operacional           | FreeRTOS             |
| Função principal              | `app_main()`         |
| GPIO utilizada                | GPIO10               |
| Modo da GPIO                  | Saída digital        |
| Nível ativo                   | Alto (`HIGH`)        |
| Largura aproximada do pulso   | 1.000 µs             |
| Largura aproximada do pulso   | 1 ms                 |
| Intervalo em nível baixo      | 10 ms                |
| Período teórico aproximado    | 11 ms                |
| Frequência teórica aproximada | 90,9 Hz              |
| Duty cycle teórico aproximado | 9,1%                 |
| Temporização de alta precisão | `esp_rom_delay_us()` |
| Temporização da tarefa        | `vTaskDelay()`       |

---

## Configuração da GPIO

O pino utilizado pelo projeto é definido por meio da diretiva:

```c
#define PIN 10
```

A GPIO10 é configurada como saída digital utilizando:

```c
gpio_set_direction(PIN, GPIO_MODE_OUTPUT);
```

Depois dessa configuração, o pino pode assumir os seguintes estados:

| Valor | Estado lógico | Tensão aproximada |
| ----: | ------------- | ----------------- |
|   `0` | Baixo (`LOW`) | 0 V               |
|   `1` | Alto (`HIGH`) | 3,3 V             |

---

## Funcionamento

1. A GPIO10 é configurada como saída digital.
2. O programa entra em um laço de repetição infinito.
3. A saída é inicialmente colocada em nível lógico baixo.
4. A saída é alterada imediatamente para nível lógico alto.
5. A função `esp_rom_delay_us(1000)` mantém a execução bloqueada durante aproximadamente 1.000 µs.
6. A GPIO retorna ao nível lógico baixo.
7. A função `vTaskDelay()` suspende a tarefa principal durante aproximadamente 10 ms.
8. Após o intervalo, um novo pulso é gerado.
9. O processo é repetido continuamente.

---

## Geração do pulso

O pulso é gerado pelo seguinte trecho:

```c
gpio_set_level(PIN, 1);

esp_rom_delay_us(1000);

gpio_set_level(PIN, 0);
```

Ao executar `gpio_set_level(PIN, 1)`, a GPIO10 assume nível lógico alto.

A instrução:

```c
esp_rom_delay_us(1000);
```

mantém o processador em espera durante aproximadamente 1.000 microssegundos, correspondentes a:

```text
1.000 µs = 1 ms
```

Depois desse período, a GPIO retorna ao nível lógico baixo:

```c
gpio_set_level(PIN, 0);
```

---

## Temporização

O firmware utiliza dois métodos de temporização.

### Temporização em microssegundos

```c
esp_rom_delay_us(1000);
```

Essa função realiza uma espera bloqueante em microssegundos.

Durante sua execução, o núcleo responsável pela tarefa permanece ocupado aguardando o término do tempo configurado.

Ela é adequada para:

* Pulsos curtos.
* Pequenos intervalos em microssegundos.
* Protocolos simples que exigem temporização precisa.
* Sinais digitais de curta duração.

O valor utilizado no firmware corresponde a:

```text
1.000 microssegundos = 1 milissegundo
```

### Temporização com FreeRTOS

```c
vTaskDelay(10 / portTICK_PERIOD_MS);
```

Essa função suspende a tarefa durante aproximadamente 10 ms e permite que o FreeRTOS execute outras tarefas enquanto aguarda.

Uma forma mais clara de escrever a mesma temporização é:

```c
vTaskDelay(pdMS_TO_TICKS(10));
```

O tempo real da suspensão depende da frequência de tick configurada no FreeRTOS.

---

## Forma de onda esperada

A GPIO permanece em nível alto durante aproximadamente 1 ms e em nível baixo durante aproximadamente 10 ms.

```text
Tensão

3,3 V       ┌───┐          ┌───┐          ┌───┐
            │   │          │   │          │   │
  0 V ──────┘   └──────────┘   └──────────┘   └───────── Tempo
              1 ms   10 ms    1 ms   10 ms
```

A forma de onda apresenta pulsos positivos periódicos.

---

## Cálculo do período

O período aproximado é formado pela soma do tempo em nível alto e do tempo em nível baixo:

```text
Período = Tempo alto + Tempo baixo
```

```text
Período = 1 ms + 10 ms
```

```text
Período ≈ 11 ms
```

Convertendo para segundos:

```text
11 ms = 0,011 s
```

---

## Cálculo da frequência

A frequência teórica pode ser calculada por:

```text
Frequência = 1 / Período
```

```text
Frequência = 1 / 0,011
```

```text
Frequência ≈ 90,9 Hz
```

A frequência real pode apresentar pequenas diferenças devido ao tempo de execução das instruções, ao escalonamento do FreeRTOS e à resolução de tick configurada.

---

## Cálculo do duty cycle

O duty cycle representa o percentual do período em que o sinal permanece em nível lógico alto:

```text
Duty cycle = Tempo em nível alto / Período × 100
```

```text
Duty cycle = 1 ms / 11 ms × 100
```

```text
Duty cycle ≈ 9,1%
```

Assim, teoricamente, o sinal permanece aproximadamente 9,1% do tempo em nível alto e 90,9% em nível baixo.

---

## Pinos utilizados

| Nome          | GPIO   | Função        |
| ------------- | ------ | ------------- |
| Pulso digital | GPIO10 | Saída digital |

A GPIO10 deve ser conectada a um osciloscópio, analisador lógico ou outro circuito compatível com sinais de 3,3 V para observação dos pulsos.

---

## Medição do sinal

Para verificar o funcionamento do projeto, recomenda-se utilizar:

* Osciloscópio.
* Analisador lógico.
* Osciloscópio virtual do Wokwi.
* Instrumento de medição compatível com sinais digitais.
* Outra entrada digital de um microcontrolador.

A medição deverá apresentar aproximadamente:

| Parâmetro            | Valor esperado |
| -------------------- | -------------- |
| Tensão mínima        | 0 V            |
| Tensão máxima        | 3,3 V          |
| Tempo em nível alto  | 1 ms           |
| Tempo em nível baixo | 10 ms          |
| Período              | 11 ms          |
| Frequência           | 90,9 Hz        |
| Duty cycle           | 9,1%           |

---

## Observações

* Conectar um osciloscópio à GPIO10 para visualizar o sinal gerado.

### Precisão da temporização

A função `esp_rom_delay_us()` fornece uma espera em microssegundos, porém o tempo efetivo do pulso também inclui o tempo de execução das instruções utilizadas para alterar o estado da GPIO.

Por isso, a largura real do pulso pode ser ligeiramente diferente de 1.000 µs.

Para aplicações que exigem maior precisão, alta frequência ou geração contínua de pulsos, recomenda-se utilizar periféricos de hardware, como:

* Timer.
* GPTimer.
* PWM por LEDC.
* MCPWM.
* RMT.

Esses periféricos geram sinais com menor dependência da execução do processador.

### Espera bloqueante

Durante a execução de:

```c
esp_rom_delay_us(1000);
```

o processador permanece ocupado em uma espera ativa. Em intervalos muito longos, esse método pode prejudicar a execução de outras tarefas.

Por isso, a função deve ser utilizada principalmente para atrasos pequenos.

### Utilização do `vTaskDelay()`

O `vTaskDelay()` libera o processador para que outras tarefas sejam executadas durante o período de espera.

Além disso, essa função ajuda a evitar que a tarefa principal permaneça executando continuamente sem permitir o funcionamento adequado do escalonador, das tarefas internas do sistema e do watchdog.

### Estado inicial do pino

A instrução:

```c
gpio_set_level(PIN, 0);
```

executada antes da ativação do pulso garante que a saída seja colocada em nível lógico baixo antes de assumir o nível alto.

Entretanto, como não existe atraso entre os dois primeiros comandos:

```c
gpio_set_level(PIN, 0);
gpio_set_level(PIN, 1);
```

o período inicial em nível baixo é extremamente curto e não representa um pulso perceptível.

### Compatibilidade elétrica

As GPIOs do ESP32 operam normalmente com níveis lógicos de 3,3 V.

Não se deve aplicar diretamente uma tensão de 5 V à GPIO, pois isso pode danificar o microcontrolador.

---

## Informações

| Info        | Modelo             |
| ----------- | ------------------ |
| uC          | ESP32-C3           |
| Placa       | ESP32-C3 DevKitM-1 |
| Arquitetura | RISC-V de 32 bits  |
| Framework   | ESP-IDF            |
| IDE         | ESP-IDF v5.4.2     |
| Linguagem   | C                  |

---

## Estrutura do projeto

```text
Delays/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```
