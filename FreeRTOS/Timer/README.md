# _Timer_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Autor | Descrição |
|--------|-------|-----------|
| 1.0.0 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a utilização de um **Software Timer do FreeRTOS** no ESP-IDF.

O firmware cria um timer através da função `xTimerCreate()`, configurado para realizar um único disparo após **500 ms**. Quando o período programado é atingido, o FreeRTOS executa automaticamente uma função de callback denominada `disparo()`.

Além disso, a função `esp_timer_get_time()` é utilizada para consultar o tempo decorrido desde a inicialização do sistema, permitindo visualizar aproximadamente em qual instante o timer foi iniciado e quando seu callback foi executado.

O exemplo apresenta conceitos fundamentais de temporização não bloqueante, callbacks e execução de eventos temporizados utilizando o FreeRTOS.

---

## Objetivo

- Demonstrar a criação de Software Timers no FreeRTOS.
- Utilizar `xTimerCreate()` para configurar um timer.
- Utilizar `xTimerStart()` para iniciar sua contagem.
- Criar uma função de callback associada ao timer.
- Configurar um timer de disparo único.
- Utilizar `pdMS_TO_TICKS()` para conversão de milissegundos.
- Consultar o tempo de execução através de `esp_timer_get_time()`.
- Demonstrar temporização sem bloquear a execução de `app_main()`.
- Diferenciar Software Timers do FreeRTOS e o `esp_timer` do ESP-IDF.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Disponibiliza funções padrão como `printf()`. |
| `freertos/FreeRTOS.h` | Contém definições principais do FreeRTOS. |
| `freertos/timers.h` | Disponibiliza os Software Timers do FreeRTOS, incluindo `TimerHandle_t`, `xTimerCreate()` e `xTimerStart()`. |
| `esp_timer.h` | Disponibiliza funções de temporização do ESP-IDF, como `esp_timer_get_time()`. |

---

## Configuração do firmware

| Parâmetro | Configuração |
|-----------|--------------|
| Linguagem | C |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Tipo de timer | Software Timer |
| Período | 500 ms |
| Modo | One-shot |
| Auto-reload | Desabilitado |
| Callback | `disparo()` |
| Tempo de espera para iniciar | 0 ticks |
| Medição de tempo | `esp_timer_get_time()` |

---

# Funcionamento

Ao iniciar o firmware, a função:

```c
app_main()
```

consulta inicialmente o tempo de execução do sistema:

```c
esp_timer_get_time()
```

Em seguida, um Software Timer é criado:

```c
TimerHandle_t xTimer = xTimerCreate(
    "timer disparo",
    pdMS_TO_TICKS(500),
    pdFALSE,
    NULL,
    disparo
);
```

O timer é então iniciado através de:

```c
xTimerStart(xTimer, 0);
```

Após aproximadamente **500 ms**, o FreeRTOS executa automaticamente:

```c
disparo()
```

A função consulta novamente o tempo do sistema e apresenta o instante do disparo no terminal.

---

# Software Timer

Um **Software Timer** é um recurso do FreeRTOS utilizado para executar uma função após determinado intervalo de tempo.

Diferentemente de um delay tradicional, não é necessário manter uma Task bloqueada apenas esperando o tempo transcorrer.

Estrutura conceitual:

```text
Criação do Timer
       │
       ▼
xTimerCreate()
       │
       ▼
Timer configurado para 500 ms
       │
       ▼
xTimerStart()
       │
       ▼
Contagem realizada pelo FreeRTOS
       │
       │ 500 ms
       ▼
Timer expira
       │
       ▼
disparo()
```

---

# Criação do Timer

O timer é criado através de:

```c
TimerHandle_t xTimer = xTimerCreate(
    "timer disparo",
    pdMS_TO_TICKS(500),
    pdFALSE,
    NULL,
    disparo
);
```

A estrutura geral da função é:

```c
xTimerCreate(
    pcTimerName,
    xTimerPeriod,
    uxAutoReload,
    pvTimerID,
    pxCallbackFunction
);
```

---

## Parâmetros de `xTimerCreate()`

| Parâmetro | Valor utilizado | Função |
|-----------|-----------------|--------|
| Nome | `"timer disparo"` | Identifica o timer |
| Período | `pdMS_TO_TICKS(500)` | Define período de 500 ms |
| Auto Reload | `pdFALSE` | Timer executa apenas uma vez |
| ID | `NULL` | Nenhum identificador adicional |
| Callback | `disparo` | Função executada quando o timer expira |

---

# Conversão de tempo

O FreeRTOS trabalha internamente utilizando **ticks**.

Por isso, o período é configurado utilizando:

```c
pdMS_TO_TICKS(500)
```

Essa macro converte:

```text
500 ms
   │
   ▼
pdMS_TO_TICKS()
   │
   ▼
Ticks do FreeRTOS
```

O número exato de ticks depende da frequência de tick configurada no FreeRTOS.

