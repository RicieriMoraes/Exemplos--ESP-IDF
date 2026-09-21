# _High Resolution Timer_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Autor | Descrição |
|--------|-------|-----------|
| 1.0.0 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a utilização do **High Resolution Timer (`esp_timer`)** do ESP-IDF para execução periódica de uma função de callback em intervalos de tempo da ordem de microssegundos.

O firmware configura a `GPIO21` como saída e cria um timer periódico denominado `Timer1`. A cada disparo do timer, a função `disparoTimer1()` inverte o estado lógico da GPIO, produzindo um sinal alternado na saída.

Inicialmente, o timer é configurado com período de **50 µs**. Durante a execução, informações internas e estatísticas relacionadas aos timers são exibidas utilizando `esp_timer_dump()`.

Após aproximadamente 5 segundos, o timer é interrompido através de `esp_timer_stop()`, podendo então ser iniciado novamente com um novo período.

O projeto demonstra conceitos de temporização de alta resolução, callbacks periódicos, manipulação de GPIO e gerenciamento do ciclo de vida de timers no ESP-IDF.

---

## Objetivo

- Demonstrar a utilização do `esp_timer` no ESP-IDF.
- Criar um timer de alta resolução.
- Configurar uma função de callback.
- Criar um timer periódico.
- Trabalhar com períodos especificados em microssegundos.
- Alterar o estado de uma GPIO através do callback.
- Consultar informações dos timers utilizando `esp_timer_dump()`.
- Interromper um timer utilizando `esp_timer_stop()`.
- Reiniciar um timer com um novo período.
- Excluir um timer utilizando `esp_timer_delete()`.
- Diferenciar `esp_timer` de Software Timers do FreeRTOS.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Disponibiliza recursos padrão de entrada e saída, incluindo `stdout`. |
| `esp_timer.h` | Disponibiliza a API de High Resolution Timer do ESP-IDF. |
| `driver/gpio.h` | Disponibiliza funções para configuração e controle das GPIOs. |
| `freertos/FreeRTOS.h` | Contém as definições principais do FreeRTOS. |
| `freertos/task.h` | Disponibiliza funções relacionadas às Tasks, incluindo `vTaskDelay()`. |

---

## Configuração do firmware

| Parâmetro | Configuração |
|-----------|--------------|
| Linguagem | C |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Timer | `esp_timer` |
| Nome | `Timer1` |
| Tipo | Periódico |
| Período inicial | 50 µs |
| Segundo período | 100 µs |
| GPIO | GPIO21 |
| Modo da GPIO | Saída |
| Callback | `disparoTimer1()` |
| Monitoramento | `esp_timer_dump()` |

---

# Funcionamento

O firmware realiza as seguintes etapas:

1. Configura a `GPIO21` como saída.
2. Define a estrutura de configuração do timer.
3. Associa o callback `disparoTimer1()` ao timer.
4. Cria um handle para o timer.
5. Cria o timer através de `esp_timer_create()`.
6. Inicia o timer periódico com período de 50 µs.
7. A cada disparo, o callback inverte o estado da GPIO21.
8. `esp_timer_dump()` apresenta informações dos timers.
9. O dump é executado cinco vezes, com intervalo de 1 segundo.
10. O timer é interrompido através de `esp_timer_stop()`.
11. O timer *pode* ser iniciado novamente com período de 100 µs.
12. O timer é excluído através de `esp_timer_delete()`.

---

# Configuração da GPIO

A GPIO utilizada é configurada através de:

```c
gpio_set_direction(
    GPIO_NUM_21,
    GPIO_MODE_OUTPUT
);
```

Isso define:

```text
GPIO21 → Saída digital
```

A GPIO será controlada pelo callback do timer.

---

# Configuração do Timer

As propriedades do timer são definidas através da estrutura:

```c
esp_timer_create_args_t
```

No projeto:

```c
const esp_timer_create_args_t timer1Conf = {
    .callback = disparoTimer1,
    .name = "Timer1"
};
```

Essa estrutura define as principais características utilizadas na criação do timer.

---

## Estrutura `esp_timer_create_args_t`

Os campos utilizados neste exemplo são:

| Campo | Valor | Finalidade |
|-------|-------|------------|
| `.callback` | `disparoTimer1` | Função executada quando o timer dispara |
| `.name` | `"Timer1"` | Nome utilizado para identificação do timer |

O nome também auxilia na análise das informações apresentadas por:

```c
esp_timer_dump()
```

---

# Handle do Timer

