# _Notification_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
|--------|------------|--------------|-------------------|
| 1.0.0  | 30/07/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a comunicação e sincronização entre Tasks utilizando o recurso de **Task Notifications** do FreeRTOS no ESP-IDF.

A `Task1` preenche sequencialmente um array com diferentes valores e envia uma notificação após cada atualização. A `Task2` permanece bloqueada aguardando essas notificações e utiliza um contador para identificar e exibir os valores armazenados no array.

As notificações diretas são um mecanismo leve e eficiente de comunicação entre tarefas, pois utilizam uma área de notificação interna presente em cada Task, dispensando a criação de filas, semáforos ou estruturas adicionais em situações simples.

---

## Objetivo

- Demonstrar o envio de notificações entre Tasks.
- Utilizar um `TaskHandle_t` para identificar a Task destinatária.
- Utilizar `xTaskNotifyGive()` para enviar eventos.
- Utilizar `ulTaskNotifyTake()` para aguardar e contabilizar notificações.
- Demonstrar o bloqueio de uma Task até o recebimento de um evento.
- Compartilhar dados entre Tasks por meio de um array global.
- Apresentar uma alternativa leve aos semáforos e filas do FreeRTOS.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Disponibiliza funções padrão de entrada e saída, como `printf()`. |
| `freertos/FreeRTOS.h` | Contém as definições principais do sistema operacional FreeRTOS. |
| `freertos/task.h` | Disponibiliza funções para criação, gerenciamento e notificação de Tasks. |

---

## Configuração do firmware

| Parâmetro | Configuração |
|-----------|--------------|
| Linguagem | C |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Função principal | `app_main()` |
| Número de Tasks | 2 |
| Prioridade das Tasks | 2 |
| Stack de cada Task | 2048 bytes |
| Número de notificações por ciclo | 5 |
| Intervalo entre ciclos | 2000 ms |
| Tempo máximo de espera da Task2 | Indefinido (`portMAX_DELAY`) |
| Estrutura de dados compartilhada | Array global com 5 posições |

---

## Funcionamento

1. A função `app_main()` cria a `Task1` e a `Task2`.
2. A `Task1` preenche as posições do array `DATA`.
3. Após preencher cada posição, a `Task1` envia uma notificação.
4. A `Task2` aguarda o recebimento das notificações por meio de `ulTaskNotifyTake()`.
5. Cada notificação incrementa o contador interno da Task destinatária.
6. O valor retornado pelo contador é utilizado como índice para acessar o array.
7. O conteúdo correspondente é exibido no terminal serial.
8. Após enviar cinco notificações, a `Task1` aguarda 2000 ms e reinicia o processo.

---

## Tasks implementadas

| Task | Função | Prioridade | Stack |
|------|--------|------------|-------|
| Task1 | Preencher o array e enviar notificações | 2 | 2048 bytes |
| Task2 | Receber notificações e exibir os valores | 2 | 2048 bytes |

---

## Estrutura de dados

O projeto utiliza um array global com cinco posições:

```c
int DATA[5] = {0};
```

Durante a execução, o array recebe os seguintes valores:

| Índice | Valor |
|-------:|------:|
| 0 | 10 |
| 1 | 20 |
| 2 | 30 |
| 3 | 40 |
| 4 | 50 |

Após cada preenchimento, uma notificação é enviada para a Task responsável pelo processamento dos dados.

---

## Task Handle

Um `TaskHandle_t` funciona como uma referência para uma Task criada pelo FreeRTOS.

```c
static TaskHandle_t tasksHandler = NULL;
```

Esse identificador é utilizado para informar qual Task deve receber a notificação:

```c
xTaskNotifyGive(tasksHandler);
```

---

## Envio de notificações

A função `xTaskNotifyGive()` incrementa o valor de notificação da Task especificada:

```c
xTaskNotifyGive(tasksHandler);
```

Cada chamada funciona de forma semelhante à liberação de um semáforo contador.

Neste projeto, a `Task1` executa cinco chamadas por ciclo, uma para cada valor inserido no array.

---

## Recebimento de notificações

A `Task2` utiliza a função:

```c
int counter = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
```

### Parâmetros utilizados

| Parâmetro | Valor | Descrição |
|-----------|-------|-----------|
| Limpeza do contador | `pdFALSE` | Decrementa o contador em uma unidade após o recebimento. |
| Tempo de espera | `portMAX_DELAY` | Mantém a Task bloqueada até receber uma notificação. |

O retorno da função representa o valor acumulado no contador de notificações antes de sua redução.

---

## Comportamento de `pdFALSE`

Ao utilizar:

```c
ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
```

o contador de notificações é reduzido em uma unidade a cada chamada.

Por exemplo, caso cinco notificações estejam pendentes:

| Chamada | Retorno | Contador após a chamada |
|--------:|--------:|-------------------------:|
| 1 | 5 | 4 |
| 2 | 4 | 3 |
| 3 | 3 | 2 |
| 4 | 2 | 1 |
| 5 | 1 | 0 |

Por esse motivo, os valores podem ser exibidos na ordem inversa:

```text
Notificação recebida 50
Notificação recebida 40
Notificação recebida 30
Notificação recebida 20
Notificação recebida 10
```

---

## Funções utilizadas

| Função | Finalidade |
|--------|------------|
| `xTaskCreate()` | Cria uma nova Task no FreeRTOS. |
| `xTaskNotifyGive()` | Incrementa o contador de notificações de uma Task. |
| `ulTaskNotifyTake()` | Aguarda e consome notificações recebidas. |
| `vTaskDelay()` | Suspende temporariamente a execução de uma Task. |
| `printf()` | Exibe informações no terminal serial. |

---

## Exemplo de saída

```text
Notificação recebida 50
Notificação recebida 40
Notificação recebida 30
Notificação recebida 20
Notificação recebida 10
```

A ordem de exibição pode variar de acordo com o escalonamento das Tasks e com o momento em que as notificações forem processadas.

---

## Conceitos importantes

### Task Notification

Cada Task do FreeRTOS possui um valor interno de notificação que pode ser utilizado para:

- Enviar eventos;
- Desbloquear Tasks;
- Armazenar valores;
- Implementar contadores;
- Substituir semáforos simples;
- Realizar sincronização entre tarefas.

### Task bloqueada

Ao utilizar `portMAX_DELAY`, a `Task2` permanece bloqueada enquanto não houver notificações disponíveis.

Durante esse período, ela não consome tempo de processamento.

### Notificação acumulada

Quando várias notificações são enviadas antes que a Task destinatária seja executada, elas ficam acumuladas no contador interno da Task.

### Dados compartilhados

O array `DATA` é global e pode ser acessado pelas duas Tasks. Em aplicações mais complexas, o acesso simultâneo a dados compartilhados pode exigir mecanismos de proteção, como:

- Mutex;
- Seções críticas;
- Filas;
- Semáforos.

---

## Informações

| Info | Modelo |
|------|--------|
| Família | ESP32 |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Linguagem | C |
| IDE | ESP-IDF v5.4.2 |
| Comunicação entre Tasks | Task Notifications |

---

## Estrutura do projeto

```text
Notification/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```