# _Notification - Máquina de Estado_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
|--------|------------|--------------|-------------------|
| 1.0.0  | 30/07/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a comunicação entre Tasks utilizando o recurso de **Task Notifications** do FreeRTOS no ESP-IDF.

A `Task1` envia diferentes valores binários para a `Task2` por meio da função `xTaskNotify()`. Durante os envios, são utilizadas as ações `eSetValueWithoutOverwrite` e `eSetBits`, demonstrando tanto a escrita direta de valores quanto a manipulação de bits no valor interno de notificação da Task destinatária.

A `Task2` permanece bloqueada aguardando os eventos e recupera os valores recebidos com `xTaskNotifyWait()`, podendo utilizá-los como comandos, flags ou estados de uma máquina de estados.

---

## Objetivo

- Demonstrar o envio de valores entre Tasks.
- Utilizar `xTaskNotify()` para enviar notificações.
- Utilizar `xTaskNotifyWait()` para aguardar notificações.
- Compreender o funcionamento de `eSetValueWithoutOverwrite`.
- Compreender o funcionamento de `eSetBits`.
- Representar eventos utilizando valores binários.
- Utilizar notificações como flags de eventos.
- Demonstrar uma possível aplicação em máquinas de estados.
- Apresentar uma alternativa leve às filas e aos grupos de eventos do FreeRTOS.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Disponibiliza funções padrão de entrada e saída, como `printf()`. |
| `freertos/FreeRTOS.h` | Contém as definições principais do sistema operacional FreeRTOS. |
| `freertos/task.h` | Disponibiliza funções para criação, controle e notificação de Tasks. |

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
| Comunicação | Task Notifications |
| Tipo do valor enviado | `uint32_t` |
| Intervalo entre notificações | 1000 ms |
| Tempo máximo de espera da Task2 | Indefinido (`portMAX_DELAY`) |

---

## Funcionamento

1. A função `app_main()` cria a `Task1` e a `Task2`.
2. A `Task1` aguarda 1000 ms antes de cada envio.
3. Os três primeiros valores são enviados utilizando `eSetValueWithoutOverwrite`.
4. Os dois últimos valores são enviados utilizando `eSetBits`.
5. A `Task2` permanece bloqueada em `xTaskNotifyWait()`.
6. Quando uma notificação é recebida, o valor é armazenado na variável `estado`.
7. O valor recebido é exibido no terminal serial.
8. Após o último envio, o processo é repetido continuamente.

---

## Tasks implementadas

| Task | Função | Prioridade | Stack |
|------|--------|------------|-------|
| Task1 | Enviar valores e bits por meio de notificações | 2 | 2048 bytes |
| Task2 | Aguardar, receber e exibir as notificações | 2 | 2048 bytes |

---

## Valores enviados

| Ordem | Valor binário | Valor decimal | Ação utilizada |
|------:|---------------|--------------:|----------------|
| 1 | `0b00001` | 1 | `eSetValueWithoutOverwrite` |
| 2 | `0b00010` | 2 | `eSetValueWithoutOverwrite` |
| 3 | `0b00100` | 4 | `eSetValueWithoutOverwrite` |
| 4 | `0b01000` | 8 | `eSetBits` |
| 5 | `0b10000` | 16 | `eSetBits` |

Cada bit pode representar um evento, comando ou estado diferente da aplicação.

---

## Task Handle

O tipo `TaskHandle_t` é utilizado como uma referência para uma Task criada pelo FreeRTOS.

```c
static TaskHandle_t tasksHandler = NULL;
```

Esse identificador informa qual Task receberá a notificação:

```c
xTaskNotify(tasksHandler, 0b00001, eSetValueWithoutOverwrite);
```

---

## Envio de notificações

A função `xTaskNotify()` permite enviar um valor de 32 bits para uma Task.

```c
xTaskNotify(
    tasksHandler,
    0b00001,
    eSetValueWithoutOverwrite
);
```

Sua estrutura básica é:

```c
xTaskNotify(
    TaskHandle,       // Handler da Task destinatária
    Value,            // Valor de 32 bits enviado
    NotifyAction      // Ação aplicada ao valor de notificação
);
```

---

## Ações de notificação utilizadas

### `eSetValueWithoutOverwrite`

A ação `eSetValueWithoutOverwrite` grava um novo valor somente quando não existe uma notificação pendente para a Task destinatária.

```c
xTaskNotify(
    task2Handler,
    0b00001,
    eSetValueWithoutOverwrite
);
```

Caso uma notificação anterior ainda não tenha sido processada, o novo valor não substitui o valor pendente.

Por isso, é recomendado verificar o retorno da função:

```c
BaseType_t result = xTaskNotify(
    task2Handler,
    0b00001,
    eSetValueWithoutOverwrite
);

if (result != pdPASS) {
    printf("A notificação anterior ainda está pendente\n");
}
```

---

### `eSetBits`

A ação `eSetBits` realiza uma operação lógica OR entre o valor enviado e o valor atual da notificação.