O timer é representado através de:

```c
esp_timer_handle_t timer1Handle;
```

O handle funciona como uma referência ao timer criado.

Após a criação, ele pode ser utilizado para:

```text
Iniciar
   │
   ├── esp_timer_start_periodic()
   │
Parar
   │
   ├── esp_timer_stop()
   │
Reiniciar
   │
   ├── esp_timer_start_periodic()
   │
Excluir
   │
   └── esp_timer_delete()
```

---

# Criação do Timer

A criação é realizada através de:

```c
esp_timer_create(
    &timer1Conf,
    &timer1Handle
);
```

O primeiro argumento aponta para a estrutura contendo as configurações:

```c
&timer1Conf
```

O segundo argumento recebe o handle do timer criado:

```c
&timer1Handle
```

Fluxo:

```text
timer1Conf
     │
     │ configura
     ▼
esp_timer_create()
     │
     ▼
timer1Handle
```

---

# Timer periódico

O timer é iniciado através de:

```c
esp_timer_start_periodic(
    timer1Handle,
    50
);
```

O segundo argumento representa o período em **microssegundos**.

Portanto:

```text
50 µs
```

corresponde a:

```text
0,000050 segundos
```

ou:

```text
0,05 ms
```

---

## Conversão de unidades

| Unidade | Valor |
|---------|------:|
| Microssegundos | 50 µs |
| Milissegundos | 0,05 ms |
| Segundos | 0,00005 s |

A frequência correspondente ao período é:

```text
f = 1 / T
```

Para:

```text
T = 50 µs
```

temos:

```text
T = 50 × 10⁻⁶ s
```

Logo:

```text
f = 1 / (50 × 10⁻⁶)
```

```text
f = 20.000 Hz
```

Portanto, o **callback é solicitado a cada 50 µs**, correspondendo a uma taxa de:

```text
20 kHz
```

> Isso não significa necessariamente que o callback conseguirá executar com precisão perfeita a 20 kHz em todas as condições. O `esp_timer` é um mecanismo de temporização de alta resolução em software e sua execução está sujeita à latência e ao processamento do sistema.

---

# Callback do Timer

A função executada periodicamente é:

```c
void disparoTimer1(void *arg)
{
    static bool status;

    status = !status;

    gpio_set_level(
        GPIO_NUM_21,
        status
    );
}
```

A variável:

```c
static bool status;
```

mantém seu valor entre diferentes chamadas da função.

Como uma variável estática é inicializada com zero quando nenhum valor inicial é especificado:

```text
status inicial = false
```

---

# Inversão do estado

A cada chamada:

```c
status = !status;
```

o valor é invertido.

Exemplo:

```text
Callback 1 → true
Callback 2 → false
Callback 3 → true
Callback 4 → false
Callback 5 → true
...
```

Esse estado é enviado para a GPIO:

```c
gpio_set_level(
    GPIO_NUM_21,
    status
);
```

---

# Forma de onda gerada

Como o estado da GPIO é invertido a cada callback:

```text
Callback:

      50µs       50µs       50µs       50µs
        │          │          │          │
        ▼          ▼          ▼          ▼

GPIO:
        ┌──────────┐          ┌──────────┐
        │          │          │          │
────────┘          └──────────┘          └──────
```

É importante observar que o **período do timer não é igual ao período completo da onda quadrada**.

São necessários dois callbacks para completar um ciclo:

```text
LOW → HIGH → LOW
```

Portanto:

```text
Ttimer = 50 µs
```

e:

```text
Tsinal = 2 × 50 µs
```

```text
Tsinal = 100 µs
```

Logo, idealmente:

```text
fsinal = 1 / 100 µs
```

```text
fsinal = 10 kHz
```

Assim:

| Grandeza | Valor ideal |
|----------|------------:|
| Período do timer | 50 µs |
| Frequência dos callbacks | 20 kHz |
| Período completo da GPIO | 100 µs |
| Frequência da GPIO | 10 kHz |

---

# Monitoramento com `esp_timer_dump()`

O projeto utiliza:

```c
esp_timer_dump(stdout);
```

Essa função permite apresentar informações relacionadas aos timers existentes.

No código:

```c
for (int i = 0; i < 5; i++) {

    esp_timer_dump(stdout);

    vTaskDelay(
        pdMS_TO_TICKS(1000)
    );
}
```

O dump é executado cinco vezes.

Entre cada execução existe um atraso de:

```text
1000 ms = 1 segundo
```

