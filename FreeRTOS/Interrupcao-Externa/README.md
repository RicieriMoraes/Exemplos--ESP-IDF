# _Interrupção Externa_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
|--------|------------|--------------|-------------------|
| 1.0.0  | 10/08/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a utilização de uma **interrupção externa por GPIO** no ESP32 utilizando o framework ESP-IDF e o FreeRTOS.

Um botão conectado à GPIO9 é configurado para gerar uma interrupção na borda de subida do sinal. Quando o evento ocorre, a rotina de interrupção envia o número do pino para uma Queue utilizando `xQueueSendFromISR()`. Uma Task permanece bloqueada aguardando os eventos da fila e incrementa um contador a cada interrupção recebida.

O projeto apresenta uma abordagem adequada para tratamento de eventos assíncronos, mantendo a rotina de interrupção curta e delegando o processamento para uma Task.

---

## Objetivo

- Demonstrar a configuração de uma interrupção externa em uma GPIO.
- Detectar eventos de borda de subida.
- Criar e registrar uma rotina de serviço de interrupção (ISR).
- Utilizar `IRAM_ATTR` em uma função de interrupção.
- Enviar informações de uma ISR para uma Task através de uma Queue.
- Utilizar `xQueueSendFromISR()` para comunicação a partir de interrupções.
- Manter o processamento principal fora da rotina de interrupção.
- Contabilizar eventos externos recebidos pelo microcontrolador.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Disponibiliza funções padrão de entrada e saída, como `printf()`. |
| `freertos/FreeRTOS.h` | Contém as definições principais do sistema operacional FreeRTOS. |
| `freertos/task.h` | Criação e gerenciamento de Tasks. |
| `freertos/queue.h` | Criação e gerenciamento de Queues, incluindo operações realizadas a partir de ISR. |
| `driver/gpio.h` | Configuração das GPIOs e das interrupções externas do ESP32. |

---

## Configuração do firmware

| Parâmetro | Configuração |
|-----------|--------------|
| Linguagem | C |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Função principal | `app_main()` |
| GPIO de entrada | GPIO9 |
| Modo da GPIO | Entrada digital |
| Tipo de interrupção | Borda de subida |
| Configuração Pull-Up | Desabilitado |
| Configuração Pull-Down | Desabilitado |
| Comunicação ISR → Task | Queue |
| Capacidade da Queue | 10 elementos |
| Tipo dos elementos | `int` |
| Stack da Task | 4096 bytes |
| Prioridade da Task | 1 |
| Tempo de espera da Task | Indefinido (`portMAX_DELAY`) |

---

## Pinos utilizados

| Nome | GPIO |
|------|------|
| Botão / Entrada de interrupção | GPIO9 |

---

## Funcionamento

1. A GPIO9 é configurada como entrada digital.
2. Os resistores internos de Pull-Up e Pull-Down são desabilitados.
3. A interrupção é configurada para ocorrer na borda de subida do sinal.
4. Uma Queue com capacidade para dez elementos inteiros é criada.
5. A Task `signalTriggered()` é criada.
6. O serviço global de interrupções das GPIOs é instalado.
7. A função `gpio4_isr_handler()` é associada à GPIO9.
8. Quando ocorre uma borda de subida:
   - A ISR é executada;
   - O número do pino é enviado para a Queue.
9. A Task `signalTriggered()` é desbloqueada ao receber o elemento.
10. O contador global é incrementado.
11. O número de eventos detectados é exibido no terminal serial.

---

## Configuração da GPIO

A GPIO utilizada como entrada é definida por:

```c
#define PIN 9
```

Sua direção é configurada através de:

```c
gpio_set_direction(PIN, GPIO_MODE_INPUT);
```

Os resistores internos são desabilitados:

```c
gpio_pulldown_dis(PIN);
gpio_pullup_dis(PIN);
```

---

## Configuração da interrupção

A interrupção é configurada através de:

```c
gpio_set_intr_type(PIN, GPIO_INTR_POSEDGE);
```

Neste projeto é utilizado:

```text
GPIO_INTR_POSEDGE
```

Isso significa que a interrupção ocorre durante uma **borda de subida**:

```text
LOW ───────┐
           │
           └──── HIGH
           ↑
      Interrupção
```

---

## Tipos de interrupção GPIO

O ESP-IDF disponibiliza diferentes modos de disparo:

