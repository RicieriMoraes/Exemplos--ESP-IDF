# _Event Group_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
| ------ | ---------- | ------------ | ----------------- |
| 1.0.0  | 13/08/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a utilização de **Event Groups (Grupos de Eventos)** do FreeRTOS para sincronização de múltiplos eventos entre Tasks.

Duas Tasks independentes, `comTask` e `sensorTask`, representam respectivamente o recebimento de uma informação de comunicação e a realização de uma leitura de sensor. Cada Task sinaliza a ocorrência de seu evento através de um bit específico utilizando `xEventGroupSetBits()`.

A `procTask` permanece bloqueada aguardando que **os dois eventos ocorram**. Quando os bits correspondentes à comunicação e ao sensor estiverem setados simultaneamente, a Task é desbloqueada, os bits são automaticamente limpos e o processamento é executado.

---

## Objetivo

* Demonstrar a criação e utilização de um Event Group no FreeRTOS.
* Utilizar bits como sinalizadores independentes de eventos.
* Utilizar `xEventGroupSetBits()` para sinalizar eventos.
* Utilizar `xEventGroupWaitBits()` para aguardar múltiplos eventos.
* Sincronizar uma Task com duas condições independentes.
* Demonstrar operações bit a bit para agrupamento de eventos.
* Bloquear uma Task até que todas as condições necessárias sejam atendidas.
* Servir como base para aplicações orientadas a múltiplos eventos no ESP-IDF.

---

## Bibliotecas utilizadas

| Biblioteca                | Finalidade                                                                                          |
| ------------------------- | --------------------------------------------------------------------------------------------------- |
| `stdio.h`                 | Disponibiliza funções padrão de entrada e saída, como `printf()`.                                   |
| `freertos/FreeRTOS.h`     | Contém as definições principais do sistema operacional FreeRTOS.                                    |
| `freertos/task.h`         | Disponibiliza funções para criação e gerenciamento de Tasks.                                        |
| `freertos/event_groups.h` | Disponibiliza funções para criação, sinalização e espera de grupos de eventos.                      |

---

## Configuração do firmware

| Parâmetro                     | Configuração                 |
| ----------------------------- | ---------------------------- |
| Linguagem                     | C                            |
| Framework                     | ESP-IDF                      |
| Sistema operacional           | FreeRTOS                     |
| Função principal              | `app_main()`                 |
| Número de Tasks               | 3                            |
| Tipo de sincronização         | Event Group                  |
| Número de eventos             | 2                            |
| Evento de comunicação         | `BIT0`                       |
| Evento de sensor              | `BIT1`                       |
| Prioridade das Tasks          | 1                            |
| Stack de cada Task            | 2048 bytes                   |
| Intervalo da `comTask`        | 1000 ms                      |
| Intervalo da `sensorTask`     | 2000 ms                      |
| Tempo de espera da `procTask` | Indefinido (`portMAX_DELAY`) |

---

## Funcionamento

1. A função `app_main()` cria um Event Group através de `xEventGroupCreate()`.
2. São criadas três Tasks:

   * `comTask`;
   * `sensorTask`;
   * `procTask`.
3. A `comTask` simula o recebimento de um dado.
4. Após o evento, a `comTask` seta o bit `BIT0`.
5. A `sensorTask` simula uma leitura de sensor.
6. Após a leitura, a `sensorTask` seta o bit `BIT1`.
7. A `procTask` permanece bloqueada aguardando os dois bits.
8. Quando `BIT0` e `BIT1` estiverem setados, `xEventGroupWaitBits()` retorna.
9. Os bits são automaticamente limpos.
10. A `procTask` executa o processamento correspondente.
11. O processo é repetido continuamente.

---

## Tasks implementadas

| Task         | Função                                           | Prioridade | Stack      | Intervalo |
| ------------ | ------------------------------------------------ | ---------- | ---------- | --------- |
| `comTask`    | Simula o recebimento de dados e seta `BIT0`      | 1          | 2048 bytes | 1000 ms   |
| `sensorTask` | Simula a leitura de sensor e seta `BIT1`         | 1          | 2048 bytes | 2000 ms   |
| `procTask`   | Aguarda os dois eventos e inicia o processamento | 1          | 2048 bytes | Evento    |

---

## Criação do Event Group

O grupo de eventos é representado pelo handle:

```c
EventGroupHandle_t grupoEvt;
```

Sua criação é realizada através de:

```c
grupoEvt = xEventGroupCreate();
```

Esse handle é compartilhado entre todas as Tasks que irão sinalizar ou aguardar eventos.

---

## Sinalizadores utilizados

Os eventos são representados através de bits:

```c
const int goCom = BIT0;
const int goSensor = BIT1;
```

