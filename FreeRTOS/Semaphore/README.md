# _Semaphore_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
|--------|------------|--------------|-------------------|
| 1.0.0  | 10/08/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a utilização de um **Semáforo Binário (Binary Semaphore)** no FreeRTOS para sincronização entre duas Tasks.

A `comTask` simula o recebimento de um novo dado e libera o semáforo utilizando `xSemaphoreGive()`. A `procTask` permanece bloqueada aguardando esse evento através de `xSemaphoreTake()`. Quando o semáforo é liberado, a Task de processamento é desbloqueada e inicia sua execução.

O exemplo representa um cenário comum em sistemas embarcados no qual uma tarefa de comunicação, sensor ou interrupção sinaliza para outra tarefa que existe um novo dado disponível para processamento.

---

## Objetivo

- Demonstrar a criação de um Semáforo Binário no FreeRTOS.
- Sincronizar a execução de duas Tasks.
- Utilizar `xSemaphoreGive()` para sinalizar a ocorrência de um evento.
- Utilizar `xSemaphoreTake()` para aguardar um evento.
- Demonstrar o bloqueio de uma Task utilizando `portMAX_DELAY`.
- Separar aquisição/comunicação e processamento em Tasks independentes.
- Servir como base para aplicações orientadas a eventos no ESP-IDF.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Disponibiliza funções padrão de entrada e saída, como `printf()`. |
| `freertos/FreeRTOS.h` | Contém as definições principais do sistema operacional FreeRTOS. |
| `freertos/task.h` | Disponibiliza funções para criação e gerenciamento de Tasks. |
| `freertos/semphr.h` | Disponibiliza funções para criação e gerenciamento de Semáforos e Mutex. |

---

## Configuração do firmware

| Parâmetro | Configuração |
|-----------|--------------|
| Linguagem | C |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Função principal | `app_main()` |
| Número de Tasks | 2 |
| Tipo de sincronização | Semáforo Binário |
| Stack da `comTask` | 2048 bytes |
| Stack da `procTask` | 2048 bytes |
| Prioridade da `comTask` | 2 |
| Prioridade da `procTask` | 1 |
| Intervalo entre eventos | 5000 ms |
| Tempo de espera da `procTask` | Indefinido (`portMAX_DELAY`) |

---

## Funcionamento

1. A função `app_main()` cria um Semáforo Binário utilizando `xSemaphoreCreateBinary()`.
2. Em seguida, são criadas duas Tasks:
   - `comTask`;
   - `procTask`.
3. A `procTask` executa `xSemaphoreTake()` e permanece bloqueada enquanto o semáforo não estiver disponível.
4. A `comTask` simula o recebimento de um dado.
5. Após o recebimento, a `comTask` libera o semáforo através de `xSemaphoreGive()`.
6. A `procTask` é desbloqueada e inicia o processamento.
7. A `comTask` aguarda 5000 ms antes de gerar um novo evento.
8. O processo é repetido continuamente.

---

## Tasks implementadas

| Task | Função | Prioridade | Stack |
|------|--------|------------|-------|
| `comTask` | Simula o recebimento de dados e sinaliza um novo evento | 2 | 2048 bytes |
| `procTask` | Aguarda o evento e inicia o processamento | 1 | 2048 bytes |

---

## Criação do Semáforo

O Semáforo Binário é criado utilizando:

```c
semaforo = xSemaphoreCreateBinary();
```

O retorno da função é armazenado em uma variável do tipo:

```c
SemaphoreHandle_t semaforo;
```

Esse handle é utilizado pelas duas Tasks para acessar o mesmo objeto de sincronização.

---

## Envio do evento

A `comTask` sinaliza a ocorrência de um novo evento através da função:

```c
xSemaphoreGive(semaforo);
```

Nesse momento, o semáforo passa para o estado disponível e uma Task que esteja aguardando por ele pode ser desbloqueada.

---

## Recebimento do evento

A `procTask` aguarda o semáforo utilizando:

```c
xSemaphoreTake(semaforo, portMAX_DELAY);
```

Como foi utilizado:

```c
portMAX_DELAY
```

a Task permanece bloqueada indefinidamente enquanto o evento não ocorrer.

Durante esse período, ela não permanece executando continuamente e não ocupa desnecessariamente o tempo da CPU.

---

## Funções utilizadas

