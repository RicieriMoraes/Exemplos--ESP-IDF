# _Queue_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
|--------|------------|--------------|-------------------|
| 1.0.0  | 10/08/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a utilização de uma **Queue (Fila)** do FreeRTOS para comunicação e transferência de dados entre duas Tasks.

A `comTask` simula o recebimento periódico de novos dados, incrementa um contador e envia esse valor para uma fila utilizando `xQueueSend()`. A `procTask` aguarda os dados disponíveis na fila por meio de `xQueueReceive()` e, quando recebe um novo elemento, inicia seu processamento e exibe o valor recebido no terminal serial.

O exemplo apresenta uma das principais formas de comunicação entre Tasks no FreeRTOS, permitindo transferir dados de maneira organizada e segura entre diferentes partes da aplicação.

---

## Objetivo

- Demonstrar a criação e utilização de uma Queue no FreeRTOS.
- Realizar comunicação entre Tasks através de uma fila.
- Utilizar `xQueueSend()` para inserir dados na fila.
- Utilizar `xQueueReceive()` para retirar dados da fila.
- Demonstrar o armazenamento temporário de múltiplos elementos.
- Implementar tempos máximos de espera para envio e recebimento.
- Verificar falhas na criação, envio e recebimento da fila.
- Servir como base para transferência de dados entre diferentes módulos de uma aplicação embarcada.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Disponibiliza funções padrão de entrada e saída, como `printf()`. |
| `freertos/FreeRTOS.h` | Contém as definições principais do sistema operacional FreeRTOS. |
| `freertos/task.h` | Disponibiliza funções para criação e gerenciamento de Tasks. |
| `freertos/queue.h` | As funções de Queue são disponibilizadas pelo FreeRTOS através desta biblioteca. |

---

## Configuração do firmware

| Parâmetro | Configuração |
|-----------|--------------|
| Linguagem | C |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Função principal | `app_main()` |
| Número de Tasks | 2 |
| Tipo de comunicação | Queue |
| Capacidade da fila | 4 elementos |
| Tipo de cada elemento | `int` |
| Stack da `comTask` | 2048 bytes |
| Stack da `procTask` | 2048 bytes |
| Prioridade da `comTask` | 2 |
| Prioridade da `procTask` | 1 |
| Timeout de envio | 1000 ms |
| Timeout de recebimento | 2000 ms |
| Intervalo entre novos dados | 5000 ms |

---

## Funcionamento

1. A função `app_main()` cria uma Queue capaz de armazenar até quatro valores do tipo `int`.
2. O retorno de `xQueueCreate()` é armazenado no handle `queue`.
3. O firmware verifica se a criação da fila foi realizada corretamente.
4. São criadas duas Tasks:
   - `comTask`;
   - `procTask`.
5. A `comTask` incrementa continuamente a variável `counter`.
6. O valor de `counter` é inserido na fila utilizando `xQueueSend()`.
7. A `procTask` executa `xQueueReceive()` e aguarda a chegada de novos dados.
8. Quando um elemento é recebido, ele é armazenado na variável `receive`.
9. O valor recebido é então exibido no terminal serial.
10. O processo se repete continuamente.

---

## Tasks implementadas

| Task | Função | Prioridade | Stack |
|------|--------|------------|-------|
| `comTask` | Gera e envia dados para a Queue | 2 | 2048 bytes |
| `procTask` | Recebe e processa os dados da Queue | 1 | 2048 bytes |

---

## Criação da Queue

A fila é criada através de:

```c
queue = xQueueCreate(4, sizeof(int));
```

A estrutura da função é:

```c
xQueueCreate(
    QueueLength,
    ItemSize
);
```

Neste projeto:

| Parâmetro | Valor | Descrição |
|-----------|------:|-----------|
| `QueueLength` | 4 | Número máximo de elementos armazenados simultaneamente |
| `ItemSize` | `sizeof(int)` | Tamanho de cada elemento da fila |

