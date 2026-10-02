# Sistema de Gerenciamento de Oficina Mecânica

Projeto Final da disciplina **Programação e Desenvolvimento de Software 2** (2º semestre de 2026)
Universidade Federal de Minas Gerais (UFMG)

## Integrantes

| Integrante | Classes sob responsabilidade |
|---|---|
| Gabriel Lucas de Oliveira | `Cliente`, `Peca` |
| Arthur Ferreira Argueles | `Veiculo`, `Agendamento` |
| Marcos Vinicius Mendes dos Santos | `OrdemDeServico` |
| Rickelve Crispim de Lima | `Mecanico` |
| João Pedro Costa Lima | `Servico` |

## Descrição do problema

Oficinas mecânicas pequenas e médias costumam controlar clientes, veículos, agendamentos, peças e ordens de serviço de forma manual (cadernos, planilhas ou mensagens), o que dificulta consultar o histórico de um veículo, evitar horários duplicados e saber quanto custou cada atendimento.

Este projeto desenvolve, em C++11, um sistema de terminal que organiza esse fluxo: o cliente e seus veículos são cadastrados, o atendimento é agendado, uma ordem de serviço acompanha o conserto (diagnóstico, serviços, peças e mecânico responsável) e o valor total é calculado ao final. A escolha do tema se deve ao fato de ele ter entidades bem definidas e relações claras entre elas, o que permite aplicar de forma natural os conceitos da disciplina: encapsulamento, herança, polimorfismo, tratamento de exceções e testes unitários.

## Objetivos

- Modelar o domínio de uma oficina com classes coesas e responsabilidades bem definidas (Cartões CRC em `design/`).
- Atender aos requisitos descritos nas User Stories (`design/User-Stories.txt`): cadastro de clientes e veículos, agendamento, ordens de serviço, controle de estoque de peças e consulta do histórico do veículo.
- Desenvolver o sistema com TDD, usando doctest e cobertura de código mínima de 60% (gcovr).
- Aplicar programação defensiva e tratamento de exceções em todas as entradas do sistema.
- Documentar o código com Doxygen e manter o histórico de commits do grupo no GitHub.

## Modelagem

| Classe | Descrição |
|---|---|
| `Cliente` | Dados pessoais do cliente, seus veículos, ordens e agendamentos. |
| `Veiculo` | Dados do veículo, quilometragem e histórico de manutenções. |
| `Agendamento` | Reserva de data e horário para um serviço, com verificação de disponibilidade. |
| `OrdemDeServico` | Acompanha o atendimento: diagnóstico, serviços, peças, mecânico, status e valor total. |
| `Servico` | Tipo de serviço oferecido, com descrição, preço e duração estimada. |
| `Peca` | Peça do estoque, com código, preço e quantidade disponível. |
| `Mecanico` | Mecânico responsável pelas ordens, com especialidade. |
| `StatusOrdem` | Enumeração do ciclo de vida da ordem: `ABERTA → EM_DIAGNOSTICO → EM_EXECUCAO ↔ AGUARDANDO_PECA → AGUARDANDO_RETIRADA → FINALIZADA`. |

## Estrutura do repositório

```
.
├── build/      # Arquivos gerados na compilação
├── design/     # User Stories e Cartões CRC
├── include/    # Contratos (.hpp)
├── src/        # Implementações (.cpp)
├── tests/      # Testes de unidade (doctest)
├── Doxyfile    # Configuração da documentação
└── Makefile    # Compilação automatizada
```

## Requisitos

- Compilador com suporte a C++11 (`g++`)
- `make`
- [doctest](https://github.com/onqtam/doctest) (testes)
- [gcovr](https://gcovr.com/en/stable/) (cobertura)
- [Doxygen](http://www.doxygen.org/) (documentação)

No Ubuntu:

```bash
sudo apt update && sudo apt install build-essential doxygen gcovr
```

## Como compilar e executar

> O `Makefile` será adicionado no checkpoint C7.

```bash
make        # compila o projeto
make run    # compila e executa
```

## Documentação

Para gerar a documentação com Doxygen:

```bash
doxygen Doxyfile
```

Os arquivos são gerados na pasta `html/`; abra `html/index.html` no navegador.

## Testes

Os testes de unidade ficam em `tests/` e usam o framework doctest. A cobertura pode ser verificada com:

```bash
gcovr -r .
```

## Contribuição

O grupo adota commits frequentes e pequenos, sempre identificados com o nome e o e-mail cadastrados no Moodle, como combinado no termo de compromisso do checkpoint C3. Funções e variáveis seguem o padrão `camelCase` (`nomeFuncao()`, `nomeVariavel`).