Correspondência:

| Evento      | Macro  | Binário      |
| ----------- | ------ | ------------ |
| Comunicação | `BIT0` | `0b00000001` |
| Sensor      | `BIT1` | `0b00000010` |

Os bits podem ser combinados através do operador OR:

```c
goCom | goSensor
```

Resultado:

```text
0b00000001
OR
0b00000010
------------
0b00000011
```

Assim, a `procTask` pode aguardar os dois eventos simultaneamente.

---

## Sinalização de eventos

### Evento de comunicação

A `comTask` sinaliza o recebimento de um dado através de:

```c
xEventGroupSetBits(grupoEvt, goCom);
```

Isso seta o `BIT0` do Event Group.

---

### Evento de sensor

A `sensorTask` sinaliza a conclusão da leitura através de:

```c
xEventGroupSetBits(grupoEvt, goSensor);
```

Isso seta o `BIT1`.

---

## Espera pelos eventos

A `procTask` utiliza:

```c
xEventGroupWaitBits(
    grupoEvt,
    goCom | goSensor,
    true,
    true,
    portMAX_DELAY
);
```

A estrutura geral da função é:

```c
xEventGroupWaitBits(
    EventGroup,
    BitsToWaitFor,
    ClearOnExit,
    WaitForAllBits,
    TicksToWait
);
```

---

## Parâmetros utilizados em `xEventGroupWaitBits()`

| Parâmetro         | Valor               | Descrição                               |
| ----------------- | ------------------- | --------------------------------------- |
| Event Group       | `grupoEvt`          | Handle do grupo de eventos              |
| Bits aguardados   | `goCom \ goSensor` | Aguarda `BIT0` e `BIT1`                 |
| Limpar após saída | `true`              | Limpa os bits após o desbloqueio        |
| Aguardar todos    | `true`              | Exige que todos os bits estejam setados |
| Tempo de espera   | `portMAX_DELAY`     | Aguarda indefinidamente                 |

---

## Comportamento do grupo de eventos

Inicialmente:

```text
BIT1 BIT0
  0    0
```

Após a `comTask`:

```text
BIT1 BIT0
  0    1
```

A `procTask` continua bloqueada.

Após a `sensorTask`:

```text
BIT1 BIT0
  1    1
```

Agora os dois eventos estão presentes e a `procTask` é liberada.

Como o parâmetro `ClearOnExit` está configurado como `true`, após o processamento:

```text
BIT1 BIT0
  0    0
```

O sistema volta a aguardar um novo conjunto de eventos.

---

## Funções utilizadas

| Função                  | Finalidade                                   |
| ----------------------- | -------------------------------------------- |
| `xEventGroupCreate()`   | Cria um novo Event Group.                    |
| `xEventGroupSetBits()`  | Seta um ou mais bits do grupo de eventos.    |
| `xEventGroupWaitBits()` | Aguarda um ou mais eventos específicos.      |
| `xTaskCreate()`         | Cria uma nova Task.                          |
| `vTaskDelay()`          | Suspende temporariamente a execução da Task. |
| `printf()`              | Exibe mensagens no terminal serial.          |

---

## Fluxo de execução

```text
        comTask                       sensorTask
           │                              │
           ▼                              ▼
     Dado recebido                 Leitura realizada
           │                              │
           ▼                              ▼
      Seta BIT0                       Seta BIT1
           │                              │
           └──────────────┬───────────────┘
                          │
                          ▼
                 ┌─────────────────┐
                 │   Event Group   │
                 │  BIT1 | BIT0    │
                 └─────────────────┘
                          │
                          ▼
              Aguarda BIT0 + BIT1
                          │
                          ▼
                     procTask
                          │
                          ▼
       Recebido comunicação + sensor
                          │
                          ▼
                  Limpa os bits
```

---

## Exemplo de saída

```text
Dado recebido
Leitura realizada
Dado recebido
Recebido a requisição e leitura do sensor

Dado recebido
Leitura realizada
Recebido a requisição e leitura do sensor
```

A ordem exata das mensagens pode variar de acordo com o escalonamento do FreeRTOS.

---

## Conceitos importantes

### Event Group

Um Event Group é uma estrutura do FreeRTOS que permite utilizar vários bits como sinalizadores independentes de eventos.

Cada bit pode representar uma condição diferente:

```text
BIT0 → Comunicação recebida
BIT1 → Sensor atualizado
BIT2 → Wi-Fi conectado
BIT3 → Memória disponível
BIT4 → Processo concluído
```

Isso permite que uma única Task aguarde diferentes combinações de eventos.

---

### Operações bit a bit

Os eventos são combinados utilizando operações binárias.

