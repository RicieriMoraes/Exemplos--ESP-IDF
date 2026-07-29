# *DAC*

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
| ------ | ---------- | ------------ | ----------------- |
| 1.0.0  | 29/07/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a configuração e a utilização do conversor digital-analógico (DAC) interno do ESP32 utilizando o framework ESP-IDF.

O canal DAC1 é habilitado e recebe valores digitais crescentes entre 0 e 254. Esses valores são convertidos internamente em níveis analógicos de tensão, produzindo na saída uma forma de onda crescente do tipo rampa ou dente de serra.

---

## Objetivo

* Demonstrar a configuração do conversor digital-analógico interno do ESP32.
* Habilitar o canal DAC1.
* Converter valores digitais em níveis analógicos de tensão.
* Gerar uma tensão crescente na saída do DAC.
* Criar uma forma de onda do tipo rampa ou dente de serra.
* Utilizar o FreeRTOS para controlar o intervalo entre as atualizações.
* Servir como exemplo básico para aplicações que utilizam saídas analógicas.

---

## Bibliotecas utilizadas

| Biblioteca            | Finalidade                                                                                                 |
| --------------------- | ---------------------------------------------------------------------------------------------------------- |
| `stdio.h`             | Disponibiliza funções padrão de entrada e saída da linguagem C. Não é utilizada diretamente neste projeto. |
| `freertos/FreeRTOS.h` | Disponibiliza as definições principais do sistema operacional FreeRTOS.                                    |
| `freertos/task.h`     | Permite utilizar funções relacionadas a tarefas e temporização, como `vTaskDelay()`.                       |
| `driver/dac.h`        | Disponibiliza as funções de configuração e controle do DAC interno do ESP32.                               |

---

## Configuração do firmware

| Parâmetro                        | Configuração                     |
| -------------------------------- | -------------------------------- |
| Linguagem                        | C                                |
| Framework                        | ESP-IDF                          |
| Sistema operacional              | FreeRTOS                         |
| Função principal                 | `app_main()`                     |
| Conversor utilizado              | DAC interno                      |
| Canal utilizado                  | `DAC_CHANNEL_1`                  |
| Resolução                        | 8 bits                           |
| Faixa digital                    | 0 a 255                          |
| Valores utilizados no laço       | 0 a 254                          |
| Tensão de referência considerada | 3,3 V                            |
| Intervalo entre atualizações     | 10 ms                            |
| Tempo aproximado da rampa        | 2,55 s                           |
| Frequência aproximada da onda    | 0,39 Hz                          |
| Tipo de saída                    | Analógica                        |
| Forma de onda gerada             | Rampa crescente / dente de serra |

---

## Configuração do DAC

O canal DAC1 é habilitado por meio da função:

```c
dac_output_enable(DAC_CHANNEL_1);
```

Após ser habilitado, o canal pode receber valores digitais de 8 bits, variando entre 0 e 255.

A escrita do valor digital no DAC é realizada pela função:

```c
dac_output_voltage(DAC_CHANNEL_1, i);
```

O primeiro argumento informa o canal que será utilizado, enquanto o segundo argumento representa o valor digital enviado ao conversor.

---

## Funcionamento

1. O canal DAC1 é habilitado.
2. O programa entra em um laço de repetição infinito.
3. Um laço `for` incrementa a variável `i` de 0 até 254.
4. Cada valor de `i` é enviado ao DAC.
5. O DAC converte o valor digital em um nível analógico de tensão.
6. O programa aguarda 10 ms antes de enviar o próximo valor.
7. Ao atingir o valor 254, o laço é reiniciado em 0.
8. A saída retorna rapidamente ao nível mínimo, formando uma onda do tipo dente de serra.

---

## Conversão digital-analógica

O DAC interno do ESP32 possui resolução de 8 bits. Portanto, o valor digital pode variar entre 0 e 255.

A relação aproximada entre o valor digital e a tensão de saída é:

```text
Tensão = Vref × Valor digital / 255
```

Considerando uma tensão de referência de 3,3 V:

```text
Tensão = 3,3 × Valor digital / 255
```

Para calcular o valor digital necessário para determinada tensão:

```text
Valor digital = 255 × Tensão desejada / 3,3
```

Exemplos aproximados:

| Valor digital | Tensão calculada |
| ------------: | ---------------: |
|             0 |              0 V |
|            64 |           0,83 V |
|           128 |           1,66 V |
|           192 |           2,48 V |
|           254 |           3,29 V |
|           255 |           3,30 V |

