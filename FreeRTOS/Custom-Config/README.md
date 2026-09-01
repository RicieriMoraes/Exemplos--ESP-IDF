# _Custom Config_

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
|--------|------------|--------------|-------------------|
| 1.0.0  | 01/09/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a criação e utilização de **configurações personalizadas no ESP-IDF** através do sistema **Kconfig**.

O arquivo `Kconfig.projbuild` adiciona um menu específico ao `menuconfig`, permitindo configurar parâmetros do firmware sem a necessidade de alterar diretamente o código-fonte.

Como exemplo, foi criada uma configuração para o conversor ADC externo **ADS1115**, permitindo definir sua habilitação, TAG de identificação, pinos SDA e SCL e uma opção de taxa de amostragem.

As configurações selecionadas são automaticamente disponibilizadas no firmware através de macros `CONFIG_*`, que podem ser utilizadas durante a compilação e execução da aplicação.

---

## Objetivo

- Demonstrar a criação de configurações personalizadas no ESP-IDF.
- Criar um menu próprio utilizando `Kconfig.projbuild`.
- Utilizar configurações do tipo `bool`, `string`, `int` e `choice`.
- Definir valores padrão através de `default`.
- Limitar valores numéricos através de `range`.
- Adicionar informações através de `help`.
- Utilizar configurações geradas pelo Kconfig no código C.
- Demonstrar o uso das macros `CONFIG_*`.
- Utilizar diretivas de pré-processamento como `#ifdef` e `#elif`.
- Centralizar parâmetros de hardware e firmware no `menuconfig`.

---

## Bibliotecas utilizadas

| Biblioteca | Finalidade |
|------------|------------|
| `stdio.h` | Disponibiliza funções padrão da linguagem C. |
| `esp_log.h` | Disponibiliza o sistema de logs do ESP-IDF. |
| `stdbool.h` | Disponibiliza o tipo booleano `bool`. |

---

## Arquivos principais

O projeto utiliza dois arquivos principais:

```text
main.c
Kconfig.projbuild
```

### `main.c`

Responsável por utilizar as configurações definidas pelo usuário.

### `Kconfig.projbuild`

Responsável por criar as opções personalizadas que serão exibidas no sistema de configuração do ESP-IDF.

---

## Funcionamento

1. O arquivo `Kconfig.projbuild` define um menu chamado `AD1115`.
2. O menu é incorporado ao sistema de configuração do ESP-IDF.
3. O usuário acessa as opções através do `menuconfig`.
4. Os valores selecionados são armazenados na configuração do projeto.
5. O ESP-IDF gera macros com prefixo `CONFIG_`.
6. O `main.c` utiliza essas macros para configurar o comportamento do firmware.
7. As configurações selecionadas são exibidas através de `ESP_LOGI()`.

---

## Acessando o menu de configuração

O sistema de configuração pode ser aberto através do terminal:

```bash
idf.py menuconfig
```

Também é possível acessar o `menuconfig` através da extensão ESP-IDF no Visual Studio Code.

Após abrir o menu, estará disponível a configuração personalizada:

```text
AD1115
```

Dentro dela serão apresentadas as opções definidas no arquivo `Kconfig.projbuild`.

---

## Menu personalizado

O menu principal é criado através de:

```text
menu "AD1115"
```

e finalizado com:

```text
endmenu
```

Tudo que estiver entre essas duas declarações será apresentado dentro desse menu.

Estrutura conceitual:

```text
ESP-IDF menuconfig
│
└── AD1115
    │
    ├── Ativar o ADS1115
    ├── TAG
    ├── SDA
    ├── SCL
    └── Selecione a taxa de amostragem
```

---

## Configurações implementadas

| Configuração | Tipo | Valor padrão | Finalidade |
|--------------|------|--------------|------------|
| `ADS1115_ENABLE` | `bool` | Desabilitado | Habilita ou desabilita o ADS1115 |
| `ADS1115_TAG` | `string` | `"ADS1115_1"` | Define a TAG utilizada nos logs |
| `ADS1115_SDA` | `int` | `20` | Define a GPIO utilizada para SDA |
| `ADS1115_SCL` | `int` | `20` | Define a GPIO utilizada para SCL |
| `ADS1115_SAMPLE_RATE` | `choice` | `OP_1` | Permite selecionar uma opção de taxa de amostragem |

---

## Configuração booleana

A habilitação do ADS1115 é criada através de:

```text
config ADS1115_ENABLE
    bool "Ativar o ADS1115"
```

Essa opção aparece no `menuconfig` como uma seleção:

```text
[*] Ativar o ADS1115
```