Por isso, utilizar:

```c
pdMS_TO_TICKS(500)
```

é preferível a realizar manualmente cálculos utilizando `portTICK_PERIOD_MS`.

---

# Inicialização do Timer

Após sua criação, o timer ainda precisa ser iniciado.

Isso é realizado através de:

```c
xTimerStart(xTimer, 0);
```

A estrutura é:

```c
xTimerStart(
    xTimer,
    xTicksToWait
);
```

Neste projeto:

```text
xTimer        = Timer criado anteriormente
xTicksToWait  = 0
```

O segundo argumento **não é o período do timer**.

Ele representa o tempo máximo que a Task chamadora aceita aguardar para que o comando de inicialização seja enviado ao serviço de timers do FreeRTOS.

O período propriamente dito já foi configurado em:

```c
pdMS_TO_TICKS(500)
```

---

# Timer de disparo único

O terceiro parâmetro de `xTimerCreate()` determina se o timer será periódico.

Neste projeto:

```c
pdFALSE
```

Isso significa:

```text
500 ms
  │
  ▼
DISPARO
  │
  ▼
FIM
```

O callback é executado apenas uma vez.

Esse comportamento é conhecido como:

```text
One-shot Timer
```

---

# Timer periódico

Caso fosse utilizado:

```c
pdTRUE
```

o timer seria automaticamente recarregado após cada disparo.

Exemplo:

```c
TimerHandle_t xTimer = xTimerCreate(
    "timer disparo",
    pdMS_TO_TICKS(500),
    pdTRUE,
    NULL,
    disparo
);
```

O comportamento seria:

```text
0 ms
 │
 ├──── 500 ms ────► disparo()
 │
 ├──── 500 ms ────► disparo()
 │
 ├──── 500 ms ────► disparo()
 │
 └──── 500 ms ────► disparo()
```

Assim, o callback seria executado aproximadamente a cada 500 ms.

---

# Função de callback

A função executada quando o timer expira é:

```c
void disparo(TimerHandle_t xTimer)
{
    printf(
        "Disparado em %lld \n",
        esp_timer_get_time() / 1000
    );
}
```

A função recebe como argumento o handle do timer responsável pelo disparo.

Neste exemplo, o argumento não é utilizado diretamente, mas poderia ser usado para identificar ou manipular o timer.

---

# Callback não é uma ISR

É importante diferenciar um callback de Software Timer de uma interrupção.

A função:

```c
disparo()
```

**não é uma ISR**.

Os callbacks dos Software Timers do FreeRTOS são executados no contexto da **Timer Service/Daemon Task** do FreeRTOS.

Portanto:

```text
Hardware Interrupt
      ≠
Software Timer Callback
```

Isso também significa que o callback deve ser mantido relativamente curto, pois callbacks demorados podem atrasar o processamento de outros Software Timers.

---

# Consulta do tempo do sistema

O projeto utiliza:

```c
esp_timer_get_time()
```

Essa função retorna o tempo transcorrido desde a inicialização do sistema em **microssegundos (µs)**.

Exemplo:

```c
int64_t tempo = esp_timer_get_time();
```

Se o retorno for:

```text
500000
```

isso representa aproximadamente:

```text
500000 µs
   ÷ 1000
      =
500 ms
```

Por isso o projeto utiliza:

```c
esp_timer_get_time() / 1000
```

convertendo o valor de microssegundos para milissegundos.

---

# FreeRTOS Timer × `esp_timer_get_time()`

Embora ambos estejam relacionados ao tempo, possuem funções diferentes neste projeto.

| Recurso | Função |
|---------|--------|
| FreeRTOS Software Timer | Agenda a execução do callback |
| `esp_timer_get_time()` | Consulta o tempo transcorrido |

Portanto:

```text
FreeRTOS Timer
      │
      └── determina QUANDO disparo() será executada

esp_timer_get_time()
      │
      └── informa QUANTO tempo passou desde o início
```

O disparo do callback neste código é controlado pelo **FreeRTOS Software Timer**, e não pelo `esp_timer`.

---

# Funções utilizadas

| Função | Finalidade |
|--------|------------|
| `xTimerCreate()` | Cria um Software Timer |
| `xTimerStart()` | Inicia a contagem do timer |
| `pdMS_TO_TICKS()` | Converte milissegundos para ticks |
| `esp_timer_get_time()` | Retorna o tempo desde a inicialização em µs |
| `printf()` | Exibe informações no terminal |

---

# Fluxo de execução

```text
             app_main()
                 │
                 ▼
        esp_timer_get_time()
                 │
                 ▼
      "Sistema iniciado em..."
                 │
                 ▼
          xTimerCreate()
                 │
                 ▼
       ┌─────────────────────┐
       │ Software Timer      │
       │ Período: 500 ms     │
       │ Auto Reload: FALSE  │
       └─────────────────────┘
                 │
                 ▼
          xTimerStart()
                 │
                 ▼
            Aguarda 500 ms
                 │
                 ▼
        Timer Service Task
                 │
                 ▼
            disparo()
                 │
                 ▼
        esp_timer_get_time()
                 │
                 ▼
          "Disparado em..."
```