| Configuração | Descrição |
|--------------|-----------|
| `GPIO_INTR_DISABLE` | Interrupção desabilitada |
| `GPIO_INTR_POSEDGE` | Borda de subida |
| `GPIO_INTR_NEGEDGE` | Borda de descida |
| `GPIO_INTR_ANYEDGE` | Qualquer alteração de borda |
| `GPIO_INTR_LOW_LEVEL` | Nível lógico baixo |
| `GPIO_INTR_HIGH_LEVEL` | Nível lógico alto |

---

## Instalação do serviço de interrupção

Antes de registrar uma função ISR para uma GPIO, o serviço deve ser instalado:

```c
gpio_install_isr_service(0);
```

Depois, o handler é associado ao pino:

```c
gpio_isr_handler_add(
    PIN,
    gpio4_isr_handler,
    (void *)PIN
);
```

Os parâmetros utilizados são:

| Parâmetro | Finalidade |
|-----------|------------|
| `PIN` | GPIO que gerará a interrupção |
| `gpio4_isr_handler` | Função executada quando o evento ocorrer |
| `(void *)PIN` | Argumento enviado para a ISR |

---

## Rotina de interrupção

A rotina de interrupção é definida como:

```c
static void IRAM_ATTR gpio4_isr_handler(void *params)
```

Ela é executada quando ocorre o evento configurado na GPIO.

Dentro da função:

```c
int pinNumber = (int)params;
```

o argumento recebido é convertido novamente para o número do pino.

Depois, o valor é enviado à Queue:

```c
xQueueSendFromISR(
    interruptQueue,
    &pinNumber,
    NULL
);
```

---

## IRAM_ATTR

A declaração:

```c
IRAM_ATTR
```

indica que a função deve ser posicionada em memória de instruções apropriada para execução durante interrupções.

Isso é importante porque uma ISR deve possuir execução rápida e previsível, evitando dependências desnecessárias de recursos que possam estar indisponíveis durante determinadas condições do sistema.

---

## Queue de interrupção

A fila é criada através de:

```c
interruptQueue = xQueueCreate(
    10,
    sizeof(int)
);
```

Sua configuração é:

| Parâmetro | Valor |
|-----------|------:|
| Número de posições | 10 |
| Tamanho de cada elemento | `sizeof(int)` |

A Queue permite desacoplar a ISR da Task que realizará o processamento.

---

## Comunicação ISR → Task

A interrupção não realiza diretamente:

```c
printf()
```

nem processamento extenso.

Ela apenas envia um evento para a Queue:

```c
xQueueSendFromISR(
    interruptQueue,
    &pinNumber,
    NULL
);
```

A Task responsável pelo processamento executa:

```c
xQueueReceive(
    interruptQueue,
    &pinNumber,
    portMAX_DELAY
);
```

Essa arquitetura é recomendada porque mantém a rotina de interrupção curta.

---

## Task de processamento

A Task `signalTriggered()` permanece aguardando um elemento da fila:

```c
if (xQueueReceive(
    interruptQueue,
    &pinNumber,
    portMAX_DELAY
)) {
    counter++;

    printf("Contador: %d\n", counter);
}
```

Como foi utilizado:

```c
portMAX_DELAY
```

a Task permanece bloqueada enquanto não houver nenhum evento.

---

## Funções utilizadas

| Função | Finalidade |
|--------|------------|
| `gpio_set_direction()` | Configura a direção da GPIO. |
| `gpio_pulldown_dis()` | Desabilita o resistor Pull-Down interno. |
| `gpio_pullup_dis()` | Desabilita o resistor Pull-Up interno. |
| `gpio_set_intr_type()` | Define o tipo de evento que gera a interrupção. |
| `gpio_install_isr_service()` | Instala o serviço de interrupção das GPIOs. |
| `gpio_isr_handler_add()` | Associa uma função ISR a uma GPIO. |
| `xQueueCreate()` | Cria uma Queue. |
| `xQueueSendFromISR()` | Envia um elemento para uma Queue a partir de uma ISR. |
| `xQueueReceive()` | Recebe um elemento da Queue. |
| `xTaskCreate()` | Cria uma Task. |
| `printf()` | Exibe informações no terminal serial. |

---

## Fluxo de execução

```text
        Botão / Sinal externo
                 │
                 ▼
           Borda de subida
                 │
                 ▼
       Interrupção da GPIO9
                 │
                 ▼
       gpio4_isr_handler()
                 │
                 ▼
      xQueueSendFromISR()
                 │
                 ▼
        ┌────────────────┐
        │ interruptQueue │
        └────────────────┘
                 │
                 ▼
       xQueueReceive()
                 │
                 ▼
      signalTriggered()
                 │
                 ▼
        Incrementa contador
                 │
                 ▼
      Exibe valor no serial
```

