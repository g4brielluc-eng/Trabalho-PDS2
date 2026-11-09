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

Este projeto desenvolve, em C++11, um sistema de terminal que organiza esse fluxo: o cliente e seus veículos são cadastrados, o atendimento é agendado, uma ordem de serviço acompanha o conserto (diagnóstico, serviços, peças e mecânico responsável) e o valor total é calculado ao final. A escolha do tema se deve ao fato de ele ter entidades bem definidas e relações claras entre elas, o que permite aplicar de forma natural os conceitos da disciplina: encapsulamento, modularização,tratamento de exceções e testes unitários.

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

O mecânico atualiza o status da ordem até `AGUARDANDO_RETIRADA`. A finalização (entrega do veículo) é feita apenas por `OrdemDeServico::finalizar`.

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

> Execute os comandos a partir da **raiz do projeto**, porque os arquivos de dados são lidos e gravados na pasta atual.

```bash
make run        # compila e executa o sistema (build/oficina)
make test       # compila e roda todos os testes de unidade
make coverage   # roda os testes e gera o relatório de cobertura
make clean      # remove os arquivos gerados
make            # roda os testes
```

## Como usar o sistema

O programa possui um menu principal e submenus numerados de 1 a 7, além do 'X', que fecha a aplicação:

```
1. Clientes
2. Veiculos
3. Agendamentos
4. Ordens de Servico
5. Pecas
6. Mecanicos
7. Servicos
X. Sair 

```

Ordem padrão do atendimento de um novo cliente:

1. Cadastre um cliente (submenu 1) e um veículo associado ao CPF dele (submenu 2).
2. Cadastre os serviços (submenu 7), as peças (submenu 5) e um mecânico que assumirá a ordem (submenu 6).
3. Abra uma ordem de serviço pela placa do veículo (submenu 4), associe o mecânico e adicione serviços e peças.
4. O mecânico registra o diagnóstico e atualiza o status da ordem até `AGUARDANDO_RETIRADA` (submenu 6).
5. Com a ordem em `AGUARDANDO_RETIRADA`, finalize-a no submenu 4. O valor total é calculado com os serviços e peças.

Em todos os submenus, a opção `0` volta ao menu anterior.

## Persistência dos dados

Os dados são lidos ao iniciar o programa e gravados em arquivos de texto em dois momentos distintos: No fim da aplicação e ao voltar para o menu principal utilizando a opção `0`. Essa estratégia foi adotada para aumentar a garantia de salvamento dos dados, não dependendo exclusivamente do fim do programa para que haja a gravação. 

| Arquivo | Conteúdo |
|---|---|
| `listaClientes.txt` | Clientes |
| `veiculos.txt` | Veículos (com o CPF do dono) |
| `agendamentos.txt` | Agendamentos (com placa e CPF) |
| `servicos.txt` | Tipos de serviço |
| `estoque.txt` | Peças em estoque |
| `mecanicos.txt` | Mecânicos |
| `ordens.txt` | Ordens de serviço (com mecânico, serviços e peças) |

## Documentação

Para gerar a documentação com Doxygen:

```bash
doxygen Doxyfile
```

Os arquivos são gerados na pasta `html/`; abra `html/index.html` no navegador.

## Testes

Os testes de unidade ficam em `tests/` (um arquivo `teste_*.cpp` por classe) e usam o framework doctest. Para rodá-los:

```bash
make test
```

## Decisões de projeto

- Exceções: argumentos inválidos lançam `std::invalid_argument`; operações em estado inválido (por exemplo, um mecânico mexendo numa ordem que não é dele) lançam `std::logic_error`. O menu captura as exceções e exibe a mensagem ao usuário.

- Modularização e Testes: O projeto separa as regras das entidades (cada classe com seu `.hpp` e `.cpp`) do fluxo principal. O arquivo `src/main.cpp` atua como o controlador do sistema, responsável pelos menus e por organizar o carregamento e salvamento geral dos dados. Como os testes de unidade servem para validar o comportamento isolado das classes, e não para testar a navegação de telas, a `main.cpp` foi intencionalmente desconsiderada da cobertura. Assim, garantiu-se que a porcentagem reflita a robustez das classes. O comando utilizado foi: `gcovr --root . --filter 'src/' -e 'src/main.cpp'`.

- Herança e polimorfismo: não foram usados, pois as classes implementadas possuem pouca dependência entre si, não sendo necessária a adição desses conceitos. Preferiu-se associação entre classes, o que evita o acoplamento forte que a herança cria entre classe base e derivadas.

## Contribuição

O grupo adota commits frequentes e pequenos, sempre identificados com o nome e o e-mail cadastrados no Moodle, como combinado no termo de compromisso do checkpoint C3. Funções e variáveis seguem o padrão `camelCase` (`nomeFuncao()`, `nomeVariavel`).
