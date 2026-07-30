# _Core_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
|--------|------------|--------------|-------------------|
| 1.0.0  | 30/07/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a criação e execução de múltiplas tarefas utilizando o sistema operacional de tempo real FreeRTOS no ESP-IDF.

São implementadas duas Tasks independentes, responsáveis por simular leituras de temperatura e umidade. A primeira Task é criada utilizando a função convencional `xTaskCreate()`, permitindo que o escalonador determine o núcleo de execução. A segunda Task é criada utilizando `xTaskCreatePinnedToCore()`, permitindo selecionar explicitamente o núcleo no qual ela será executada.

---

## Objetivo

- Demonstrar a criação de múltiplas Tasks utilizando o FreeRTOS.
- Compreender o funcionamento básico do escalonador de tarefas.
- Demonstrar a passagem de parâmetros para uma Task.
- Configurar prioridade e memória de cada Task.
- Fixar uma Task em um núcleo específico do microcontrolador.
- Comparar `xTaskCreate()` com `xTaskCreatePinnedToCore()`.
- Utilizar `vTaskDelay()` para liberar tempo de processamento.
- Servir como base para aplicações multitarefa em microcontroladores ESP32 multicore.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Disponibiliza funções padrão de entrada e saída, como `printf()`. |
| `freertos/FreeRTOS.h` | Disponibiliza as definições principais do sistema operacional FreeRTOS. |
| `freertos/task.h` | Disponibiliza funções para criação, gerenciamento e controle de Tasks. |

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
| Handler das Tasks | `NULL` |
| Intervalo entre execuções | 1000 ms |
| Core selecionado para a Task2 | Core 0 |

---

## Funcionamento

1. A função `app_main()` cria duas Tasks.
2. A `Task1` simula a leitura de temperatura.
3. A `Task2` simula a leitura de umidade.
4. A `Task1` é criada utilizando `xTaskCreate()`.
5. A `Task2` é criada utilizando `xTaskCreatePinnedToCore()`.
6. A `Task2` é fixada para execução no Core 0.
7. Cada Task recebe uma string como parâmetro.
8. As Tasks executam continuamente em um laço infinito.
9. Após cada execução, a Task entra em estado de espera durante 1000 ms.

---

## Tasks implementadas

| Task | Função | Prioridade | Stack | Core |
|------|--------|------------|-------|------|
| Task1 | Simulação de leitura de temperatura | 2 | 2048 bytes | Definido pelo Scheduler |
| Task2 | Simulação de leitura de umidade | 2 | 2048 bytes | Core 0 |

---

## Criação da Task1

A primeira Task é criada utilizando a função convencional `xTaskCreate()`:

```c
xTaskCreate(
    &Task1,
    "Leitura de temperatura",
    2048,
    "Task 1",
    2,
    NULL
);
```

Nesse caso, a Task não é fixada em um núcleo específico. O escalonador do FreeRTOS gerencia sua execução de acordo com a disponibilidade dos núcleos e as configurações do sistema.

---

## Criação da Task2

A segunda Task é criada utilizando `xTaskCreatePinnedToCore()`:

```c
xTaskCreatePinnedToCore(
    &Task2,
    "Leitura de umidade",
    2048,
    "Task 2",
    2,
    NULL,
    0
);
```

O último parâmetro define o núcleo no qual a Task será executada.

Neste projeto:

```text
Core selecionado: 0
```

---

## Estrutura da função `xTaskCreate()`

```c
xTaskCreate(
    TaskFunction,      // Função executada pela Task
    "TaskName",        // Nome da Task
    StackSize,         // Tamanho da pilha
    Parameter,         // Parâmetro enviado para a Task
    Priority,          // Prioridade
    TaskHandle         // Handler da Task
);
```

---

## Estrutura da função `xTaskCreatePinnedToCore()`