Quando habilitada, o ESP-IDF disponibiliza:

```c
CONFIG_ADS1115_ENABLE
```

---

## Verificação de configuração booleana

No firmware, a configuração é verificada através do pré-processador:

```c
bool ADS1115_ENABLE = 0;

#ifdef CONFIG_ADS1115_ENABLE
    ADS1115_ENABLE = 1;
#else
    ADS1115_ENABLE = 0;
#endif
```

Posteriormente, seu estado é exibido:

```c
ESP_LOGI(
    CONFIG_ADS1115_TAG,
    "ADS 1115 STATUS: %s",
    ADS1115_ENABLE ? "yes" : "no"
);
```

Exemplo:

```text
ADS 1115 STATUS: yes
```

---

## Configuração de String

A TAG utilizada pelo sistema de logs é configurada através de:

```text
config ADS1115_TAG
    string "TAG"
    default "ADS1115_1"
```

O valor fica disponível no firmware através de:

```c
CONFIG_ADS1115_TAG
```

Podendo ser utilizado diretamente:

```c
ESP_LOGI(
    CONFIG_ADS1115_TAG,
    "SDA PIN: %d",
    CONFIG_ADS1115_SDA
);
```

Isso permite alterar a TAG pelo `menuconfig` sem modificar o código C.

---

## Configurações numéricas

Os pinos SDA e SCL são configurados como valores inteiros.

### SDA

```text
config ADS1115_SDA
    int "SDA"
    default 20
    range 10 21
```

### SCL

```text
config ADS1115_SCL
    int "SCL"
    default 20
    range 10 21
```

Os valores são acessados no firmware através de:

```c
CONFIG_ADS1115_SDA
```

e:

```c
CONFIG_ADS1115_SCL
```

---

## Limitação através de `range`

A instrução:

```text
range 10 21
```

limita os valores aceitos pelo `menuconfig`.

Nesse caso:

```text
Valor mínimo: 10
Valor máximo: 21
```

Isso ajuda a impedir configurações fora da faixa definida pelo projeto.

---

## Utilização de `help`

O Kconfig permite adicionar informações explicativas:

```text
help
    Configuração de IO SDA para ADS1115
```

O conteúdo pode ser consultado dentro do próprio sistema de configuração, auxiliando na documentação das opções disponíveis.

---

## Configuração utilizando `choice`

Para selecionar uma opção entre várias alternativas foi utilizado:

```text
choice
```

Exemplo:

```text
config ADS1115_SAMPLE_RATE
    choice name
        bool "Selecione a taxa de amostragem"
        default OP_1
```

As opções disponíveis são:

| Configuração | Valor apresentado |
|--------------|-------------------:|
| `OP_1` | 60 |
| `OP_2` | 120 |
| `OP_3` | 240 |
| `OP_4` | 480 |

Apenas uma das opções pode ser selecionada por vez.

---

## Macros geradas

Dependendo da configuração selecionada, o ESP-IDF disponibiliza macros como:

```c
CONFIG_OP_1
CONFIG_OP_2
CONFIG_OP_3
CONFIG_OP_4
```

O firmware identifica a opção selecionada utilizando:

```c
#ifdef CONFIG_OP_1
    OP_SAMPLER = 1;
#elif CONFIG_OP_2
    OP_SAMPLER = 2;
#elif CONFIG_OP_3
    OP_SAMPLER = 3;
#else
    OP_SAMPLER = 4;
#endif
```

Posteriormente:

```c
ESP_LOGI(
    CONFIG_ADS1115_TAG,
    "SAMPLE RATE OPTION %d",
    OP_SAMPLER
);
```

---

## Macros `CONFIG_*`

As configurações definidas no Kconfig são disponibilizadas no firmware utilizando o prefixo:

```text
CONFIG_
```

Exemplo:

```text
Kconfig                    Firmware

ADS1115_ENABLE      →      CONFIG_ADS1115_ENABLE
ADS1115_TAG         →      CONFIG_ADS1115_TAG
ADS1115_SDA         →      CONFIG_ADS1115_SDA
ADS1115_SCL         →      CONFIG_ADS1115_SCL
OP_1                →      CONFIG_OP_1
OP_2                →      CONFIG_OP_2
OP_3                →      CONFIG_OP_3
OP_4                →      CONFIG_OP_4
```

---

## Tipos de configuração demonstrados