---

## Exemplo de saída

A cada evento detectado:

```text
Contador: 1
Contador: 2
Contador: 3
Contador: 4
Contador: 5
```

Cada incremento corresponde a uma interrupção recebida pela GPIO.

---

## Conceitos importantes

### Interrupção

Uma interrupção permite que o microcontrolador responda rapidamente a um evento sem precisar consultar continuamente o estado de uma entrada.

Em vez de executar:

```text
Verificar botão
Verificar botão
Verificar botão
Verificar botão
...
```

o processador pode executar outras tarefas e ser interrompido apenas quando o evento ocorrer.

---

### ISR

ISR significa:

```text
Interrupt Service Routine
```

É a função executada automaticamente quando uma interrupção ocorre.

Uma ISR deve ser:

- Curta;
- Rápida;
- Não bloqueante;
- Com o mínimo possível de processamento.

---

### ISR × Task

O projeto separa o processamento em duas partes:

```text
ISR
 ↓
Detecta o evento
 ↓
Envia informação

Task
 ↓
Recebe informação
 ↓
Executa processamento
```

Essa estrutura reduz o tempo de execução da interrupção e melhora a previsibilidade do sistema.

---

## Interrupção × Polling

| Característica | Interrupção | Polling |
|---------------|-------------|---------|
| Verificação contínua da GPIO | Não | Sim |
| Uso de CPU | Menor | Maior |
| Resposta rápida a eventos | ✔ | Depende do loop |
| Complexidade | Maior | Menor |
| Ideal para eventos assíncronos | ✔ | Limitado |

---

## Aplicações práticas

Interrupções externas podem ser utilizadas em:

- Botões;
- Encoders;
- Sensores digitais;
- Detectores de presença;
- Entradas industriais;
- Alarmes;
- Contadores de pulsos;
- Medidores de frequência;
- Sensores de velocidade;
- Zero-cross detector;
- Sinais de outros microcontroladores;
- Eventos de hardware externo.

---

## Observações

- A rotina ISR deve executar o mínimo possível de processamento.
- Evite utilizar `printf()` diretamente dentro de uma ISR.
- Para comunicação a partir de uma ISR, devem ser utilizadas as versões específicas das APIs do FreeRTOS terminadas em `FromISR`.
- O uso de uma Queue permite armazenar múltiplos eventos caso eles ocorram antes da Task conseguir processá-los.
- Neste projeto, a Queue possui capacidade para dez eventos.
- Caso ocorram mais interrupções do que a Queue consegue armazenar, novos eventos podem ser perdidos.
- O contador global é incrementado apenas pela Task, evitando alteração direta dentro da interrupção.

---

## Debounce do botão

Botões mecânicos podem gerar múltiplas transições elétricas durante um único acionamento devido ao fenômeno de **bounce**.

Assim, um único pressionamento pode produzir:

```text
1 → 2 → 3 → 4 interrupções
```

em vez de apenas uma.

Este exemplo não implementa debounce.

Em aplicações reais, podem ser utilizados:

- Debounce por software;
- Timer;
- Delay controlado fora da ISR;
- Filtro RC;
- Circuito Schmitt Trigger.

---

### Verificação da criação da Queue

Pode-se verificar se a fila foi criada corretamente:

```c
interruptQueue = xQueueCreate(
    10,
    sizeof(int)
);

if (interruptQueue == NULL) {
    printf("Erro ao criar fila de interrupção\n");
    return;
}
```

---

### Verificação da instalação da ISR

Também pode ser verificado o retorno de:

```c
gpio_install_isr_service(0);
```

e:

```c
gpio_isr_handler_add(...);
```

para detectar possíveis falhas de configuração.

---

### Desbloqueio imediato de Task

Em uma ISR mais completa, pode-se utilizar uma variável do tipo:

```c
BaseType_t taskWoken = pdFALSE;
```

Exemplo:

```c
xQueueSendFromISR(
    interruptQueue,
    &pinNumber,
    &taskWoken
);

if (taskWoken) {
    portYIELD_FROM_ISR();
}
```

Isso permite que uma Task de maior prioridade seja executada imediatamente após o evento, caso necessário.

---

## Informações

| Info | Modelo |
|------|--------|
| Família | ESP32 |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Linguagem | C |
| IDE | ESP-IDF v5.4.2 |
| Entrada | GPIO9 |
| Interrupção | Borda de subida |
| Comunicação | Queue |
| Arquitetura | ISR → Queue → Task |

---

## Estrutura do projeto

```text
Interrupcao-Externa/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```