---

# Exemplo de saída

Uma saída possível é:

```text
Sistema iniciado em 310
Disparado em 810
```

A diferença aproximada será:

```text
810 ms - 310 ms = 500 ms
```

Os valores absolutos podem variar, pois `app_main()` não necessariamente começa exatamente no instante zero do sistema.

Também podem existir pequenas variações devido ao escalonamento e funcionamento interno do sistema operacional.

---

# Conceitos importantes

## Timer

Um timer permite executar determinada ação após um intervalo temporal.

Neste projeto:

```text
Evento:
    expiração do timer

Período:
    500 ms

Ação:
    executar disparo()
```

---

## Timer Handle

O tipo:

```c
TimerHandle_t
```

é utilizado pelo FreeRTOS para representar um Software Timer.

Neste projeto:

```c
TimerHandle_t xTimer;
```

O handle é utilizado posteriormente para operações sobre o timer:

```c
xTimerStart(xTimer, 0);
```

Outras APIs do FreeRTOS também podem usar esse handle para parar, reiniciar, excluir ou modificar o período do timer.

---

## Callback

Um callback é uma função registrada para ser executada posteriormente quando determinado evento ocorrer.

Neste caso:

```text
Evento
   │
   ▼
Timer atingiu 500 ms
   │
   ▼
Callback
   │
   ▼
disparo()
```

A função não precisa ser chamada diretamente pelo `app_main()`.

---

# Timer × `vTaskDelay()`

Um Software Timer não deve ser confundido com:

```c
vTaskDelay()
```

| Característica | Software Timer | `vTaskDelay()` |
|----------------|----------------|----------------|
| Bloqueia a Task chamadora | Não | Sim |
| Executa callback | Sim | Não |
| Pode ser periódico | Sim | Pode ser usado em loop |
| Possui handle próprio | Sim | Não |
| Ideal para eventos temporizados | Sim | Depende da aplicação |

Por exemplo:

```c
vTaskDelay(pdMS_TO_TICKS(500));
disparo();
```

bloquearia a Task que executasse esse trecho durante o período.

Já o Software Timer permite agendar o callback e liberar a Task para outras atividades.

---

# Software Timer × Hardware Timer

Também é importante diferenciar um Software Timer de um timer de hardware.

| Característica | Software Timer | Hardware Timer |
|----------------|----------------|----------------|
| Gerenciado por | FreeRTOS | Periférico do microcontrolador |
| Execução | Software | Hardware |
| Callback | Timer Service Task | Geralmente associado a interrupção |
| Precisão | Dependente do RTOS/tick | Maior |
| Uso típico | Eventos de software | Temporização precisa |
| ISR | Não necessária | Frequentemente utilizada |

Software Timers são adequados para:

- timeouts;
- eventos periódicos;
- atualização de estados;
- tarefas de baixa criticidade temporal;
- temporizações de aplicação.

Para requisitos de temporização muito precisos, periféricos de hardware ou mecanismos específicos do ESP-IDF podem ser mais adequados.

---

# Aplicações práticas

Software Timers podem ser utilizados para:

- timeout de comunicação;
- desligamento automático;
- atualização periódica de estados;
- envio periódico de informações;
- supervisão de sensores;
- temporização de interfaces;
- watchdogs implementados em software;
- debounce temporizado;
- acionamento atrasado de funções;
- execução periódica de rotinas;
- controle de estados temporizados.

Exemplo:

```text
Recebe comando
      │
      ▼
Inicia timer de 5 s
      │
      ├──────── resposta recebida
      │              │
      │              ▼
      │         cancela timer
      │
      └──────── 5 s sem resposta
                     │
                     ▼
                  TIMEOUT
```

---

# Observações

- O timer utilizado é um **Software Timer do FreeRTOS**.
- O período configurado é de 500 ms.
- `pdFALSE` configura o timer para um único disparo.
- `pdTRUE` poderia ser utilizado para torná-lo periódico.
- `esp_timer_get_time()` não cria nem controla esse timer.
- `esp_timer_get_time()` retorna o tempo em microssegundos.
- A divisão por `1000` converte o valor para milissegundos.
- O callback é executado pela Timer Service Task do FreeRTOS.
- Callbacks de Software Timers devem evitar operações demoradas ou bloqueantes.

---

# Informações

| Info | Modelo |
|------|--------|
| Família | ESP32 |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Linguagem | C |
| IDE | ESP-IDF v5.4.2 |
| Recurso principal | FreeRTOS Software Timer |
| Período | 500 ms |
| Tipo | One-shot |
| Callback | `disparo()` |
| Medição de tempo | `esp_timer_get_time()` |

---

# Estrutura do projeto

```text
Timer/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```