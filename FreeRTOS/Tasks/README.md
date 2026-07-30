# _Tasks_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
|--------|------------|--------------|-------------------|
| 1.0.0  | 30/07/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a criação e execução de múltiplas tarefas (Tasks) utilizando o sistema operacional de tempo real FreeRTOS no ESP-IDF. São implementadas duas tarefas independentes que executam simultaneamente, simulando a leitura de temperatura e umidade. O exemplo apresenta o funcionamento do escalonador (Scheduler), a passagem de parâmetros para as tarefas e a utilização de atrasos temporizados para compartilhamento do tempo de processamento.

---

## Objetivo

- Demonstrar a criação de múltiplas Tasks utilizando o FreeRTOS.
- Compreender o funcionamento do escalonador de tarefas (Scheduler).
- Demonstrar a passagem de parâmetros para uma Task.
- Configurar prioridade e memória de cada Task.
- Utilizar `vTaskDelay()` para compartilhamento do tempo de CPU.
- Servir como base para aplicações multitarefa utilizando ESP-IDF.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Funções padrão de entrada e saída da linguagem C. |
| `freertos/FreeRTOS.h` | Definições principais do sistema operacional FreeRTOS. |
| `freertos/task.h` | Criação, gerenciamento e controle de tarefas (Tasks). |

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
| Stack de cada Task | 2048 Bytes |
| Handler | `NULL` |
| Tempo entre execuções | 1000 ms |

---

## Funcionamento

1. A função `app_main()` cria duas Tasks utilizando `xTaskCreate()`.
2. A **Task1** simula a leitura de temperatura.
3. A **Task2** simula a leitura de umidade.
4. Cada Task recebe um parâmetro durante sua criação.
5. O parâmetro recebido é recuperado através do ponteiro `void *param`.
6. Ambas executam continuamente em um laço infinito (`while(1)`).
7. Após cada execução, a Task entra em espera por 1000 ms utilizando `vTaskDelay()`, permitindo que outras tarefas sejam executadas.

---

## Tasks implementadas

| Task | Função | Prioridade | Stack |
|------|---------|------------|-------|
| Task1 | Simulação de leitura de temperatura | 2 | 2048 Bytes |
| Task2 | Simulação de leitura de umidade | 2 | 2048 Bytes |

---

## Funções utilizadas

| Função | Finalidade |
|--------|------------|
| `xTaskCreate()` | Cria uma nova Task no FreeRTOS. |
| `vTaskDelay()` | Suspende temporariamente a execução da Task. |
| `printf()` | Exibe mensagens no terminal serial. |

---

## Estrutura da função `xTaskCreate()`

```c
xTaskCreate(
    TaskFunction,      // Ponteiro para a função da Task
    "Nome da Task",    // Nome da Task
    StackSize,         // Memória da pilha (Bytes)
    Parametro,         // Parâmetro passado para a Task
    Prioridade,        // Prioridade da Task
    Handler            // Manipulador da Task
);
```

### Parâmetros utilizados neste projeto

| Parâmetro | Valor |
|-----------|-------|
| Nome | "Leitura de temperatura" / "Leitura de umidade" |
| Stack | 2048 Bytes |
| Prioridade | 2 |
| Handler | `NULL` |
| Parâmetro | `"Task 1"` e `"Task 2"` |

---

## Exemplo de saída

```text
Realizando leitura de temperatura Task 1
Realizando leitura de umidade Task 2
Realizando leitura de temperatura Task 1
Realizando leitura de umidade Task 2
...
```

A ordem das mensagens pode variar durante a execução, pois o escalonador do FreeRTOS decide qual Task será executada em cada instante.

---

## Observações

- Cada Task possui sua própria pilha (Stack), área de memória utilizada para armazenamento de variáveis locais e contexto de execução.
- As duas Tasks possuem a mesma prioridade (**2**). Dessa forma, o Scheduler distribui o tempo de processamento entre ambas.
- O parâmetro recebido em `void *param` pode representar qualquer tipo de dado, sendo necessário realizar o cast apropriado antes de sua utilização.
- A chamada de `vTaskDelay()` é fundamental para liberar tempo de processamento para outras Tasks e evitar o consumo desnecessário da CPU.
- Caso uma Task possua prioridade superior às demais e nunca entre em espera, ela poderá monopolizar o processador.

---

## Conceitos importantes

### Task

Uma Task é uma unidade independente de execução, semelhante a uma thread, executada pelo FreeRTOS.

---

### Scheduler

É o escalonador responsável por decidir qual Task será executada em cada momento, considerando suas prioridades e estados.

---

### Prioridade

Cada Task possui uma prioridade numérica.

- Quanto maior o valor, maior a prioridade.
- Tasks de mesma prioridade compartilham o tempo de processamento.

---

### Stack

Cada Task possui sua própria pilha de memória para armazenamento de:

- Variáveis locais;
- Endereço de retorno;
- Contexto do processador;
- Chamadas de funções.

---

## Informações

| Info | Modelo |
|------|--------|
| uC | ESP32-C3 |
| Placa | ESP32-C3 DevKitM-1 |
| Arquitetura | RISC-V (32 bits) |
| IDE | ESP-IDF v5.4.2 |

---

## Estrutura do projeto

```text
Tasks/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```
````