Portanto, a Queue pode armazenar:

```text
4 valores inteiros
```

---

## Handler da Queue

O identificador da fila é armazenado em:

```c
QueueHandle_t queue;
```

Esse handle é compartilhado entre as Tasks e utilizado pelas funções de envio e recebimento.

---

## Envio de dados

A `comTask` insere dados utilizando:

```c
long result = xQueueSend(
    queue,
    &counter,
    1000 / portTICK_PERIOD_MS
);
```

A função recebe:

| Parâmetro | Finalidade |
|-----------|------------|
| `queue` | Handle da fila |
| `&counter` | Endereço do dado que será copiado para a fila |
| `1000 / portTICK_PERIOD_MS` | Tempo máximo de espera caso a fila esteja cheia |

Se houver espaço disponível, o dado é copiado para a fila.

---

## Verificação do envio

O código verifica o retorno de `xQueueSend()`:

```c
if (result) {
    printf("Dado inserido na fila com sucesso \n");
} else {
    printf("Erro ao inserir dado na fila \n");
}
```

Em FreeRTOS, é mais comum utilizar:

```c
BaseType_t result;
```

e verificar explicitamente:

```c
if (result == pdPASS)
```

---

## Recebimento de dados

A `procTask` utiliza:

```c
long result = xQueueReceive(
    queue,
    &receive,
    2000 / portTICK_PERIOD_MS
);
```

A função tenta retirar o primeiro elemento disponível da fila.

Se nenhum dado estiver disponível, a Task aguarda por até:

```text
2000 ms
```

Caso um elemento seja recebido, ele é copiado para a variável:

```c
receive
```

---

## Estrutura FIFO

Uma Queue do FreeRTOS normalmente funciona seguindo o princípio:

```text
FIFO
First In, First Out
```

Ou seja:

```text
Primeiro dado inserido → Primeiro dado retirado
```

Exemplo:

```text
Entrada:

1 → 2 → 3 → 4
```

A ordem de retirada será:

```text
1 → 2 → 3 → 4
```

---

## Representação da fila

```text
              Queue
        ┌────┬────┬────┬────┐
Entrada │  1 │  2 │  3 │  4 │ → Saída
        └────┴────┴────┴────┘
             4 posições
```

A `comTask` insere dados em uma extremidade enquanto a `procTask` retira os dados para processamento.

---

## Funções utilizadas

| Função | Finalidade |
|--------|------------|
| `xQueueCreate()` | Cria uma nova Queue. |
| `xQueueSend()` | Insere um elemento no final da fila. |
| `xQueueReceive()` | Retira o primeiro elemento disponível da fila. |
| `xTaskCreate()` | Cria uma nova Task. |
| `vTaskDelay()` | Suspende temporariamente a execução da Task. |
| `printf()` | Exibe mensagens no terminal serial. |

---

## Fluxo de execução

```text
             comTask
                │
                ▼
        Incrementa counter
                │
                ▼
          Dado recebido
                │
                ▼
       xQueueSend(queue)
                │
                ▼
        ┌──────────────┐
        │    Queue     │
        │  [ dado ]    │
        └──────────────┘
                │
                ▼
       xQueueReceive(queue)
                │
                ▼
             procTask
                │
                ▼
      Inicia processamento
```

---

## Exemplo de saída

```text
Dado recebido
Dado inserido na fila com sucesso
Iniciando processamento - Dado recebido: 1

Dado recebido
Dado inserido na fila com sucesso
Iniciando processamento - Dado recebido: 2

Dado recebido
Dado inserido na fila com sucesso
Iniciando processamento - Dado recebido: 3
```

O valor enviado aumenta continuamente através da variável `counter`.

---

## Conceitos importantes

### Queue

Uma Queue é uma estrutura de comunicação entre Tasks que permite armazenar e transportar dados.

Diferentemente de um Semáforo, que normalmente sinaliza apenas a ocorrência de um evento, uma Queue pode transportar o conteúdo associado ao evento.