Exemplo:

```c
BIT0 | BIT1
```

O operador `|` representa uma operação OR bit a bit.

```text
00000001
00000010
--------
00000011
```

---

### Esperar todos os eventos

Neste projeto:

```c
WaitForAllBits = true
```

Portanto, a `procTask` somente continua quando:

```text
BIT0 = 1
E
BIT1 = 1
```

Se apenas um evento ocorrer, a Task permanece bloqueada.

---

### Esperar qualquer evento

Caso o parâmetro fosse:

```c
false
```

a Task seria desbloqueada quando **qualquer um dos bits** fosse setado.

Exemplo:

```c
xEventGroupWaitBits(
    grupoEvt,
    goCom | goSensor,
    true,
    false,
    portMAX_DELAY
);
```

Nesse caso:

```text
BIT0 OU BIT1 → desbloqueia a Task
```

---

## Event Group × Semaphore

| Característica                         | Event Group             | Semáforo            |
| -------------------------------------- | ----------------------- | ------------------- |
| Representa múltiplos eventos           | ✔                       | ✘                   |
| Utiliza sinalizadores em bits          | ✔                       | ✘                   |
| Aguarda vários eventos simultaneamente | ✔                       | ✘                   |
| Sincronização entre Tasks              | ✔                       | ✔                   |
| Transporte de dados                    | ✘                       | ✘                   |
| Contagem de eventos                    | ✘                       | Depende do tipo     |
| Uso típico                             | Combinação de condições | Sinalização simples |

---

## Event Group × Task Notification

| Característica                         | Event Group         | Task Notification |
| -------------------------------------- | ------------------- | ----------------- |
| Múltiplas Tasks podem aguardar eventos | ✔                   | Mais limitado     |
| Vários bits como eventos               | ✔                   | ✔                 |
| Estrutura dedicada                     | ✔                   | Não               |
| Consumo de memória                     | Maior               | Menor             |
| Sincronização de múltiplas condições   | Excelente           | Possível          |
| Comunicação direta Task → Task         | Não necessariamente | ✔                 |

---

## Aplicações práticas

Event Groups podem ser utilizados em situações como:

* Aguardar conexão Wi-Fi e sincronização de horário;
* Aguardar sensor e comunicação;
* Verificar inicialização de múltiplos periféricos;
* Sincronizar diferentes módulos da aplicação;
* Controlar máquinas de estados;
* Aguardar conclusão de múltiplas tarefas;
* Sincronizar aquisição, processamento e transmissão;
* Controlar etapas de inicialização do firmware.

Exemplo:

```text
Wi-Fi conectado ────────► BIT0
Sensor inicializado ─────► BIT1
SD Card disponível ──────► BIT2

                           │
                           ▼
                  Aguarda BIT0 + BIT1 + BIT2
                           │
                           ▼
                  Inicia aplicação principal
```

---

## Observações

* Um Event Group utiliza bits individuais para representar diferentes eventos.
* A função `xEventGroupSetBits()` pode setar um ou vários bits simultaneamente.
* A função `xEventGroupWaitBits()` pode aguardar todos os eventos ou apenas um deles.
* Como `ClearOnExit` está configurado como `true`, os bits aguardados são limpos após a condição ser satisfeita.
* Como `WaitForAllBits` também está em `true`, a `procTask` depende dos dois eventos.
* A `comTask` executa a cada 1 segundo, enquanto a `sensorTask` executa a cada 2 segundos.
* Portanto, o evento de comunicação pode ser setado mais de uma vez antes da chegada de uma nova leitura de sensor, mas o bit continua simplesmente no estado `1`; Event Groups não contam quantas vezes um evento ocorreu.

---

## Event Group não é contador

É importante observar que os bits representam apenas estados:

```text
0 = Evento não sinalizado
1 = Evento sinalizado
```

Se a `comTask` executar várias vezes:

```c
xEventGroupSetBits(grupoEvt, goCom);
```

antes do processamento, o `BIT0` continuará apenas como:

```text
1
```

Ele não armazenará quantas vezes o evento ocorreu.

Caso seja necessário contar eventos, podem ser utilizados:

* Semáforo Contador;
* Queue;
* Task Notification como contador.

---

## Informações

| Info                | Modelo          |
| ------------------- | --------------- |
| Família             | ESP32           |
| Framework           | ESP-IDF         |
| Sistema operacional | FreeRTOS        |
| Linguagem           | C               |
| IDE                 | ESP-IDF v5.4.2  |
| Sincronização       | Event Group     |
| Eventos utilizados  | 2               |
| Sinalizadores       | `BIT0` e `BIT1` |

---

## Estrutura do projeto

```text
Event-Group/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```