Portanto, o acompanhamento ocorre durante aproximadamente 5 segundos.

---

# `stdout`

A chamada:

```c
esp_timer_dump(stdout);
```

utiliza:

```c
stdout
```

como destino das informações.

`stdout` representa a saída padrão do programa.

No ambiente ESP-IDF, normalmente essas informações poderão ser visualizadas através do terminal/monitor serial.

---

# Interrupção do Timer

Após o laço:

```c
esp_timer_stop(timer1Handle);
```

interrompe o timer periódico.

Fluxo:

```text
Timer ativo
    │
    │ esp_timer_stop()
    ▼
Timer parado
```

O timer continua existindo.

Apenas deixa de realizar novos disparos.

---

# Reinicialização do Timer

Após ser interrompido, o timer pode ser iniciado novamente:

```c
esp_timer_start_periodic(
    timer1Handle,
    100
);
```

Então o período passa a ser:

```text
100 µs
```

A frequência de callbacks correspondente é:

```text
f = 1 / (100 × 10⁻⁶)
```

```text
f = 10 kHz
```

Como a GPIO é invertida em cada callback, a frequência ideal da onda quadrada passa a ser:

```text
5 kHz
```

---

## Comparação dos períodos

| Período do Timer | Taxa de callbacks | Frequência ideal da GPIO |
|-----------------:|------------------:|-------------------------:|
| 50 µs | 20 kHz | 10 kHz |
| 100 µs | 10 kHz | 5 kHz |

---

# Exclusão do Timer

O timer é excluído através de:

```c
esp_timer_delete(timer1Handle);
```

Após essa operação, o handle não deve mais ser utilizado para controlar o timer.

O ciclo de vida conceitual é:

```text
esp_timer_create()
        │
        ▼
      Criado
        │
        ▼
esp_timer_start_periodic()
        │
        ▼
       Ativo
        │
        ▼
esp_timer_stop()
        │
        ▼
      Parado
        │
        ▼
esp_timer_delete()
        │
        ▼
      Excluído
```

---

# Funções utilizadas

| Função | Finalidade |
|--------|------------|
| `gpio_set_direction()` | Configura a direção da GPIO |
| `gpio_set_level()` | Altera o nível lógico da GPIO |
| `esp_timer_create()` | Cria um High Resolution Timer |
| `esp_timer_start_periodic()` | Inicia um timer periódico |
| `esp_timer_dump()` | Exibe informações dos timers |
| `esp_timer_stop()` | Interrompe um timer |
| `esp_timer_delete()` | Exclui o timer |
| `vTaskDelay()` | Suspende temporariamente a Task |
| `pdMS_TO_TICKS()` | Converte milissegundos para ticks |

---

# Fluxo de execução

```text
                 app_main()
                     │
                     ▼
         Configura GPIO21 como saída
                     │
                     ▼
          Configura timer1Conf
                     │
                     ▼
           esp_timer_create()
                     │
                     ▼
              Timer1 criado
                     │
                     ▼
       start_periodic(50 µs)
                     │
                     ▼
        ┌─────────────────────┐
        │ Callback periódico  │
        │ a cada 50 µs        │
        └─────────────────────┘
                     │
                     ▼
             disparoTimer1()
                     │
                     ▼
             Inverte status
                     │
                     ▼
           Alterna GPIO21
                     │
                     │
          Paralelamente
                     │
                     ▼
       esp_timer_dump(stdout)
                     │
                     ▼
             Delay de 1 s
                     │
                     ▼
             Repete 5 vezes
                     │
                     ▼
          esp_timer_stop()
                     │
                     ▼
      start_periodic(100 µs) (linha desativada)
                     │
                     ▼
          esp_timer_delete()
```

---

# `esp_timer` × FreeRTOS Software Timer

Este projeto é diferente do exemplo "Timer" utilizando:

```c
xTimerCreate()
```

No projeto "Timer", o timer era um **Software Timer do FreeRTOS**.

Neste projeto é utilizada a API:

```c
esp_timer
```

do ESP-IDF.

| Característica | `esp_timer` | FreeRTOS Software Timer |
|----------------|-------------|-------------------------|
| API principal | ESP-IDF | FreeRTOS |
| Criação | `esp_timer_create()` | `xTimerCreate()` |
| Início periódico | `esp_timer_start_periodic()` | `xTimerStart()` |
| Unidade do período | µs | ticks |
| Alta resolução | ✔ | Mais limitada pelo tick |
| Callback | ✔ | ✔ |
| One-shot | ✔ | ✔ |
| Periódico | ✔ | ✔ |
| Uso típico | Temporização de maior resolução | Eventos temporizados do RTOS |