| Tipo | Exemplo | Aplicação |
|------|---------|-----------|
| `bool` | `ADS1115_ENABLE` | Habilitar/desabilitar recurso |
| `string` | `ADS1115_TAG` | Textos e identificadores |
| `int` | `ADS1115_SDA` | Valores numéricos |
| `range` | `10 21` | Limitar valores permitidos |
| `choice` | `OP_1...OP_4` | Selecionar uma alternativa |
| `default` | `20` | Definir valor padrão |
| `help` | Texto explicativo | Documentar uma configuração |

---

## Exemplo de configuração

Considerando a seguinte configuração:

```text
ADS1115_ENABLE = habilitado
TAG = ADS1115_1
SDA = 20
SCL = 21
Sample Rate = OP_1
```

o firmware poderá gerar uma saída semelhante a:

```text
I (...) ADS1115_1: SDA PIN: 20
I (...) ADS1115_1: SCL PIN: 21
I (...) ADS1115_1: ADS 1115 STATUS: yes
I (...) ADS1115_1: SAMPLE RATE OPTION 1
```

---

## Fluxo de configuração

```text
          Kconfig.projbuild
                 │
                 ▼
         ESP-IDF menuconfig
                 │
                 ▼
        Usuário seleciona
          configurações
                 │
                 ▼
             sdkconfig
                 │
                 ▼
          Macros CONFIG_*
                 │
                 ▼
              main.c
                 │
                 ▼
       Comportamento definido
          em compilação
```

---

## Kconfig × sdkconfig

O arquivo:

```text
Kconfig.projbuild
```

define **quais configurações existem** e como elas aparecem no menu.

Já o arquivo:

```text
sdkconfig
```

armazena os **valores selecionados** para o projeto.

Exemplo conceitual:

```text
Kconfig.projbuild
       │
       ▼
"Qual GPIO será SDA?"
       │
       ▼
menuconfig
       │
       ▼
Usuário seleciona GPIO20
       │
       ▼
sdkconfig
       │
       ▼
CONFIG_ADS1115_SDA=20
```

---

## Conceitos importantes

### Kconfig

Kconfig é o sistema utilizado pelo ESP-IDF para definir opções configuráveis de um projeto.

Ele permite criar menus e parâmetros que posteriormente são transformados em configurações utilizadas pelo firmware.

---

### `Kconfig.projbuild`

O arquivo `Kconfig.projbuild` permite adicionar opções específicas do projeto ao sistema de configuração do ESP-IDF.

Dessa forma, parâmetros que anteriormente poderiam estar fixos no código:

```c
#define SDA_PIN 20
#define SCL_PIN 21
```

podem ser configurados externamente:

```c
CONFIG_ADS1115_SDA
CONFIG_ADS1115_SCL
```

---

### Configuração em tempo de compilação

As opções do Kconfig são utilizadas principalmente como configurações de **build/compilação**.

Isso significa que alterar uma configuração normalmente exige recompilar o firmware para que o novo valor seja utilizado.

---

## Aplicações práticas

Custom Configs podem ser utilizadas para configurar:

- GPIOs;
- Endereços I²C;
- Frequências;
- Baud rate;
- Habilitação de periféricos;
- Recursos opcionais;
- Níveis de log;
- Modos de funcionamento;
- Intervalos de aquisição;
- Configurações de sensores;
- Protocolos de comunicação;
- Identificação de dispositivos;
- Recursos de debug;
- Variantes de hardware.

Por exemplo:

```text
Hardware versão A
├── SDA = GPIO20
├── SCL = GPIO21
└── Sensor habilitado

Hardware versão B
├── SDA = GPIO8
├── SCL = GPIO9
└── Sensor desabilitado
```

O mesmo código-fonte pode ser utilizado com configurações diferentes.

---

## Observações

- As configurações criadas pelo Kconfig recebem automaticamente o prefixo `CONFIG_` no código C.
- Configurações booleanas desabilitadas podem não gerar uma macro definida, justificando o uso de `#ifdef`.
- `choice` garante que apenas uma opção do grupo seja selecionada.
- `range` permite restringir valores numéricos.
- `default` define o valor utilizado inicialmente.
- `help` permite documentar cada configuração diretamente no `menuconfig`.
- O ADS1115 é utilizado neste projeto apenas como exemplo para demonstrar o sistema de configuração personalizada.

---

## Informações

| Info | Modelo |
|------|--------|
| Família | ESP32 |
| Framework | ESP-IDF |
| Linguagem | C |
| IDE | ESP-IDF v5.4.2 |
| Sistema de configuração | Kconfig |
| Arquivo de configuração | `Kconfig.projbuild` |
| Interface | `menuconfig` |
| Exemplo utilizado | ADS1115 |

---

## Estrutura do projeto

```text
Custom-Config/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    ├── Kconfig.projbuild
    └── main.c
```