Os valores apresentados são teóricos. A tensão real pode variar devido às características internas do ESP32 e à tensão de alimentação.

---

## Geração da forma de onda

O firmware utiliza o seguinte laço para produzir uma tensão crescente:

```c
for (int i = 0; i < 255; i++) {
    dac_output_voltage(DAC_CHANNEL_1, i);
    vTaskDelay(10 / portTICK_PERIOD_MS);
}
```

A cada 10 ms, o valor enviado ao DAC é incrementado em uma unidade.

Como são utilizados 255 níveis, o tempo aproximado para completar uma rampa é:

```text
Tempo da rampa = 255 × 10 ms
Tempo da rampa = 2550 ms
Tempo da rampa = 2,55 s
```

A frequência aproximada da onda é:

```text
Frequência = 1 / 2,55
Frequência ≈ 0,39 Hz
```

Quando o laço termina, a variável retorna para zero, fazendo a tensão cair do valor máximo para o valor mínimo e iniciando uma nova rampa.

---

## Pinos utilizados

| Nome | Canal           | Pino   |
| ---- | --------------- | ------ |
| DAC1 | `DAC_CHANNEL_1` | GPIO25 |

No ESP32 clássico:

* `DAC_CHANNEL_1` corresponde à GPIO25.
* `DAC_CHANNEL_2` corresponde à GPIO26.

---

## Forma de onda esperada

A saída apresenta uma tensão crescente em pequenos degraus, seguida de uma queda rápida para o nível mínimo:

```text
Tensão
 3,3 V |                 /|                 /|
       |                / |                / |
       |               /  |               /  |
       |              /   |              /   |
       |             /    |             /    |
   0 V |____________/     |____________/     |____ Tempo
```

Devido à resolução de 8 bits, a rampa é formada por 255 pequenos degraus de tensão.

---

## Temporização

A temporização entre cada atualização do DAC é realizada por:

```c
vTaskDelay(10 / portTICK_PERIOD_MS);
```

Essa instrução suspende a tarefa principal durante aproximadamente 10 ms.

Em versões mais recentes do FreeRTOS e do ESP-IDF, também pode ser utilizada a macro:

```c
vTaskDelay(pdMS_TO_TICKS(10));
```

Essa forma torna a conversão de milissegundos para ticks mais clara e independente da frequência de tick configurada.

---

## Observações

* Conectar um osciloscópio à GPIOX para visualizar o sinal gerado.

### Resolução do DAC

O DAC possui resolução de 8 bits, disponibilizando 256 níveis digitais possíveis, numerados de 0 a 255.

No código atual, a condição:

```c
i < 255
```

faz com que sejam utilizados somente os valores entre 0 e 254. Para incluir o valor máximo 255, o laço pode ser alterado para:

```c
for (int i = 0; i <= 255; i++) {
    dac_output_voltage(DAC_CHANNEL_1, i);
    vTaskDelay(pdMS_TO_TICKS(10));
}
```

### Precisão da tensão

A equação de conversão considera uma relação ideal entre o valor digital e a tensão de alimentação. Entretanto, o DAC interno do ESP32 pode apresentar:

* Erro de offset.
* Erro de ganho.
* Não linearidade.
* Ruído elétrico.
* Variações na tensão de alimentação.
* Limitações de corrente na saída.

Por esse motivo, a saída não deve ser utilizada diretamente para alimentar cargas de potência. Quando necessário, recomenda-se utilizar um amplificador operacional como buffer.

### Compatibilidade

O DAC interno não está disponível em todos os modelos da família ESP32.

Este código é compatível com o ESP32 clássico que possui os canais DAC nas GPIO25 e GPIO26. Microcontroladores como o ESP32-C3 não possuem DAC interno convencional e, portanto, não executam este exemplo sem adaptação.

Para placas sem DAC interno, uma saída analógica aproximada pode ser gerada por meio de PWM combinado com um filtro passa-baixas.

---

## Informações

| Info        | Modelo                         |
| ----------- | ------------------------------ |
| uC          | ESP32-D0WDQ6                   |
| Placa       | ESP32 DevKit V1 / ESP32 Module |
| Arquitetura | Xtensa LX6 / RISC de 32 bits   |
| Framework   | ESP-IDF                        |
| IDE         | ESP-IDF v5.4.2                 |
| Linguagem   | C                              |

---

## Estrutura do projeto

```text
DAC/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```