| Função | Finalidade |
|--------|------------|
| `xSemaphoreCreateBinary()` | Cria um Semáforo Binário. |
| `xSemaphoreGive()` | Libera ou sinaliza o semáforo. |
| `xSemaphoreTake()` | Aguarda e consome o semáforo. |
| `xTaskCreate()` | Cria uma nova Task. |
| `vTaskDelay()` | Suspende temporariamente uma Task. |
| `printf()` | Exibe mensagens no terminal serial. |

---

## Fluxo de execução

```text
         comTask
            │
            ▼
     Dado recebido
            │
            ▼
xSemaphoreGive(semaforo)
            │
            ├───────────────────────┐
            │                       │
            ▼                       ▼
Novo processamento            procTask desbloqueada
            │                       │
            ▼                       ▼
      Delay de 5 s          Iniciando processamento
                                    │
                                    ▼
                         Aguarda próximo semáforo
```

---

## Exemplo de saída

```text
Dado recebido
Novo processamento
Iniciando processamento

Dado recebido
Novo processamento
Iniciando processamento

Dado recebido
Novo processamento
Iniciando processamento
```

A ordem exata das mensagens após `xSemaphoreGive()` pode variar, pois a execução depende do escalonamento realizado pelo FreeRTOS.

---

## Conceitos importantes

### Semáforo Binário

Um Semáforo Binário pode possuir apenas dois estados:

```text
Disponível
ou
Indisponível
```

Ele é bastante utilizado para sinalizar a ocorrência de eventos entre Tasks ou entre interrupções e Tasks.

---

### Sincronização entre Tasks

Neste projeto, o semáforo não está protegendo um recurso compartilhado. Ele está sendo utilizado para **sincronizar a execução** das Tasks.

A lógica pode ser interpretada como:

```text
comTask → "Existe um novo dado"
procTask → "Recebi o aviso, vou processar"
```

---

### Task bloqueada

Enquanto a `procTask` executa:

```c
xSemaphoreTake(semaforo, portMAX_DELAY);
```

ela permanece no estado **Blocked**.

Nesse estado, o Scheduler pode utilizar o processador para executar outras Tasks.

---

### Prioridades

A `comTask` possui prioridade:

```text
2
```

A `procTask` possui prioridade:

```text
1
```

Portanto, quando ambas estiverem prontas para execução, a `comTask` possui prioridade superior.

Entretanto, normalmente a `procTask` permanece bloqueada aguardando o semáforo.

---

## Semáforo × Mutex

Apesar de ambos utilizarem funções da biblioteca `semphr.h`, possuem finalidades diferentes.

| Característica | Semáforo Binário | Mutex |
|---------------|------------------|-------|
| Sincronização de eventos | ✔ | Não recomendado |
| Proteção de recurso compartilhado | Possível, mas não ideal | ✔ |
| Ownership | ✘ | ✔ |
| Priority Inheritance | ✘ | ✔ |
| Sinalização entre Tasks | ✔ | ✘ |
| Uso típico | Eventos e sincronização | Exclusão mútua |

Neste projeto, o **Semáforo Binário** é a escolha adequada porque o objetivo é sinalizar um evento entre Tasks.

---

## Aplicações práticas

O mesmo princípio pode ser utilizado em situações como:

- Nova mensagem recebida pela UART;
- Novo pacote recebido pela rede;
- Conversão ADC concluída;
- Dado de sensor disponível;
- Interrupção de GPIO;
- Leitura de encoder;
- Pacote CAN recebido;
- Operação DMA finalizada;
- Buffer preenchido;
- Evento externo detectado.

Exemplo conceitual:

```text
Sensor / Comunicação
        │
        ▼
Novo dado disponível
        │
        ▼
  Libera semáforo
        │
        ▼
Task de processamento
```

---

## Observações

- Um Semáforo Binário criado por `xSemaphoreCreateBinary()` inicia indisponível.
- A `procTask` somente continuará sua execução após a ocorrência de um `xSemaphoreGive()`.
- Como o semáforo é binário, ele não funciona como um contador ilimitado de eventos.
- Caso vários `xSemaphoreGive()` ocorram antes que o semáforo seja consumido, os eventos adicionais não são acumulados como ocorreria em um Semáforo Contador.
- Para contar múltiplos eventos pendentes, deve-se utilizar `xSemaphoreCreateCounting()`.
- Para proteger um recurso compartilhado, normalmente deve-se utilizar um **Mutex**, e não um Semáforo Binário.

---

## Informações

| Info | Modelo |
|------|--------|
| Família | ESP32 |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Linguagem | C |
| IDE | ESP-IDF v5.4.2 |
| Sincronização | Semáforo Binário |

---

## Estrutura do projeto

```text
Semaphore/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```