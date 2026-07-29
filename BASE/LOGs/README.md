# *LOGs*

![Firmware version](https://img.shields.io/badge/Firmware_version-1.0.0-blue)

---

## Histórico de versão

| Versão | Data       | Autor        | Descrição         |
| ------ | ---------- | ------------ | ----------------- |
| 1.0.0  | 29/07/2026 | Ricieri Juan | Início do projeto |

---

## Resumo

Este projeto demonstra a utilização do sistema de logs nativo do ESP-IDF por meio da biblioteca `esp_log.h`. São apresentados todos os níveis de log disponíveis (Erro, Warning, Informação, Debug e Verbose), além da configuração dinâmica do nível de exibição para uma TAG específica, permitindo controlar quais mensagens serão exibidas durante a execução da aplicação.

---

## Objetivo

* Demonstrar a utilização do sistema de logs do ESP-IDF.
* Apresentar todos os níveis de prioridade de logs disponíveis.
* Utilizar TAGs para identificação das mensagens.
* Configurar dinamicamente o nível de exibição de uma TAG.
* Facilitar a depuração e o acompanhamento da execução do firmware.
* Servir como exemplo básico para utilização do sistema de logs em aplicações embarcadas.

---

## Bibliotecas utilizadas

| Biblioteca  | Finalidade                                                                                    |
| ----------- | --------------------------------------------------------------------------------------------- |
| `stdio.h`   | Funções padrão de entrada e saída da linguagem C.                                             |
| `esp_log.h` | Sistema de logs do ESP-IDF, permitindo geração e filtragem de mensagens por prioridade e TAG. |

---

## Configuração do firmware

| Parâmetro             | Configuração          |
| --------------------- | --------------------- |
| Linguagem             | C                     |
| Framework             | ESP-IDF               |
| Função principal      | `app_main()`          |
| Biblioteca de logs    | `esp_log.h`           |
| TAG principal         | `LOG1`                |
| TAG secundária        | `LOG2`                |
| Configuração dinâmica | `esp_log_level_set()` |
| Variável utilizada    | `int var`             |

---

## Funcionamento

1. Inicializa uma variável inteira (`var`) utilizada para demonstrar a impressão de valores nos logs.
2. Emite mensagens utilizando todos os níveis de prioridade disponíveis para a TAG **LOG1**:

   * Erro (`ESP_LOGE`)
   * Warning (`ESP_LOGW`)
   * Informação (`ESP_LOGI`)
   * Debug (`ESP_LOGD`)
   * Verbose (`ESP_LOGV`)
3. Configura a TAG **LOG2** para exibir apenas mensagens com prioridade **Warning** ou superior através da função:

```c
esp_log_level_set("LOG2", ESP_LOG_WARN);
```

4. Emite novamente mensagens utilizando todos os níveis de log para a TAG **LOG2**.
5. Como o nível configurado é **Warning**, apenas os logs de **Erro** e **Warning** serão exibidos para essa TAG.

---

## Níveis de prioridade dos logs

| Nível      | Macro        | Descrição                                                                                    |
| ---------- | ------------ | -------------------------------------------------------------------------------------------- |
| Erro       | `ESP_LOGE()` | Indica falhas críticas que normalmente impedem o funcionamento correto da aplicação.         |
| Warning    | `ESP_LOGW()` | Indica situações inesperadas ou potenciais problemas que não interrompem a execução.         |
| Informação | `ESP_LOGI()` | Utilizado para informar eventos importantes durante a execução do firmware.                  |
| Debug      | `ESP_LOGD()` | Destinado ao auxílio na depuração durante o desenvolvimento.                                 |
| Verbose    | `ESP_LOGV()` | Exibe informações detalhadas da execução, geralmente utilizadas para diagnósticos avançados. |

---

## TAGs

As TAGs permitem identificar a origem das mensagens de log.

Neste projeto são utilizadas:

| TAG    | Finalidade                                                         |
| ------ | ------------------------------------------------------------------ |
| `LOG1` | Demonstra todos os níveis de log disponíveis.                      |
| `LOG2` | Demonstra o filtro de prioridade utilizando `esp_log_level_set()`. |

---

## Observações

* A prioridade mínima dos logs exibidos pode ser configurada através do **ESP-IDF Menuconfig**.
* Também é possível alterar o nível de exibição individualmente para cada TAG durante a execução do firmware utilizando a função `esp_log_level_set()`.
* O sistema de logs do ESP-IDF facilita significativamente a depuração de aplicações embarcadas, permitindo organizar mensagens por prioridade e origem.

Para alterar o nível global de logs:

```text
idf.py menuconfig
→ Component config
→ Log output
→ Default log verbosity
```

---

## Informações

| Info        | Modelo             |
| ----------- | ------------------ |
| uC          | ESP32-C3           |
| Placa       | ESP32-C3 DevKitM-1 |
| Arquitetura | RISC-V (32 bits)   |
| IDE         | ESP-IDF v5.4.2     |

---

## Estrutura do projeto

```text
LOGs/
├── CMakeLists.txt
├── sdkconfig
├── README.md
└── main/
    ├── CMakeLists.txt
    └── main.c
```