---

### Cópia de dados

Ao executar:

```c
xQueueSend(queue, &counter, ...);
```

o FreeRTOS copia o conteúdo da variável para a memória interna da Queue.

Portanto, a fila não armazena simplesmente o endereço original de `counter`.

---

### Buffer de dados

Como a Queue possui quatro posições, o produtor pode inserir até quatro elementos antes que o consumidor precise removê-los.

Isso permite desacoplar temporalmente as Tasks.

Exemplo:

```text
Task produtora rápida
        │
        ▼
    [ Queue ]
        │
        ▼
Task consumidora lenta
```

---

### Timeout de envio

Caso a fila esteja cheia, `xQueueSend()` aguarda até:

```text
1000 ms
```

pela liberação de uma posição.

Se nenhuma posição ficar disponível nesse período, ocorre falha no envio.

---

### Timeout de recebimento

Caso a fila esteja vazia, `xQueueReceive()` aguarda até:

```text
2000 ms
```

por um novo elemento.

Após esse período, a função retorna sem receber dados.

---

## Queue × Semaphore

| Característica | Queue | Semáforo |
|---------------|-------|----------|
| Transporte de dados | ✔ | ✘ |
| Armazenamento de vários elementos | ✔ | Limitado |
| Sincronização | ✔ | ✔ |
| Comunicação entre Tasks | ✔ | ✔ |
| Implementação FIFO | ✔ | ✘ |
| Uso típico | Transporte de dados | Sinalização de eventos |

---

## Queue × Task Notification

| Característica | Queue | Task Notification |
|---------------|-------|-------------------|
| Transporta estruturas complexas | ✔ | Limitado a valor de notificação |
| Múltiplos elementos pendentes | ✔ | Limitado |
| Vários produtores | ✔ | ✔ |
| Vários consumidores | ✔ | Mais limitado |
| Consumo de memória | Maior | Menor |
| Flexibilidade | Alta | Alta para casos simples |

Task Notifications normalmente são mais leves, enquanto Queues são mais adequadas quando é necessário armazenar e transportar múltiplos dados.

---

## Aplicações práticas

Queues são muito utilizadas em aplicações como:

- Leituras de sensores;
- Pacotes UART;
- Mensagens CAN;
- Dados recebidos por Wi-Fi;
- Eventos de interface;
- Comandos entre Tasks;
- Buffers de aquisição;
- Dados de ADC;
- Registros para armazenamento;
- Comunicação produtor-consumidor.

Exemplo:

```text
Sensor
  │
  ▼
Task de aquisição
  │
  ▼
Queue
  │
  ▼
Task de processamento
  │
  ▼
Task de comunicação
```

---

## Observações

- A Queue possui capacidade para quatro elementos inteiros.
- Quando a fila estiver cheia, novos envios podem bloquear a Task produtora até que exista espaço disponível.
- Quando estiver vazia, a Task consumidora pode permanecer bloqueada aguardando novos dados.
- O FreeRTOS realiza a cópia dos dados inseridos na fila.
- Para estruturas maiores, deve-se considerar o impacto no consumo de memória.
- Também é possível criar Queues contendo `struct`, ponteiros ou outros tipos de dados.
- Para aguardar indefinidamente por novos dados, poderia ser utilizado `portMAX_DELAY` em `xQueueReceive()`.

---

### Conversão de tempo

Pode-se substituir:

```c
1000 / portTICK_PERIOD_MS
```

por:

```c
pdMS_TO_TICKS(1000)
```

E:

```c
5000 / portTICK_PERIOD_MS
```

por:

```c
pdMS_TO_TICKS(5000)
```

---

## Informações

| Info | Modelo |
|------|--------|
| Família | ESP32 |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Linguagem | C |
| IDE | ESP-IDF v5.4.2 |
| Comunicação entre Tasks | Queue |
| Estrutura | FIFO |
| Capacidade | 4 elementos |

---

## Estrutura do projeto

```text
Queue/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```