---

# `esp_timer` × Hardware Timer

Apesar do nome **High Resolution Timer**, o `esp_timer` não deve ser confundido com um periférico de timer de hardware dedicado utilizado diretamente pela aplicação.

O `esp_timer` fornece uma abstração de temporização de alta resolução gerenciada pelo ESP-IDF.

| Característica | `esp_timer` | Hardware Timer |
|----------------|-------------|----------------|
| Gerenciamento | ESP-IDF | Periférico |
| API | Alto nível | Mais próxima do hardware |
| Unidade típica | µs | Dependente da configuração |
| Callback | ✔ | Normalmente ISR/callback |
| Facilidade de uso | Alta | Menor |
| Precisão para geração de sinais | Limitada por software/latência | Maior |
| Aplicações de tempo crítico | Limitado | Mais indicado |

---

# High Resolution Timer não é PWM

Embora este exemplo alterne uma GPIO periodicamente:

```c
gpio_set_level(GPIO_NUM_21, status);
```

o `esp_timer` **não é um periférico PWM**.

O sinal produzido serve como demonstração da temporização.

Para geração contínua e precisa de sinais periódicos, normalmente é mais adequado utilizar periféricos específicos do ESP32, como:

- LEDC;
- MCPWM;
- GPTimer;
- RMT, dependendo da aplicação.

Esses periféricos reduzem a dependência da execução periódica de código pela CPU.

---

# Callback e tempo de execução

Como o callback pode ser executado frequentemente, é importante mantê-lo curto.

Neste exemplo:

```c
status = !status;
gpio_set_level(GPIO_NUM_21, status);
```

o processamento é simples.

Callbacks muito grandes podem introduzir atrasos.

Deve-se evitar dentro de callbacks de alta frequência:

- processamento pesado;
- loops longos;
- operações bloqueantes;
- delays;
- acesso lento a periféricos;
- grandes quantidades de logs.

---

# Aplicações práticas

O `esp_timer` pode ser utilizado para:

- execução periódica de funções;
- geração de timeouts;
- aquisição periódica de sensores;
- controle de máquinas de estados;
- temporização de protocolos;
- medição de intervalos;
- eventos de curta duração;
- agendamento de tarefas;
- controle temporal de aplicações;
- execução one-shot;
- execução periódica.

Exemplo:

```text
Timer 1000 µs
      │
      ▼
Callback
      │
      ▼
Solicita leitura do sensor
      │
      ▼
Processamento em outra Task
```

---

# Observações

- `esp_timer_start_periodic()` recebe o período em **microssegundos**.
- O timer inicialmente utiliza um período de 50 µs.
- Isso corresponde a uma taxa de callbacks de 20 kHz.
- Como a GPIO muda de estado a cada callback, a onda ideal possui frequência de 10 kHz.
- O callback deve ser mantido curto.
- `esp_timer_stop()` interrompe o timer, mas não o exclui.
- Um timer parado pode ser iniciado novamente.
- `esp_timer_delete()` libera o timer criado.
- `esp_timer_dump()` pode auxiliar no diagnóstico e análise dos timers.
- Para geração precisa de sinais periódicos, deve-se considerar um periférico dedicado em vez de depender de callbacks de software.

---

# Resultado esperado

Durante os primeiros cinco segundos:

```text
Período do Timer = 50 µs
Taxa de callbacks ≈ 20 kHz
GPIO21 ≈ 10 kHz
```

Com a reconfiguração (100 us):

```text
Período do Timer = 100 µs
Taxa de callbacks ≈ 10 kHz
GPIO21 ≈ 5 kHz
```

Os valores reais observados podem apresentar diferenças devido à latência, escalonamento, carga do sistema e limitações inerentes à execução de callbacks em software.

---

# Informações

| Info | Modelo |
|------|--------|
| Família | ESP32 |
| Framework | ESP-IDF |
| Sistema operacional | FreeRTOS |
| Linguagem | C |
| IDE | ESP-IDF v5.4.2 |
| Recurso principal | `esp_timer` |
| Tipo | High Resolution Timer |
| Operação | Periódica |
| Período inicial | 50 µs |
| Segundo período | 100 µs |
| GPIO | GPIO21 |
| Callback | `disparoTimer1()` |
| Diagnóstico | `esp_timer_dump()` |

---

# Estrutura do projeto

```text
High_Resolution_Timer/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```