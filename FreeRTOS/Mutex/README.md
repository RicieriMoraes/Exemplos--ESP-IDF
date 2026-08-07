# _Mutex_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
|--------|------------|--------------|-------------------|
| 1.0.0  | 07/08/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a utilização de um **Mutex (Mutual Exclusion)** do FreeRTOS para controlar o acesso concorrente a um recurso compartilhado. Duas Tasks simulam a leitura de temperatura e umidade, compartilhando um mesmo barramento de comunicação (representado pela função `writeBus()`). O Mutex garante que apenas uma Task utilize esse recurso por vez, evitando conflitos e garantindo a integridade das informações transmitidas.

---

## Objetivo

- Demonstrar a criação de um Mutex no FreeRTOS.
- Controlar o acesso concorrente a um recurso compartilhado.
- Utilizar `xSemaphoreTake()` para solicitar acesso ao recurso.
- Utilizar `xSemaphoreGive()` para liberar o recurso.
- Implementar tratamento de timeout caso o recurso permaneça ocupado.
- Demonstrar uma aplicação prática de exclusão mútua em sistemas embarcados.
- Servir como base para proteção de barramentos compartilhados, como UART, SPI e I²C.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Funções padrão de entrada e saída, como `printf()`. |
| `freertos/FreeRTOS.h` | Definições principais do sistema operacional FreeRTOS. |
| `freertos/task.h` | Criação e gerenciamento de Tasks. |
| `freertos/semphr.h` | Criação e gerenciamento de Semáforos e Mutex. |

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
| Stack de cada Task | 4096 bytes |
| Tipo de sincronização | Mutex |
| Tempo máximo de espera | 1000 ms |

---

## Funcionamento

1. A função `app_main()` cria um Mutex utilizando `xSemaphoreCreateMutex()`.
2. São criadas duas Tasks independentes.
3. Cada Task tenta acessar o mesmo recurso compartilhado (`writeBus()`).
4. Antes de utilizar o recurso, a Task solicita o Mutex utilizando `xSemaphoreTake()`.
5. Caso o Mutex esteja disponível, a Task escreve no barramento e libera o recurso utilizando `xSemaphoreGive()`.
6. Caso o Mutex permaneça ocupado durante o tempo configurado, ocorre um timeout e uma mensagem de erro é exibida.
7. Após a utilização do recurso, cada Task aguarda 1000 ms antes de uma nova tentativa.

---

## Tasks implementadas

| Task | Função | Prioridade | Stack |
|------|--------|------------|-------|
| Task1 | Simulação de leitura de temperatura | 2 | 4096 bytes |
| Task2 | Simulação de leitura de umidade | 2 | 4096 bytes |

---

## Recurso compartilhado

O recurso compartilhado é representado pela função:

```c
void writeBus(char *msg)
{
    printf(msg);
}
```

Embora o exemplo utilize o terminal serial, o mesmo conceito pode ser aplicado a:

- UART
- SPI
- I²C
- CAN (TWAI)
- Memória Flash
- Cartão SD
- Displays
- Arquivos
- Variáveis globais

---

## Criação do Mutex

O Mutex é criado antes da criação das Tasks:

```c
mutexBus = xSemaphoreCreateMutex();
```

O retorno da função é armazenado em um `SemaphoreHandle_t`, utilizado posteriormente pelas Tasks para controlar o acesso ao recurso.

---

## Solicitação do Mutex

Antes de acessar o recurso compartilhado, cada Task executa:

```c
if (xSemaphoreTake(mutexBus, 1000 / portTICK_PERIOD_MS))
{
    writeBus("Mensagem");

    xSemaphoreGive(mutexBus);
}
else
{
    writeBus("Timeout");
}
```

O segundo parâmetro representa o tempo máximo que a Task aguardará pela liberação do Mutex.

Neste projeto:

```text
1000 ms
```

---

## Liberação do Mutex

Após finalizar o uso do recurso, a Task libera o Mutex:

```c
xSemaphoreGive(mutexBus);
```

Isso permite que outra Task obtenha acesso ao barramento compartilhado.

---

## Funções utilizadas

| Função | Finalidade |
|--------|------------|
| `xSemaphoreCreateMutex()` | Cria um Mutex. |
| `xSemaphoreTake()` | Solicita acesso ao recurso protegido. |
| `xSemaphoreGive()` | Libera o recurso protegido. |
| `xTaskCreate()` | Cria uma nova Task. |
| `vTaskDelay()` | Suspende temporariamente a execução da Task. |
| `printf()` | Exibe mensagens no terminal serial. |

---

## Fluxo de execução

```text
Task
 │
 ▼
Solicita Mutex
 │
 ├── Mutex ocupado ─────► Aguarda até 1000 ms
 │
 ▼
Mutex disponível
 │
 ▼
Acessa recurso compartilhado
 │
 ▼
Libera Mutex
 │
 ▼
Delay
```

---

## Exemplo de saída

```text
Realizando leitura de temperatura

Realizando leitura de umidade

Realizando leitura de temperatura

Realizando leitura de umidade
```

Caso uma Task não consiga obter o Mutex dentro do tempo configurado:

```text
Timeout na leitura de temperatura
```

ou

```text
Timeout na leitura de umidade
```

---

## Conceitos importantes

### Mutex

Mutex (**Mutual Exclusion**) é um mecanismo de sincronização utilizado para garantir que apenas uma Task utilize determinado recurso compartilhado por vez.

---

### Recurso compartilhado

Um recurso compartilhado é qualquer elemento utilizado por múltiplas Tasks, como:

- UART
- SPI
- I²C
- CAN
- Displays
- Memória Flash
- Cartão SD
- Variáveis globais
- Arquivos

---

### Timeout

Ao utilizar:

```c
xSemaphoreTake(
    mutexBus,
    1000 / portTICK_PERIOD_MS
);
```

a Task permanece aguardando até **1000 ms** pela liberação do recurso.

Caso esse período expire, a função retorna `pdFALSE`, permitindo tratar a falha de aquisição.

---

## Mutex × Semáforo

| Característica | Mutex | Semáforo |
|---------------|-------|----------|
| Exclusão mútua | ✔ | ✔ |
| Proteção de recursos | ✔ | ✔ |
| Ownership (dono do recurso) | ✔ | ✘ |
| Priority Inheritance | ✔ | ✘ |
| Sincronização entre Tasks | Limitado | ✔ |

O Mutex é a melhor escolha quando o objetivo é proteger recursos compartilhados.

---

## Observações

- Todo `xSemaphoreTake()` bem-sucedido deve possuir um `xSemaphoreGive()` correspondente.
- O Mutex implementa **Priority Inheritance**, reduzindo problemas de inversão de prioridade.
- Não liberar um Mutex impede que outras Tasks utilizem o recurso protegido.
- Neste exemplo, **todo o acesso ao recurso compartilhado ocorre dentro da região crítica**, sendo uma implementação mais adequada para aplicações reais.
- Em aplicações embarcadas, é comum proteger com Mutex recursos como UART, SPI, I²C, arquivos, displays e memória compartilhada.

---

## Informações

| Info | Modelo |
|------|--------|
| Família | ESP32 |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Linguagem | C |
| IDE | ESP-IDF v5.4.2 |
| Sincronização | Mutex |

---

## Estrutura do projeto

```text
Mutex/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```