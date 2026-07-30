# _Exemplos--ESP-IDF_

![Framework](https://img.shields.io/badge/Framework-ESP--IDF%20v5.4.2-blue)
![Language](https://img.shields.io/badge/Language-C-orange)
![Platform](https://img.shields.io/badge/Platform-ESP32-green)
![Status](https://img.shields.io/badge/Status-Em%20Desenvolvimento-success)

---

## Sobre o repositório

O **Exemplos--ESP-IDF** é um repositório desenvolvido com o objetivo de reunir diversos exemplos práticos utilizando o framework **ESP-IDF** para a família de microcontroladores **ESP32** da Espressif.

Os projetos foram desenvolvidos com foco didático, apresentando exemplos simples, organizados e comentados que demonstram o funcionamento dos principais periféricos, recursos do hardware e APIs disponibilizadas pelo ESP-IDF.

Cada exemplo é independente, possuindo sua própria documentação (`README.md`), facilitando a consulta e reutilização em novos projetos.

---

## Objetivos

- Disponibilizar exemplos práticos utilizando o ESP-IDF.
- Facilitar o aprendizado da plataforma ESP32.
- Demonstrar a utilização das APIs oficiais da Espressif.
- Servir como base para novos projetos embarcados.
- Disponibilizar códigos simples, organizados e comentados.
- Criar uma biblioteca de exemplos para consultas futuras.

---

## Organização do repositório

Os projetos estão organizados por categorias.

Atualmente, todos os exemplos básicos encontram-se na pasta **BASE**, enquanto novas categorias serão adicionadas conforme o repositório evoluir.

---

## Conteúdo atual

### BASE

| Projeto | Descrição |
|---------|-----------|
| Blink | Controle de uma saída digital para acionamento de LED. |
| Delays | Controle de delay e temporização bloqueante para pulsos rápidos. |
| Entrada Digital | Leitura de uma entrada digital utilizando botão. |
| ADC | Configuração e leitura dos conversores analógico-digitais ADC1 e ADC2. |
| DAC | Configuração e leitura dos conversores digital-analógicos. |
| PWM | Geração de sinais PWM utilizando o periférico LEDC. |
| LOGs | Utilização do sistema de logs do ESP-IDF. |
| MemoryView | Consulta e monitoramento da memória do ESP32. |

### FreeRTOS

Categoria criada para armazenar exemplos relacionados ao sistema operacional de tempo real (RTOS).

*Em desenvolvimento.*

---

## Estrutura do repositório

```text
Exemplos--ESP-IDF/
│
├── BASE/
│   ├── ADC/
│   ├── Entrada-Digital/
│   ├── GPIO-Blink/
│   ├── LOGs/
│   ├── MemoryView/
│   ├── PWM/
│   └── ...
│
├── FreeRTOS/
│   └── (em desenvolvimento)
│
├── .git/
├── .gitattributes
└── README.md
```

Cada projeto possui sua própria estrutura contendo:

- Código-fonte (`main.c`);
- README específico;
- Arquivos de configuração do ESP-IDF;
- Arquivos para simulação utilizando o Wokwi (quando aplicável).

---

## Requisitos

Para executar os exemplos recomenda-se:

| Item | Versão |
|------|---------|
| Framework | ESP-IDF v5.4.2 ou superior |
| Linguagem | C |
| IDE | Visual Studio Code |
| Extensão | Espressif IDF Extension |
| Compilador | GCC (Toolchain ESP-IDF) |

---

## Simulação

Grande parte dos exemplos pode ser executada visualmente utilizando o **Wokwi Simulator**.

Para isso é necessário:

- Instalar a extensão **Wokwi Simulator** para o Visual Studio Code;
- Criar o arquivo `wokwi.toml`;
- Criar o arquivo `diagram.json` contendo o circuito da simulação.

Documentação oficial:

[Wokwi Simulator VS Code](https://docs.wokwi.com/vscode/getting-started)

---

## Padrão dos projetos

Cada projeto deste repositório segue um padrão de documentação contendo:

- Histórico de versões;
- Resumo;
- Objetivos;
- Bibliotecas utilizadas;
- Configuração do firmware;
- Funcionamento;
- Pinos utilizados (quando aplicável);
- Observações;
- Informações do hardware;
- Estrutura do projeto.

Esse padrão facilita a manutenção do repositório e a consulta rápida dos exemplos.

---

## Contribuições

Sugestões, melhorias e correções são sempre bem-vindas.

Caso encontre algum problema ou queira sugerir novos exemplos, fique à vontade para abrir uma **Issue** ou enviar um **Pull Request** na branch **develop**.

---

## Autor

**Ricieri Juan**

Engenharia Elétrica • P&D • Sistemas Embarcados • Projetos Elétricos • Perícias Técnicas

---

## Licença

Este repositório possui finalidade educacional e de estudo.

Os exemplos podem ser utilizados livremente como base para novos projetos, sendo recomendada a atribuição dos devidos créditos ao autor.