```c
xTaskNotify(
    task2Handler,
    0b01000,
    eSetBits
);
```

Exemplo:

```text
Valor atual:   0b00001
Valor enviado: 0b01000
Resultado:     0b01001
```

Essa ação permite utilizar cada bit como uma flag independente.

---

## Recebimento de notificações

A `Task2` utiliza a função:

```c
xTaskNotifyWait(
    estado,
    0,
    &estado,
    portMAX_DELAY
);
```

A estrutura da função é:

```c
xTaskNotifyWait(
    BitsToClearOnEntry,
    BitsToClearOnExit,
    NotificationValue,
    TicksToWait
);
```

| Parâmetro | Finalidade |
|-----------|------------|
| `BitsToClearOnEntry` | Define quais bits serão limpos antes de aguardar uma nova notificação. |
| `BitsToClearOnExit` | Define quais bits serão limpos após o recebimento. |
| `NotificationValue` | Ponteiro que recebe o valor da notificação. |
| `TicksToWait` | Tempo máximo durante o qual a Task permanecerá aguardando. |

---

## Máscaras de entrada e saída

No código original, a função é chamada desta forma:

```c
xTaskNotifyWait(estado, 0, &estado, portMAX_DELAY);
```

O valor anterior de `estado` é utilizado como máscara de limpeza na entrada. Portanto, os bits recebidos anteriormente são apagados antes de uma nova espera.

Como a máscara de saída é igual a zero, nenhum bit é apagado imediatamente após a leitura.

Uma forma mais explícita de receber uma notificação e limpar todos os bits após a leitura é:

```c
xTaskNotifyWait(
    0,
    UINT32_MAX,
    &estado,
    portMAX_DELAY
);
```

Nesse caso:

- Nenhum bit é apagado antes da espera;
- Todos os bits são apagados após serem copiados para `estado`.

Para utilizar `UINT32_MAX`, pode ser incluído:

```c
#include <stdint.h>
```

---

## Funções utilizadas

| Função | Finalidade |
|--------|------------|
| `xTaskCreate()` | Cria uma nova Task. |
| `xTaskNotify()` | Envia um valor ou conjunto de bits para uma Task. |
| `xTaskNotifyWait()` | Aguarda e recupera o valor de uma notificação. |
| `vTaskDelay()` | Suspende temporariamente a execução de uma Task. |
| `printf()` | Exibe informações no terminal serial. |

---

## Exemplo de saída

Considerando que cada notificação seja processada antes do próximo envio, a saída esperada é:

```text
Notificação recebida 1
Notificação recebida 2
Notificação recebida 4
Notificação recebida 8
Notificação recebida 16
```

Os valores correspondem às representações binárias enviadas pela `Task1`.

---

## Aplicação como máquina de estados

Os valores recebidos podem ser associados a diferentes estados ou comandos:

```c
#define EVENTO_INICIAR    (1 << 0)
#define EVENTO_PARAR      (1 << 1)
#define EVENTO_PAUSAR     (1 << 2)
#define EVENTO_ERRO       (1 << 3)
#define EVENTO_FINALIZAR  (1 << 4)
```

A Task receptora pode verificar os eventos utilizando operações bit a bit:

```c
if (estado & EVENTO_INICIAR) {
    printf("Evento de início recebido\n");
}

if (estado & EVENTO_ERRO) {
    printf("Evento de erro recebido\n");
}
```

Esse método permite que vários eventos sejam representados em uma única variável de 32 bits.

---

## Conceitos importantes

### Task Notification

Cada Task possui internamente um valor de notificação que pode ser utilizado para:

- Enviar valores;
- Enviar eventos;
- Definir flags;
- Desbloquear Tasks;
- Implementar contadores;
- Realizar sincronização;
- Controlar máquinas de estados.

---

### Operação bit a bit

Cada bit de uma variável de 32 bits pode representar um evento independente.

```text
Bit 0 → Evento 1
Bit 1 → Evento 2
Bit 2 → Evento 3
Bit 3 → Evento 4
Bit 4 → Evento 5
```

A ação `eSetBits` permite ativar um ou mais desses eventos sem necessariamente apagar os bits que já estavam definidos.

---

### Task bloqueada

A utilização de `portMAX_DELAY` mantém a `Task2` bloqueada até que uma notificação seja recebida.

Enquanto está bloqueada, a Task não consome tempo de CPU.

---

## Diferença entre os exemplos de Task Notification

| Recurso | Funções | Aplicação |
|---------|---------|-----------|
| Notificação como contador | `xTaskNotifyGive()` e `ulTaskNotifyTake()` | Contagem de eventos ou funcionamento semelhante a semáforo contador. |
| Notificação com valores | `xTaskNotify()` e `xTaskNotifyWait()` | Envio de comandos, estados ou valores de 32 bits. |
| Notificação com bits | `xTaskNotify()` com `eSetBits` | Representação simultânea de diferentes eventos ou flags. |

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
| Tipo de notificação | Valores e bits |

---

## Estrutura do projeto

```text
Notification-Maquina-Estado/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```