```c
xTaskCreatePinnedToCore(
    TaskFunction,      // Função executada pela Task
    "TaskName",        // Nome da Task
    StackSize,         // Tamanho da pilha
    Parameter,         // Parâmetro enviado para a Task
    Priority,          // Prioridade
    TaskHandle,        // Handler da Task
    CoreID             // Núcleo selecionado
);
```

---

## Parâmetros utilizados

| Parâmetro | Task1 | Task2 |
|-----------|-------|-------|
| Função | `Task1` | `Task2` |
| Nome | `Leitura de temperatura` | `Leitura de umidade` |
| Stack | 2048 bytes | 2048 bytes |
| Parâmetro | `"Task 1"` | `"Task 2"` |
| Prioridade | 2 | 2 |
| Handler | `NULL` | `NULL` |
| Core | Não especificado | Core 0 |

---

## Funções utilizadas

| Função | Finalidade |
|--------|------------|
| `xTaskCreate()` | Cria uma Task sem fixá-la explicitamente em um núcleo. |
| `xTaskCreatePinnedToCore()` | Cria uma Task e fixa sua execução em um núcleo específico. |
| `vTaskDelay()` | Suspende temporariamente a Task, liberando o processador. |
| `printf()` | Exibe informações no terminal serial. |

---

## Exemplo de saída

```text
Realizando leitura de temperatura Task 1
Realizando leitura de umidade Task 2
Realizando leitura de temperatura Task 1
Realizando leitura de umidade Task 2
...
```

A ordem das mensagens pode variar, pois depende do escalonamento realizado pelo FreeRTOS.

---

## Conceitos importantes

### Task

Uma Task é uma unidade independente de execução gerenciada pelo FreeRTOS, semelhante a uma thread em sistemas operacionais convencionais.

### Scheduler

O Scheduler é responsável por determinar qual Task será executada em cada instante, considerando:

- Prioridade;
- Estado da Task;
- Disponibilidade do processador;
- Núcleo selecionado;
- Tempo de bloqueio ou espera.

### Afinidade de núcleo

A afinidade de núcleo permite determinar em qual Core uma Task deve executar.

Ao utilizar `xTaskCreatePinnedToCore()`, a Task permanece associada ao Core informado durante sua criação.

### Prioridade

As duas Tasks possuem prioridade 2. Como possuem a mesma prioridade, o Scheduler distribui o tempo de processamento entre elas, respeitando os períodos de espera definidos por `vTaskDelay()`.

### Stack

Cada Task possui sua própria pilha de memória, utilizada para armazenar:

- Variáveis locais;
- Parâmetros;
- Endereços de retorno;
- Contexto da CPU;
- Chamadas de funções.

---

## Observações

- O último parâmetro de `xTaskCreatePinnedToCore()` representa o número do Core.
- Em microcontroladores ESP32 com dois núcleos, normalmente são utilizados:
  - `0` para o Core 0;
  - `1` para o Core 1.
- A função `xTaskCreate()` não fixa explicitamente a Task em um Core.
- A função `xTaskCreatePinnedToCore()` permite controlar a afinidade da Task.
- Fixar Tasks em núcleos específicos pode ser útil para separar processamento crítico, comunicação, interface e aquisição de dados.
- O uso incorreto da afinidade pode prejudicar o balanceamento de carga entre os núcleos.
- O `vTaskDelay()` impede que as Tasks permaneçam executando continuamente e consumam todo o tempo disponível da CPU.

> **Importante:** o ESP32-C3 possui apenas um núcleo. Nesse microcontrolador, somente o Core 0 está disponível. Para demonstrar a distribuição real de Tasks entre Core 0 e Core 1, é necessário utilizar um modelo dual-core, como determinados modelos do ESP32 original ou ESP32-S3.

---

## Informações

| Info | Modelo |
|------|--------|
| Família | ESP32 |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Linguagem | C |
| IDE | ESP-IDF v5.4.2 |
| Compatibilidade | Single-core e dual-core |
| Core utilizado pela Task2 | Core 0 |

---

## Estrutura do projeto

```text
Core/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```