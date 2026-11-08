/*
======================================================================================
                                    SISTEMA DA OFICINA
======================================================================================

1 - Agendamentos
2 - Clientes
3 - Mecanicos
4 - Ordens de Servico
5 - Pecas
6 - Servicos
7 - Veiculos
0 - Sair

Escolha uma opcao:


MENU PRINCIPAL

1 - Agendamentos
2 - Clientes
3 - Mecanicos
4 - Ordens de Servico
5 - Pecas
6 - Servicos
7 - Veiculos
0 - Sair

Escolha uma opcao:

SUBMENUS

AGENDAMENTOS

1 - Alterar data e horario
2 - Cancelar agendamento
3 - Confirmar agendamento
4 - Criar agendamento
5 - Consultar agendamentos
6 - Verificar disponibilidade
X - Voltar


CLIENTES

1 - Cadastrar cliente
2 - Consultar agendamentos do cliente
3 - Consultar cliente
4 - Consultar ordens de servico do cliente
5 - Consultar veiculos do cliente
6 - Listar clientes
X - Voltar


MECANICOS -- Rickelve

1 - Alterar especialidade
2 - Atualizar status
3 - Cadastrar mecanico
4 - Consultar mecanico
5 - Consultar ordens atribuidas
6 - Listar mecanicos
7 - Registrar diagnostico
X - Voltar


ORDENS DE SERVICO

1 - Adicionar peca
2 - Adicionar servico
3 - Associar mecanico
4 - Calcular valor total
5 - Consultar ordem
6 - Finalizar ordem
7 - Listar ordens
8 - Abrir ordem de servico
9 - Atualizar status
X - Voltar


PECAS

1 - Atualizar preco
2 - Atualizar quantidade
3 - Cadastrar peca
4 - Consultar disponibilidade da peca
5 - Consultar peca
6 - Listar pecas
7 - Listar pecas em falta
X - Voltar


SERVICOS

1 - Atualizar descricao
2 - Atualizar duracao
3 - Atualizar duracao estimada
4 - Atualizar preco
5 - Calcular custo total
6 - Cadastrar servico
7 - Consultar servicos
8 - Consultar tempo estimado
X - Voltar


VEICULOS

1 - Atualizar placa
2 - Associar cliente ao veiculo
3 - Cadastrar veiculo
4 - Consultar historico de manutencoes
5 - Consultar veiculo
6 - Listar veiculos
7 - Registrar quilometragem
X - Voltar
*/

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>
#include <memory>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "Agendamento.hpp"
#include "Peca.hpp"
#include "Cliente.hpp"
#include "Veiculo.hpp"
#include "OrdemDeServico.hpp"
#include "Mecanico.hpp"
#include "Status_Ordem.hpp"
#include "Servico.hpp"

std::vector<std::shared_ptr<Agendamento>> armazenamentoAgendamentos; // Armazena os agendamentos criados
std::vector<std::shared_ptr<Veiculo>> armazenamentoVeiculos;         // Armazena os veiculos criados
std::vector<std::shared_ptr<Servico>> armazenamentoServicos;         // Armazena os servicos cadastrados

// Função para limpar a tela do console
void limparTela()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

// Função para pausar a execução até que o usuário pressione Enter
void pausar()
{
    std::cout << std::endl
              << "Pressione Enter para continuar...";
    std::cin.get();
}

bool testarVolta(const std::string &str)
{
    if (str == "0")
    {
        std::cout << "Operacao cancelada" << std::endl;
        pausar();
        return true; // Sai da função e volta ao menu anterior
    }
    return false; // Continua a execução normal
}

bool converterParaId(const std::string &entrada, int &idDestino)
{

    try
    {
        idDestino = std::stoi(entrada);
        return true;
    }
    catch (...)
    {
        std::cout << "Erro: O ID deve ser um numero valido." << std::endl;
        pausar();
        return false;
    }
}

bool verificarID(int &id)
{
    while (!(std::cin >> id))
    {
        std::cin.clear();            // Tira o cin do estado de erro
        std::cin.ignore(5000, '\n'); // Joga a letra errada fora
        std::cout << "Entrada invalida! Digite apenas numeros";
    }
    std::cin.ignore(5000, '\n'); // Limpa o \n para os próximos getlines

    if (id == 0)
    {
        return true;
    }
    return false;
}

std::string converterStatusParaString(StatusOrdem status)
{
    switch (status)
    {
    case StatusOrdem::ABERTA:
        return "ABERTA";
    case StatusOrdem::EM_DIAGNOSTICO:
        return "EM DIAGNOSTICO";
    case StatusOrdem::EM_EXECUCAO:
        return "EM EXECUCAO";
    case StatusOrdem::AGUARDANDO_PECA:
        return "AGUARDANDO PECA";
    case StatusOrdem::AGUARDANDO_RETIRADA:
        return "AGUARDANDO RETIRADA";
    case StatusOrdem::FINALIZADA:
        return "FINALIZADA";
    default:
        return "STATUS DESCONHECIDO";
    }
}

// Funcao para salvar os agendamentos em um arquivo
void salvarAgendamentos()
{
    std::ofstream arquivo("agendamentos.txt");

    if (!arquivo.is_open())
    {
        std::cout << "Erro ao salvar agendamentos." << std::endl;
        return;
    }

    for (const auto &agendamento : armazenamentoAgendamentos)
    {
        std::string placa = "";
        std::string cpf = "";

        if (agendamento->getVeiculo() != nullptr)
        {
            placa = agendamento->getVeiculo()->getPlaca();
        }

        if (agendamento->getCliente() != nullptr)
        {
            cpf = agendamento->getCliente()->getCpf();
        }

        arquivo << agendamento->getData() << ";"
                << agendamento->getHorario() << ";";

        switch (agendamento->getStatus())
        {
        case Agendamento::Status::PENDENTE:
            arquivo << "PENDENTE";
            break;

        case Agendamento::Status::CONFIRMADO:
            arquivo << "CONFIRMADO";
            break;

        case Agendamento::Status::CANCELADO:
            arquivo << "CANCELADO";
            break;
        }

        arquivo << ";" << placa
                << ";" << cpf << std::endl;
    }

    arquivo.close();

    std::cout << "Agendamentos salvos com sucesso!" << std::endl;
}

// Funcao para carregar os agendamentos de um arquivo
void carregarAgendamentos()
{
    std::ifstream arquivo("agendamentos.txt");

    if (!arquivo.is_open())
    {
        return;
    }

    std::string linha;

    while (std::getline(arquivo, linha))
    {
        std::stringstream ss(linha);

        std::string data, horario, status, placa, cpf;

        std::getline(ss, data, ';');
        std::getline(ss, horario, ';');
        std::getline(ss, status, ';');
        std::getline(ss, placa, ';');
        std::getline(ss, cpf);

        try
        {
            auto novoAgendamento =
                std::make_shared<Agendamento>(data, horario);

            // Procura o veículo pela placa
            Veiculo *veiculoEncontrado = nullptr;

            for (const auto &veiculo : armazenamentoVeiculos)
            {
                if (veiculo->getPlaca() == placa)
                {
                    veiculoEncontrado = veiculo.get();
                    break;
                }
            }

            if (veiculoEncontrado != nullptr)
            {
                novoAgendamento->associarVeiculo(veiculoEncontrado);
            }

            // Procura o cliente pelo CPF
            Cliente *clienteEncontrado = nullptr;

            if (!cpf.empty())
            {
                clienteEncontrado = Cliente::pesquisarPorCpf(cpf);
            }

            if (clienteEncontrado == nullptr && veiculoEncontrado != nullptr)
            {
                clienteEncontrado = veiculoEncontrado->getCliente();
            }

            if (clienteEncontrado != nullptr)
            {
                novoAgendamento->associarCliente(clienteEncontrado);
            }

            novoAgendamento->registrarAgendamento();

            if (status == "CONFIRMADO")
            {
                novoAgendamento->confirmar();
            }
            else if (status == "CANCELADO")
            {
                novoAgendamento->cancelar();
            }

            armazenamentoAgendamentos.push_back(novoAgendamento);
        }
        catch (const std::exception &e)
        {
            std::cout << "Erro ao carregar agendamento: "
                      << e.what() << std::endl;
        }
    }

    arquivo.close();
}

// Funcao para salvar os veiculos em um arquivo
void salvarVeiculos()
{
    std::ofstream arquivo("veiculos.txt");

    if (!arquivo.is_open())
    {
        std::cout << "Erro ao salvar os veiculos." << std::endl;
        return;
    }

    for (const auto &veiculo : armazenamentoVeiculos)
    {
        std::string cpf = "";

        if (veiculo->getCliente() != nullptr)
        {
            cpf = veiculo->getCliente()->getCpf();
        }

        arquivo << veiculo->getMarca() << ";"
                << veiculo->getModelo() << ";"
                << veiculo->getAno() << ";"
                << veiculo->getPlaca() << ";"
                << veiculo->getQuilometragem() << ";"
                << cpf << std::endl;
    }

    arquivo.close();
}

// Funcao para carregar os veiculos de um arquivo
void carregarVeiculos()
{
    std::ifstream arquivo("veiculos.txt");

    if (!arquivo.is_open())
    {
        return;
    }

    std::string marca, modelo, placa;
    std::string anoTexto, quilometragemTexto, cpf;

    while (std::getline(arquivo, marca, ';'))
    {
        std::getline(arquivo, modelo, ';');
        std::getline(arquivo, anoTexto, ';');
        std::getline(arquivo, placa, ';');
        std::getline(arquivo, quilometragemTexto, ';');
        std::getline(arquivo, cpf);

        try
        {
            int ano = std::stoi(anoTexto);
            int quilometragem = std::stoi(quilometragemTexto);

            std::shared_ptr<Veiculo> novoVeiculo =
                std::make_shared<Veiculo>(
                    marca, modelo, ano, placa, quilometragem);

            // Se existir um CPF salvo, procura o cliente
            if (!cpf.empty())
            {
                Cliente *clienteEncontrado = Cliente::pesquisarPorCpf(cpf);

                if (clienteEncontrado != nullptr)
                {
                    novoVeiculo->associarCliente(clienteEncontrado);
                    clienteEncontrado->setVeiculos(novoVeiculo.get());
                }
                else
                {
                    std::cout << "Cliente de CPF " << cpf
                              << " nao encontrado para o veiculo "
                              << placa << std::endl;
                }
            }

            armazenamentoVeiculos.push_back(novoVeiculo);
        }
        catch (const std::exception &e)
        {
            std::cout << "Erro ao carregar veiculo: "
                      << e.what() << std::endl;
        }
    }

    arquivo.close();
}

// Funcao para salvar os servicos em um arquivo
void salvarServicos()
{
    std::ofstream arquivo("servicos.txt");

    if (!arquivo.is_open())
    {
        std::cout << "Erro ao salvar os servicos." << std::endl;
        return;
    }

    for (const auto &servico : armazenamentoServicos)
    {
        arquivo << servico->getNome() << ";"
                << servico->getDescricao() << ";"
                << servico->getPreco() << ";"
                << servico->getDuracaoEstimada() << std::endl;
    }

    arquivo.close();
}

// Funcao para carregar os servicos de um arquivo
void carregarServicos()
{
    std::ifstream arquivo("servicos.txt");

    if (!arquivo.is_open())
    {
        return;
    }

    std::string nome, descricao, precoTexto, duracaoTexto;

    while (std::getline(arquivo, nome, ';'))
    {
        std::getline(arquivo, descricao, ';');
        std::getline(arquivo, precoTexto, ';');
        std::getline(arquivo, duracaoTexto);

        try
        {
            float preco = std::stof(precoTexto);
            int duracao = std::stoi(duracaoTexto);

            armazenamentoServicos.push_back(
                std::make_shared<Servico>(nome, descricao, preco, duracao));
        }
        catch (const std::exception &e)
        {
            std::cout << "Erro ao carregar servico: " << e.what() << std::endl;
        }
    }

    arquivo.close();
}

// Procura um servico pelo nome (ignora maiusculas/minusculas). Retorna nullptr se nao achar.
Servico *buscarServico(const std::string &nome)
{
    auto minusculo = [](std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });
        return s;
    };

    for (const auto &servico : armazenamentoServicos)
    {
        if (minusculo(servico->getNome()) == minusculo(nome))
        {
            return servico.get();
        }
    }
    return nullptr;
}

// Le um float do teclado; se o usuario digitar lixo, limpa o cin e retorna false
bool lerFloat(float &valor)
{
    if (!(std::cin >> valor))
    {
        std::cin.clear();
        std::cin.ignore(5000, '\n');
        return false;
    }
    std::cin.ignore(5000, '\n');
    return true;
}

// Le um int do teclado; se o usuario digitar lixo, limpa o cin e retorna false
bool lerInt(int &valor)
{
    if (!(std::cin >> valor))
    {
        std::cin.clear();
        std::cin.ignore(5000, '\n');
        return false;
    }
    std::cin.ignore(5000, '\n');
    return true;
}

//================================================= SERVICOS =============================================

void servicos()
{
    char opcao;

    do
    {
        limparTela();

        std::cout << "-------------- SERVICOS --------------" << std::endl;
        std::cout << "1 - Cadastrar servico" << std::endl;
        std::cout << "2 - Consultar servicos" << std::endl;
        std::cout << "3 - Atualizar descricao" << std::endl;
        std::cout << "4 - Atualizar preco" << std::endl;
        std::cout << "5 - Atualizar duracao estimada" << std::endl;
        std::cout << "6 - Consultar tempo estimado" << std::endl;
        std::cout << "7 - Calcular custo total" << std::endl;
        std::cout << "X - Voltar" << std::endl;

        std::cout << std::endl
                  << "Digite uma opcao: ";
        std::cin >> opcao;
        std::cin.ignore(5000, '\n');

        switch (opcao)
        {
        case '1':
        {
            limparTela();

            std::cout << "-------------- CADASTRAR SERVICO --------------" << std::endl;

            std::string nome, descricao;
            float preco;
            int duracao;

            std::cout << "Digite o nome do servico ou 'X' para voltar: ";
            std::getline(std::cin, nome);

            if (nome == "X" || nome == "x")
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            if (buscarServico(nome) != nullptr)
            {
                std::cout << "\nErro: ja existe um servico com esse nome." << std::endl;
                pausar();
                break;
            }

            std::cout << "Digite a descricao do servico: ";
            std::getline(std::cin, descricao);

            std::cout << "Digite o preco do servico: ";
            if (!lerFloat(preco))
            {
                std::cout << "\nErro: preco invalido." << std::endl;
                pausar();
                break;
            }

            std::cout << "Digite a duracao estimada (em minutos): ";
            if (!lerInt(duracao))
            {
                std::cout << "\nErro: duracao invalida." << std::endl;
                pausar();
                break;
            }

            try
            {
                armazenamentoServicos.push_back(
                    std::make_shared<Servico>(nome, descricao, preco, duracao));

                std::cout << "\nServico cadastrado com sucesso!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "\nErro ao cadastrar servico: " << e.what() << std::endl;
            }

            pausar();
            break;
        }

        case '2':
        {
            limparTela();

            std::cout << "-------------- LISTA DE SERVICOS --------------" << std::endl;

            if (armazenamentoServicos.empty())
            {
                std::cout << "Nenhum servico cadastrado." << std::endl;
            }
            else
            {
                for (const auto &servico : armazenamentoServicos)
                {
                    std::cout << "\n----------------------------------------" << std::endl;
                    std::cout << servico->consultarInformacoes() << std::endl;
                }

                std::cout << "\n----------------------------------------" << std::endl;
                std::cout << "Total de servicos: "
                          << armazenamentoServicos.size() << std::endl;
            }

            pausar();
            break;
        }

        case '3':
        {
            limparTela();

            std::cout << "-------------- ATUALIZAR DESCRICAO --------------" << std::endl;

            if (armazenamentoServicos.empty())
            {
                std::cout << "Nenhum servico cadastrado." << std::endl;
                pausar();
                break;
            }

            std::string nome;
            std::cout << "Digite o nome do servico: ";
            std::getline(std::cin, nome);

            Servico *servico = buscarServico(nome);

            if (servico == nullptr)
            {
                std::cout << "\nServico nao encontrado." << std::endl;
            }
            else
            {
                std::string novaDescricao;

                std::cout << "Descricao atual: " << servico->getDescricao() << std::endl;
                std::cout << "Digite a nova descricao: ";
                std::getline(std::cin, novaDescricao);

                servico->atualizarDescricao(novaDescricao);

                std::cout << "\nDescricao atualizada com sucesso!" << std::endl;
            }

            pausar();
            break;
        }

        case '4':
        {
            limparTela();

            std::cout << "-------------- ATUALIZAR PRECO --------------" << std::endl;

            if (armazenamentoServicos.empty())
            {
                std::cout << "Nenhum servico cadastrado." << std::endl;
                pausar();
                break;
            }

            std::string nome;
            std::cout << "Digite o nome do servico: ";
            std::getline(std::cin, nome);

            Servico *servico = buscarServico(nome);

            if (servico == nullptr)
            {
                std::cout << "\nServico nao encontrado." << std::endl;
            }
            else
            {
                float novoPreco;

                std::cout << "Preco atual: R$ " << servico->getPreco() << std::endl;
                std::cout << "Digite o novo preco: ";

                if (!lerFloat(novoPreco))
                {
                    std::cout << "\nErro: preco invalido." << std::endl;
                }
                else
                {
                    try
                    {
                        servico->atualizarPreco(novoPreco);

                        std::cout << "\nPreco atualizado com sucesso!" << std::endl;
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "\nErro ao atualizar preco: " << e.what() << std::endl;
                    }
                }
            }

            pausar();
            break;
        }

        case '5':
        {
            limparTela();

            std::cout << "-------------- ATUALIZAR DURACAO ESTIMADA --------------" << std::endl;

            if (armazenamentoServicos.empty())
            {
                std::cout << "Nenhum servico cadastrado." << std::endl;
                pausar();
                break;
            }

            std::string nome;
            std::cout << "Digite o nome do servico: ";
            std::getline(std::cin, nome);

            Servico *servico = buscarServico(nome);

            if (servico == nullptr)
            {
                std::cout << "\nServico nao encontrado." << std::endl;
            }
            else
            {
                int novaDuracao;

                std::cout << "Duracao atual: " << servico->getDuracaoEstimada() << " min" << std::endl;
                std::cout << "Digite a nova duracao (em minutos): ";

                if (!lerInt(novaDuracao))
                {
                    std::cout << "\nErro: duracao invalida." << std::endl;
                }
                else
                {
                    try
                    {
                        servico->registrarDuracaoEstimada(novaDuracao);

                        std::cout << "\nDuracao atualizada com sucesso!" << std::endl;
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "\nErro ao atualizar duracao: " << e.what() << std::endl;
                    }
                }
            }

            pausar();
            break;
        }

        case '6':
        {
            limparTela();

            std::cout << "-------------- CONSULTAR TEMPO ESTIMADO --------------" << std::endl;

            if (armazenamentoServicos.empty())
            {
                std::cout << "Nenhum servico cadastrado." << std::endl;
                pausar();
                break;
            }

            std::string nome;
            std::cout << "Digite o nome do servico: ";
            std::getline(std::cin, nome);

            Servico *servico = buscarServico(nome);

            if (servico == nullptr)
            {
                std::cout << "\nServico nao encontrado." << std::endl;
            }
            else
            {
                std::cout << "\nO servico \"" << servico->getNome()
                          << "\" leva cerca de " << servico->getDuracaoEstimada()
                          << " minutos." << std::endl;
            }

            pausar();
            break;
        }

        case '7':
        {
            limparTela();

            std::cout << "-------------- CALCULAR CUSTO TOTAL --------------" << std::endl;

            if (armazenamentoServicos.empty())
            {
                std::cout << "Nenhum servico cadastrado." << std::endl;
                pausar();
                break;
            }

            std::string nome;
            std::cout << "Digite o nome do servico: ";
            std::getline(std::cin, nome);

            Servico *servico = buscarServico(nome);

            if (servico == nullptr)
            {
                std::cout << "\nServico nao encontrado." << std::endl;
            }
            else
            {
                int quantidade;

                std::cout << "Preco unitario: R$ " << servico->getPreco() << std::endl;
                std::cout << "Digite a quantidade: ";

                if (!lerInt(quantidade))
                {
                    std::cout << "\nErro: quantidade invalida." << std::endl;
                }
                else
                {
                    try
                    {
                        float custo = servico->calcularCusto(quantidade);

                        std::cout << "\nCusto total: R$ " << custo << std::endl;
                    }
                    catch (const std::exception &e)
                    {
                        std::cout << "\nErro ao calcular custo: " << e.what() << std::endl;
                    }
                }
            }

            pausar();
            break;
        }

        case 'X':
        case 'x':
        {
            limparTela();
            std::cout << "Voltando ao menu principal..." << std::endl;
            break;
        }

        default:
        {
            limparTela();
            std::cout << "Opcao invalida. Tente novamente." << std::endl;
            pausar();
            break;
        }
        }

    } while (opcao != 'X' && opcao != 'x');
}

//============================================= Cliente =============================================

void cliente() {
    static bool carregado = false;
    const std::string arquivoClientes = "listaClientes.txt";

    if (!carregado) {
        Cliente::carregarDoArquivo(arquivoClientes);
        carregado = true;
    }

    char opcao;

    while (true) {
        limparTela(); // Utilizar essa função sempre que abrir o while

        std::cout << "-------------- CLIENTE --------------" << std::endl;
        std::cout << "1 - Cadastrar cliente" << std::endl;
        std::cout << "2 - Consultar agendamentos do cliente" << std::endl;
        std::cout << "3 - Consultar informações de cliente" << std::endl;
        std::cout << "4 - Consultar ordens de serviço do cliente" << std::endl;
        std::cout << "5 - Consultar veículos do cliente" << std::endl;
        std::cout << "6 - Listar clientes" << std::endl;
        std::cout << "X - Voltar" << std::endl;

        std::cout << std::endl << "Digite uma opcao:" << std::endl;
        std::cin >> opcao;
        std::cin.ignore(5000, '\n');

switch (opcao) {
            case '1': {
                limparTela();
                std::cout << "-------------- CADASTRAR CLIENTE --------------" << std::endl;

                std::string continuar = "s";
                while (continuar == "s" || continuar == "S") {
                    try {
                        new Cliente();
                    } catch (const std::runtime_error& e) {
                        std::cout << "Erro: " << e.what() << std::endl;
                        return;
                    }
                    Cliente::salvarNoArquivo(arquivoClientes);

                    std::cout << "\nDeseja cadastrar outro cliente? (S/N): ";
                    std::getline(std::cin, continuar);
                }
                break;
            }

            case '2': {
                limparTela();
                std::cout << "-------------- CONSULTAR AGENDAMENTOS --------------" << std::endl;

                // Falta um getAgendamentos() na classe Cliente para isso.
                std::cout << "Consulta de agendamentos ainda não disponível.\n";
                break;
            }

            case '3': {
                limparTela();
                std::cout << "-------------- CONSULTAR INFORMAÇÕES DE CLIENTE --------------" << std::endl;
                std::cout << "Digite o CPF do cliente (ou 'X' para voltar): " << std::endl;
                std::string cpf;
                std::getline(std::cin, cpf);

                if (cpf == "X" || cpf == "x") {
                    break;
                }

                Cliente* consulta = Cliente::pesquisarPorCpf(cpf);
                if (consulta == nullptr) {
                    break;
                }

                std::cout << "\nCliente encontrado!\n";
                std::cout << "Nome: " << consulta->getNome() << std::endl;
                std::cout << "CPF: " << consulta->getCpf() << std::endl;
                std::cout << "Contato: " << consulta->getContato() << std::endl;
                std::cout << "Endereço: " << consulta->getEndereco() << std::endl;
                std::string excluir;
                std::cout << "\nPara excluir o cadastro desse cliente tecle 'S': ";
                std::getline(std::cin, excluir);
                if(excluir == "s" || excluir == "S"){
                    std::cout << "\nTem certeza que deseja excluir o cadastro desse cliente? (S/N): ";
                    std::getline(std::cin, excluir);
                    if(excluir == "s" || excluir == "S") {
                        delete consulta;
                        consulta = nullptr;
                        Cliente::salvarNoArquivo(arquivoClientes);
                        std::cout << "Cliente excluido." << std::endl;
                    }
                }
                break;
            }

            case '4': {
                limparTela();
                std::cout << "-------------- CONSULTAR ORDENS DE SERVIÇO DO CLIENTE --------------" << std::endl;

                std::cout << "Digite o CPF do cliente (ou 'X' para voltar): " << std::endl;
                std::string cpf;
                std::getline(std::cin, cpf);

                if (cpf == "X" || cpf == "x") {
                    break;
                }

                Cliente* consulta = Cliente::pesquisarPorCpf(cpf);
                if (consulta == nullptr) {
                    break;
                }
                consulta->listarServicos();
                break;
            }

            case '5': {
                limparTela();
                std::cout << "-------------- CONSULTAR VEÍCULOS DO CLIENTE --------------" << std::endl;

                std::cout << "Digite o CPF do cliente (ou 'X' para voltar): " << std::endl;
                std::string cpf;
                std::getline(std::cin, cpf);

                if (cpf == "X" || cpf == "x") {
                    break;
                }

                Cliente* consulta = Cliente::pesquisarPorCpf(cpf);
                if (consulta == nullptr) {
                    break;
                }

                std::vector<Veiculo*>& veiculos = consulta->getVeiculos();
                if (veiculos.empty()) {
                    std::cout << "\nNenhum veículo associado a esse cliente.\n";
                    break;
                }

                std::cout << "\n-------------- VEÍCULOS --------------\n";
                std::cout << "Total: " << veiculos.size() << "\n";
                // aqui entra a impressão de cada veículo
                std::cout << "--------------------------------\n";
                break;
            }

            case '6': {
                limparTela();
                Cliente::listarTodas();
                break;
            }

            case 'X':
            case 'x': {
                limparTela();
                std::cout << "Voltando ao menu principal..." << std::endl;
                return;
            }

            default: {
                limparTela();
                std::cout << "Opcao invalida. Tente novamente." << std::endl;
                break;
            }
        }

        std::cout << "\nPressione Enter para continuar...";
        std::string pausa;
        std::getline(std::cin, pausa);
    }
}


//============================================= AGENDAMENTOS =============================================

void agendamento()
{

    char opcao;

    do
    {

        limparTela(); // Utilizar essa função sempre que abrir o do {}

        std::cout << "-------------- AGENDAMENTOS --------------" << std::endl;
        std::cout << "1- Verificar disponibilidade" << std::endl;
        std::cout << "2- Criar agendamento" << std::endl;
        std::cout << "3- Alterar data e horario" << std::endl;
        std::cout << "4- Confirmar agendamento" << std::endl;
        std::cout << "5- Cancelar agendamento" << std::endl;
        std::cout << "6- Consultar agendamentos" << std::endl;
        std::cout << "0- Voltar" << std::endl;

        std::cout << std::endl
                  << "Digite uma opcao:" << std::endl;

        std::cin >> opcao;
        std::cin.ignore(5000, '\n'); // Limpa o buffer de entrada

        switch (opcao)
        {
        case '1':
        {
            limparTela(); // Utilizar essa função sempre que abrir um case novo
            std::cout << "Verificando disponibilidade" << std::endl;

            std::cout << "Digite a data (dd/mm/aaaa) ou digite '0' para voltar: " << std::endl;
            std::string data;
            std::getline(std::cin, data);

            if (testarVolta(data))break;

            std::cout << "Digite o horario (hh:mm) ou digite '0' para voltar: " << std::endl;
            std::string horario;
            std::getline(std::cin, horario);

            if (testarVolta(horario))break;

            try
            {
                bool disponivel = Agendamento::verificarDisponibilidade(data, horario);
                if (disponivel)
                {
                    std::cout << "O horario " << horario << " do dia " << data << " esta disponivel." << std::endl;
                }
                else
                {
                    std::cout << "O horario " << horario << " do dia " << data << " nao esta disponivel." << std::endl;
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Erro: " << e.what() << std::endl;
            }

            pausar();
            break;
        }

        case '2':
        {
            limparTela();

            std::string data, horario, placa;

            std::cout << "Criando agendamento" << std::endl;
            std::cout << "Digite a data (dd/mm/aaaa) ou digite '0' para voltar: ";
            std::getline(std::cin, data);
            if (testarVolta(data))break;

            std::cout << "Digite o horario (hh:mm) ou digite '0' para voltar: ";
            std::getline(std::cin, horario);
            if (testarVolta(horario))break;

            std::cout << "Digite a placa do veiculo: ";
            std::getline(std::cin, placa);

            try
            {
                Veiculo *veiculoEncontrado = nullptr;

                // Procura o veiculo pela placa
                for (const auto &veiculo : armazenamentoVeiculos)
                {
                    if (veiculo->getPlaca() == placa)
                    {
                        veiculoEncontrado = veiculo.get();
                        pausar();
                        break;
                    }
                }

                if (veiculoEncontrado == nullptr)
                {
                    throw std::invalid_argument(
                        "Nao existe veiculo cadastrado com essa placa.");
                }

                // Recupera o cliente associado ao veiculo
                Cliente *clienteEncontrado = veiculoEncontrado->getCliente();

                if (clienteEncontrado == nullptr)
                {
                    throw std::invalid_argument(
                        "O veiculo nao possui um cliente associado.");
                }

                // Cria o agendamento
                std::shared_ptr<Agendamento> novoAgendamento =
                    std::make_shared<Agendamento>(data, horario);

                // Associa veiculo e cliente ao agendamento
                novoAgendamento->associarVeiculo(veiculoEncontrado);
                novoAgendamento->associarCliente(clienteEncontrado);

                novoAgendamento->registrarAgendamento();

                armazenamentoAgendamentos.push_back(novoAgendamento);

                std::cout << "\nAgendamento criado com sucesso!" << std::endl;
                std::cout << "Cliente: " << clienteEncontrado->getNome() << std::endl;
                std::cout << "Veiculo: " << veiculoEncontrado->getPlaca() << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "\nErro: " << e.what() << std::endl;
            }

            pausar();
            break;
        }

        case '3':
        {
            const auto &lista = Agendamento::consultarAgendamentos();

            limparTela();

            if (lista.empty())
            {
                std::cout << "Nenhum agendamento cadastrado." << std::endl;
                pausar();
                break;
            }

            std::cout << "AGENDAMENTOS CADASTRADOS" << std::endl;

            for (size_t i = 0; i < lista.size(); i++)
            {
                std::cout << i + 1 << " - Data: " << lista[i]->getData()
                          << " | Horario: " << lista[i]->getHorario() << std::endl;
            }

            size_t indice=0;

            std::cout << std::endl
                      << "Digite o numero do agendamento ou '0' para voltar: ";
            if (indice == 0)
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            try
            {
                if (indice < 1 || indice > lista.size())
                {
                    throw std::invalid_argument("Numero de agendamento invalido.");
                }

                std::string novaData, novoHorario;

                std::cout << "Digite a nova data: ";
                std::getline(std::cin, novaData);

                std::cout << "Digite o novo horario: ";
                std::getline(std::cin, novoHorario);

                lista[indice - 1]->alterarDataHorario(novaData, novoHorario);

                std::cout << std::endl
                          << "Agendamento alterado com sucesso!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << std::endl
                          << "Erro: " << e.what() << std::endl;
            }

            pausar();
            break;
        }
        case '4':
        {
            limparTela();

            std::cout << "=== CONFIRMAR AGENDAMENTO ===" << std::endl;

            const std::vector<Agendamento *> &agendamentos =
                Agendamento::consultarAgendamentos();

            if (agendamentos.empty())
            {
                std::cout << "Nao ha agendamentos cadastrados." << std::endl;
                pausar();
                break;
            }

            // Exibe os agendamentos cadastrados
            for (size_t i = 0; i < agendamentos.size(); i++)
            {
                std::cout << i + 1 << " - Data: "
                          << agendamentos[i]->getData()
                          << " | Horario: "
                          << agendamentos[i]->getHorario()
                          << std::endl;
            }

            size_t indice;

            std::cout << "\nDigite o numero do agendamento que deseja confirmar ou '0' para voltar: ";
            std::cin >> indice;
            std::cin.ignore(5000, '\n');

            if (indice == 0)
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            // Verifica se o numero informado e valido
            if (indice < 1 || indice > agendamentos.size())
            {
                std::cout << "Agendamento invalido." << std::endl;
                pausar();
                break;
            }

            try
            {
                agendamentos[indice - 1]->confirmar();

                std::cout << "\nAgendamento confirmado com sucesso!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "\nErro ao confirmar agendamento: "
                          << e.what() << std::endl;
            }

            pausar();
            break;
        }

        case '5':
        {
            limparTela();

            std::cout << "=== CANCELAR AGENDAMENTO ===" << std::endl;

            const std::vector<Agendamento *> &agendamentos =
                Agendamento::consultarAgendamentos();

            if (agendamentos.empty())
            {
                std::cout << "Nao ha agendamentos cadastrados." << std::endl;
                pausar();
                break;
            }

            // Exibe os agendamentos cadastrados
            for (size_t i = 0; i < agendamentos.size(); i++)
            {
                std::cout << i + 1 << " - Data: "
                          << agendamentos[i]->getData()
                          << " | Horario: "
                          << agendamentos[i]->getHorario()
                          << std::endl;
            }

            size_t indice;

            std::cout << "\nDigite o numero do agendamento que deseja cancelar ou '0' para voltar: ";
            std::cin >> indice;
            std::cin.ignore(5000, '\n');

            if (indice == 0)
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }
            // Verifica se o numero informado e valido
            if (indice < 1 || indice > agendamentos.size())
            {
                std::cout << "Agendamento invalido." << std::endl;
                pausar();
                break;
            }

            try
            {
                agendamentos[indice - 1]->cancelar();

                std::cout << "\nAgendamento cancelado com sucesso!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "\nErro ao cancelar agendamento: "
                          << e.what() << std::endl;
            }

            pausar();
            break;
        }
        case '6':
        {
            limparTela();

            std::cout << "=== CONSULTAR AGENDAMENTOS ===" << std::endl;

            const std::vector<Agendamento *> &agendamentos =
                Agendamento::consultarAgendamentos();

            if (agendamentos.empty())
            {
                std::cout << "Nao ha agendamentos cadastrados." << std::endl;
            }
            else
            {
                for (size_t i = 0; i < agendamentos.size(); i++)
                {

                    std::cout << "\nAgendamento " << i + 1 << std::endl;

                    Cliente *cliente = agendamentos[i]->getCliente();
                    Veiculo *veiculo = agendamentos[i]->getVeiculo();

                    std::cout << "Cliente: ";

                    if (cliente != nullptr)
                    {
                        std::cout << cliente->getNome() << std::endl;
                    }
                    else
                    {
                        std::cout << "Nao associado" << std::endl;
                    }

                    std::cout << "Placa: ";

                    if (veiculo != nullptr)
                    {
                        std::cout << veiculo->getPlaca() << std::endl;
                    }
                    else
                    {
                        std::cout << "Nao associado" << std::endl;
                    }
                    std::cout << "Data: "
                              << agendamentos[i]->getData() << std::endl;

                    std::cout << "Horario: "
                              << agendamentos[i]->getHorario() << std::endl;

                    std::cout << "Status: ";

                    switch (agendamentos[i]->getStatus())
                    {
                    case Agendamento::Status::PENDENTE:
                        std::cout << "Pendente";
                        break;

                    case Agendamento::Status::CONFIRMADO:
                        std::cout << "Confirmado";
                        break;

                    case Agendamento::Status::CANCELADO:
                        std::cout << "Cancelado";
                        break;
                    }

                    std::cout << std::endl;
                    std::cout << "-----------------------------" << std::endl;
                }
            }

            pausar();
            break;
        }

        case '0':
            limparTela();
            std::cout << "Voltando ao menu principal..." << std::endl;
            break;

        default:
            limparTela();
            std::cout << "Opcao invalida. Tente novamente." << std::endl;
            pausar();
            break;
        }

    } while (opcao != '0');
    salvarAgendamentos();
}

//================================================= VEÍCULOS =============================================

void veiculos()
{
    char opcao;

    do
    {
        limparTela();

        std::cout << "-------------- VEICULOS --------------" << std::endl;
        std::cout << "1 - Consultar veiculo" << std::endl;
        std::cout << "2 - Cadastrar veiculo" << std::endl;
        std::cout << "3 - Atualizar placa" << std::endl;
        std::cout << "4 - Registrar quilometragem" << std::endl;
        std::cout << "5 - Consultar historico de manutencoes" << std::endl;
        std::cout << "6 - Listar veiculos" << std::endl;
        std::cout << "0 - Voltar" << std::endl;

        std::cout << std::endl
                  << "Digite uma opcao: ";
        std::cin >> opcao;
        std::cin.ignore(5000, '\n');

        switch (opcao)
        {
        case '1':
        {
            limparTela();

            std::cout << "-------------- CONSULTAR VEICULO --------------" << std::endl;

            if (armazenamentoVeiculos.empty())
            {
                std::cout << "Nenhum veiculo cadastrado." << std::endl;
                pausar();
                break;
            }

            std::string placa;
            std::cout << "Digite a placa do veiculo ou '0' para voltar: ";
            std::getline(std::cin, placa);

            if (testarVolta(placa))break;

            bool encontrado = false;

            for (const auto &veiculo : armazenamentoVeiculos)
            {
                if (veiculo->getPlaca() == placa)
                {
                    std::cout << "\nVeiculo encontrado!" << std::endl;
                    std::cout << "Marca: " << veiculo->getMarca() << std::endl;
                    std::cout << "Modelo: " << veiculo->getModelo() << std::endl;
                    std::cout << "Ano: " << veiculo->getAno() << std::endl;
                    std::cout << "Placa: " << veiculo->getPlaca() << std::endl;
                    std::cout << "Quilometragem: " << veiculo->getQuilometragem() << " km" << std::endl;
                    std::cout << "Cliente: ";
                    if (veiculo->getCliente() != nullptr)
                    {
                        std::cout << veiculo->getCliente()->getNome();
                    }
                    else
                    {
                        std::cout << "Nao associado";
                    }
                    std::cout << std::endl;

                    encontrado = true;
                    break;
                }
            }

            if (!encontrado)
            {
                std::cout << "\nNenhum veiculo encontrado com essa placa." << std::endl;
            }

            pausar();
            break;
        }

        case '2':
        {
            limparTela();
            std::cout << "-------------- CADASTRAR VEICULO --------------" << std::endl;

            std::string marca, modelo, placa, cpf;
            int ano = 0, quilometragem = 0;

            std::cout << "Digite a marca do veiculo ou '0' para voltar: ";
            std::getline(std::cin, marca);

            if (testarVolta(marca)){
                pausar();
                break;
            }
            std::cout << "Digite o modelo do veiculo: ";
            std::getline(std::cin, modelo);

            std::cout << "Digite o ano do veiculo: ";
            if(verificarID(ano)){
                pausar();
                break;
            }

            std::cout << "Digite a placa do veiculo: ";
            std::getline(std::cin, placa);

            std::cout << "Digite a quilometragem do veiculo: ";
            if(verificarID(quilometragem)){
                pausar();
                break;
            }

            std::cout << "Digite o CPF do proprietario: ";
            std::getline(std::cin, cpf);

            try
            {
                Cliente *clienteEncontrado = Cliente::pesquisarPorCpf(cpf);

                if (clienteEncontrado == nullptr)
                {
                    std::cout << "\nCliente nao encontrado. Veiculo nao cadastrado." << std::endl;
                    pausar();
                    break;
                }

                // Procura um veículo com a mesma placa
                std::shared_ptr<Veiculo> veiculoEncontrado = nullptr;

                for (const auto &veiculo : armazenamentoVeiculos)
                {
                    if (veiculo->getPlaca() == placa)
                    {
                        veiculoEncontrado = veiculo;
                        break;
                    }
                }

                // Se o veículo já existe
                if (veiculoEncontrado != nullptr)
                {
                    if (veiculoEncontrado->getCliente() != nullptr)
                    {
                        throw std::invalid_argument(
                            "Ja existe um veiculo cadastrado com essa placa.");
                    }

                    // Associa o cliente ao veículo existente
                    veiculoEncontrado->associarCliente(clienteEncontrado);
                    clienteEncontrado->setVeiculos(veiculoEncontrado.get());

                    std::cout << "\nVeiculo existente associado ao cliente com sucesso!" << std::endl;
                }
                else
                {
                    // Se não existe, cria um novo veículo
                    std::shared_ptr<Veiculo> novoVeiculo =
                        std::make_shared<Veiculo>(marca, modelo, ano, placa, quilometragem);

                    novoVeiculo->associarCliente(clienteEncontrado);
                    clienteEncontrado->setVeiculos(novoVeiculo.get());

                    armazenamentoVeiculos.push_back(novoVeiculo);

                    std::cout << "\nVeiculo cadastrado com sucesso!" << std::endl;
                }
            }
            catch (const std::exception &e)
            {
                std::cout << "\nErro ao cadastrar veiculo: " << e.what() << std::endl;
            }

            pausar();
            break;
        }

        case '3':
        {
            limparTela();

            std::cout << "-------------- ATUALIZAR PLACA --------------" << std::endl;

            if (armazenamentoVeiculos.empty())
            {
                std::cout << "Nenhum veiculo cadastrado." << std::endl;
                pausar();
                break;
            }

            std::string placaAtual;

            std::cout << "Digite a placa atual do veiculo ou '0' para voltar: ";
            std::getline(std::cin, placaAtual);

            if (testarVolta(placaAtual))break;

            Veiculo *veiculoEncontrado = nullptr;

            for (const auto &veiculo : armazenamentoVeiculos)
            {
                if (veiculo->getPlaca() == placaAtual)
                {
                    veiculoEncontrado = veiculo.get();
                    break;
                }
            }

            if (veiculoEncontrado == nullptr)
            {
                std::cout << "\nVeiculo nao encontrado." << std::endl;
            }
            else
            {
                std::string novaPlaca;

                std::cout << "Digite a nova placa: ";
                std::getline(std::cin, novaPlaca);

                try
                {
                    veiculoEncontrado->atualizarInformacoes(novaPlaca);

                    std::cout << "\nPlaca atualizada com sucesso!" << std::endl;
                }
                catch (const std::exception &e)
                {
                    std::cout << "\nErro ao atualizar placa: " << e.what() << std::endl;
                }
            }

            pausar();
            break;
        }

        case '4':
        {
            limparTela();

            std::cout << "-------------- REGISTRAR QUILOMETRAGEM --------------" << std::endl;

            if (armazenamentoVeiculos.empty())
            {
                std::cout << "Nenhum veiculo cadastrado." << std::endl;
                pausar();
                break;
            }

            std::string placa;

            std::cout << "Digite a placa do veiculo ou '0' para voltar: ";
            std::getline(std::cin, placa);

            if (testarVolta(placa))break;

            Veiculo *veiculoEncontrado = nullptr;

            for (const auto &veiculo : armazenamentoVeiculos)
            {
                if (veiculo->getPlaca() == placa)
                {
                    veiculoEncontrado = veiculo.get();
                    break;
                }
            }

            if (veiculoEncontrado == nullptr)
            {
                std::cout << "\nVeiculo nao encontrado." << std::endl;
            }
            else
            {
                int novaQuilometragem = 0;

                std::cout << "Quilometragem atual: "
                          << veiculoEncontrado->getQuilometragem() << " km" << std::endl;

                std::cout << "Digite a nova quilometragem: ";
                if(verificarID(novaQuilometragem)){
                pausar();
                break;
            }

                try
                {
                    veiculoEncontrado->registrarQuilometragem(novaQuilometragem);

                    std::cout << "\nQuilometragem atualizada com sucesso!" << std::endl;
                }
                catch (const std::exception &e)
                {
                    std::cout << "\nErro ao registrar quilometragem: "
                              << e.what() << std::endl;
                }
            }

            pausar();
            break;
        }

        case '5':
        {
            limparTela();

            std::cout << "-------------- HISTORICO DE MANUTENCOES --------------"
                      << std::endl;

            if (armazenamentoVeiculos.empty())
            {
                std::cout << "Nenhum veiculo cadastrado." << std::endl;
                pausar();
                break;
            }

            std::string placa;

            std::cout << "Digite a placa do veiculo ou '0' para voltar: ";
            std::getline(std::cin, placa);

            if (testarVolta(placa))break;
            Veiculo *veiculoEncontrado = nullptr;

            for (const auto &veiculo : armazenamentoVeiculos)
            {
                if (veiculo->getPlaca() == placa)
                {
                    veiculoEncontrado = veiculo.get();
                    break;
                }
            }

            if (veiculoEncontrado == nullptr)
            {
                std::cout << "\nVeiculo nao encontrado." << std::endl;
            }
            else
            {
                std::cout << "\nVeiculo: "
                          << veiculoEncontrado->getMarca() << " "
                          << veiculoEncontrado->getModelo() << std::endl;

                std::cout << "Placa: "
                          << veiculoEncontrado->getPlaca() << std::endl;

                veiculoEncontrado->consultarHistoricoManutencoes();
            }

            pausar();
            break;
        }

        case '6':
        {
            limparTela();

            std::cout << "-------------- LISTA DE VEICULOS --------------" << std::endl;

            if (armazenamentoVeiculos.empty())
            {
                std::cout << "Nenhum veiculo cadastrado." << std::endl;
            }
            else
            {
                for (const auto &veiculo : armazenamentoVeiculos)
                {
                    std::cout << "\n----------------------------------------" << std::endl;
                    std::cout << "Marca: " << veiculo->getMarca() << std::endl;
                    std::cout << "Modelo: " << veiculo->getModelo() << std::endl;
                    std::cout << "Ano: " << veiculo->getAno() << std::endl;
                    std::cout << "Placa: " << veiculo->getPlaca() << std::endl;
                    std::cout << "Quilometragem: "
                              << veiculo->getQuilometragem() << " km" << std::endl;
                    std::cout << "Cliente: ";
                    if (veiculo->getCliente() != nullptr)
                    {
                        std::cout << veiculo->getCliente()->getNome();
                    }
                    else
                    {
                        std::cout << "Nao associado";
                    }
                    std::cout << std::endl;
                }

                std::cout << "\n----------------------------------------" << std::endl;
                std::cout << "Total de veiculos: "
                          << armazenamentoVeiculos.size() << std::endl;
            }

            pausar();
            break;
        }

        case '0':
        {
            limparTela();
            std::cout << "Voltando ao menu principal..." << std::endl;
            break;
        }

        default:
        {
            limparTela();
            std::cout << "Opcao invalida. Tente novamente." << std::endl;
            pausar();
            break;
        }
        }

    } while (opcao != '0');
    salvarVeiculos();
}

//------------------------------- PEÇAS  --------------------------------------

void pecas()
{
    Peca::carregarDoArquivo("estoque.txt");
    char opcao = ' ';

    do
    {
        limparTela(); // Utilizar essa função sempre que abrir o do {}

        std::cout << "-------------- PEÇAS --------------" << std::endl;
        std::cout << "1 - Atualizar preço" << std::endl;
        std::cout << "2 - Atualizar quantidade" << std::endl;
        std::cout << "3 - Cadastrar peça" << std::endl;
        std::cout << "4 - Consultar informações sobre peça" << std::endl;
        std::cout << "5 - Listar peças" << std::endl;
        std::cout << "6 - Listar peças em falta" << std::endl;
        std::cout << "X - Voltar" << std::endl;

        std::cout << std::endl
                  << "Digite uma opcao:" << std::endl;
        std::cin >> opcao;
        std::cin.ignore(5000, '\n'); // Limpa o buffer de entrada

        switch (opcao)
        {
        case '1':
        {
            limparTela();
            std::cout << "-------------- ATUALIZAR PREÇO --------------" << std::endl;

            Peca *atualizando = nullptr;
            std::string cod;

            do
            {
                std::cout << "Digite o código da peça que deseja alterar o preço (ou 'X' para voltar): " << std::endl;
                std::getline(std::cin, cod);

                if (cod == "0" || cod == "0")
                {
                    break; // Sai do do-while se o usuário quiser voltar
                }

                atualizando = Peca::pesquisarPorCodigo(cod);
                if (atualizando == nullptr)
                {
                    std::cout << "Não existe peça com esse código.\n"
                              << std::endl;
                }
            } while (atualizando == nullptr);

            if (atualizando == nullptr)
            {
                break;
            }

            std::cout << "Descrição: " << atualizando->getDescricao() << std::endl;
            std::cout << "Digite o novo preço da peça " << atualizando->getCodigo() << ":" << std::endl;
            float a;
            std::cin >> a;
            try
            {
                atualizando->setPreco(a);
                std::cout << "Preço atualizado com sucesso, a peça " << atualizando->getCodigo() << " custa R$" << atualizando->getPreco() << std::endl;
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Erro capturado: " << e.what() << std::endl;
            }

            break;
        }

        case '2':
        {
            limparTela();
            std::cout << "-------------- ATUALIZAR QUANTIDADE --------------" << std::endl;

            Peca *atualizando = nullptr;
            std::string cod;

            do
            {
                std::cout << "Digite o código da peça que deseja alterar a quantidade (ou 'X' para voltar): " << std::endl;
                std::getline(std::cin, cod);

                if (cod == "0" || cod == "0")
                {
                    break;
                }

                atualizando = Peca::pesquisarPorCodigo(cod);
                if (atualizando == nullptr)
                {
                    std::cout << "Não existe peça com esse código.\n"
                              << std::endl;
                }
            } while (atualizando == nullptr);

            if (atualizando == nullptr)
            {
                break;
            }

            std::cout << "Descrição: " << atualizando->getDescricao() << std::endl;
            std::cout << "Digite a nova quantidade de peças para " << atualizando->getCodigo() << ":" << std::endl;
            int novaQtd;
            std::cin >> novaQtd;
            std::cin.ignore(5000, '\n');

            try
            {
                atualizando->setQuantidade(novaQtd);
                std::cout << "Quantidade atualizada com sucesso, a peça " << atualizando->getCodigo() << " possui " << atualizando->getQuantidade() << " unidades." << std::endl;
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Erro capturado: " << e.what() << std::endl;
            }

            break;
        }

        case '3':
        {
            limparTela();
            std::cout << "-------------- CADASTRAR PEÇA --------------" << std::endl;

            char continuar = 's';
            while (continuar == 's' || continuar == 'S')
            {
                std::string cod;
                float prec;
                int quantidad;

                std::cout << "\nDigite o código da peça que deseja cadastrar: " << std::endl;
                std::getline(std::cin, cod);

                std::cout << "Digite o preço da peça: " << std::endl;
                std::cin >> prec;

                std::cout << "Digite quantas unidades temos em estoque: " << std::endl;
                std::cin >> quantidad;
                try
                {
                    Peca *novaPeca = new Peca(cod, prec, quantidad);
                    std::cout << "Peça cadastrada com sucesso! Deseja adicionar descrição? (S/N): " << std::endl;
                    char l;
                    std::cin >> l;
                    std::cin.ignore(5000, '\n');

                    if (l == 's' || l == 'S')
                    {
                        std::string descric;
                        std::cout << "Descreva a peça: " << std::endl;
                        std::getline(std::cin, descric);
                        novaPeca->setDescricao(descric);
                    }
                    std::cout << "Peça salva na memória com sucesso!\n";
                }
                catch (const std::invalid_argument &e)
                {
                    std::cout << "Erro capturado: " << e.what() << std::endl;
                }

                std::cout << "\nDeseja cadastrar outra peça? (S/N): ";
                std::cin >> continuar;
                std::cin.ignore(5000, '\n');
            }

            break;
        }

        case '4':
        {
            limparTela();
            std::cout << "-------------- CONSULTAR INFORMAÇÕES DE PEÇA --------------" << std::endl;

            Peca *consulta = nullptr;
            std::string cod;

            do
            {
                std::cout << "Digite o código da peça que deseja consultar (ou 'X' para voltar): " << std::endl;
                std::getline(std::cin, cod);

                if (cod == "X" || cod == "x")
                {
                    break;
                }

                consulta = Peca::pesquisarPorCodigo(cod);
                if (consulta == nullptr)
                {
                    std::cout << "Não existe peça com esse código.\n"
                              << std::endl;
                }
            } while (consulta == nullptr);

            if (consulta == nullptr)
            {
                break;
            }

            std::cout << "\nPeça encontrada!\n";
            std::cout << "Descrição: " << consulta->getDescricao() << std::endl;
            std::cout << "Código: " << consulta->getCodigo() << std::endl;
            std::cout << "Preço: R$ " << consulta->getPreco() << std::endl;
            std::cout << "Quantidade: " << consulta->getQuantidade() << std::endl;

            break;
        }

        case '5':
        {
            limparTela();
            Peca::listarTodas(false);
            break;
        }

        case '6':
        {
            limparTela();
            Peca::listarTodas(true);
            break;
        }

        case 'X':
        case 'x':
        {
            limparTela();
            Peca::salvarNoArquivo("estoque.txt");
            std::cout << "\nAlterações salvas com sucesso! Voltando ao menu principal..." << std::endl;
            return;
        }

        default:
        {
            limparTela();
            std::cout << "Opcao invalida. Tente novamente." << std::endl;
            break;
        }
        }

        std::cout << "\nPressione Enter para continuar...";
        std::string pausaStr;
        std::getline(std::cin, pausaStr);

    } while (true);
}

void mecanicos(std::vector<std::shared_ptr<Mecanico>> &listaDeMecanicos)
{
    char opcao = ' ';

    Mecanico::carregarMecanicosDoArquivo(listaDeMecanicos, "mecanicos.txt");

    do
    {
        limparTela();

        std::cout << "-------------- MECANICOS --------------" << std::endl;
        std::cout << "1- Cadastrar mecanico" << std::endl;
        std::cout << "2- Consultar mecanico" << std::endl;
        std::cout << "3- Alterar especialidade" << std::endl;
        std::cout << "4- Registrar diagnostico" << std::endl;
        std::cout << "5- Atualizar status" << std::endl;
        std::cout << "6- Consultar ordens atribuidas" << std::endl;
        std::cout << "7- Listar mecanicos" << std::endl;
        std::cout << "0- Voltar" << std::endl;

        std::cout << std::endl
                  << "Digite uma opcao:" << std::endl;

        std::cin >> opcao;
        std::cin.ignore(5000, '\n');

        switch (opcao)
        {
        case '1':
        {
            limparTela();
            std::cout << "-------------- CADASTRAR MECANICO --------------" << std::endl;

            std::cout << "Digite o ID do mecanico ou digite '0' para voltar: " << std::endl;
            int id = 0;
            if (verificarID(id))
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            try
            {
                if (id <= 0)
                {
                    throw std::invalid_argument("O ID do mecanico deve ser um numero positivo.");
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Erro: " << e.what() << std::endl;
                pausar();
                break;
            }

            auto it = std::find_if(listaDeMecanicos.begin(), listaDeMecanicos.end(), [id](const std::shared_ptr<Mecanico> &m)
                                   { return m->getId() == id; });

            if (it != listaDeMecanicos.end())
            {
                std::cout << "Erro: Ja existe um mecanico cadastrado com o ID " << id << "." << std::endl;
                pausar();
                break;
            }

            std::cout << "Digite o nome do mecanico ou '0' para voltar: " << std::endl;
            std::string nome;
            std::getline(std::cin, nome);

            if (testarVolta(nome))
            {
                break;
            }

            try
            {
                if (nome == "")
                {
                    throw std::invalid_argument("O nome do mecanico nao pode ser vazio.");
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Erro: " << e.what() << std::endl;
                pausar();
                break;
            }

            std::cout << "Digite a especialidade do mecanico ou '0' para voltar: " << std::endl;
            std::string especialidade;
            std::getline(std::cin, especialidade);

            if (testarVolta(especialidade))
            {
                break;
            }

            try
            {
                if (especialidade == "")
                {
                    throw std::invalid_argument("A especialidade do mecanico nao pode ser vazia.");
                }
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Erro: " << e.what() << std::endl;
                pausar();
                break;
            }

            std::shared_ptr<Mecanico> new_mecanico = std::make_shared<Mecanico>(id, nome, especialidade);

            listaDeMecanicos.push_back(new_mecanico);

            std::cout << "Mecanico cadastrado com sucesso!" << std::endl;

            pausar();
            break;
        }
        case '2':
        {
            limparTela();
            std::cout << "-------------- CONSULTAR MECANICO --------------" << std::endl;
            if (listaDeMecanicos.empty())
            {
                std::cout << "Nenhum mecanico cadastrado." << std::endl;
                pausar();
                break;
            }

            std::cout << "Digite o ID do mecanico que deseja consultar ou digite '0' para voltar: " << std::endl;
            int id = 0;
            if (verificarID(id))
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            auto it = std::find_if(listaDeMecanicos.begin(), listaDeMecanicos.end(), [id](const std::shared_ptr<Mecanico> &m)
                                   { return m->getId() == id; });

            if (it == listaDeMecanicos.end())
            {
                std::cout << "Mecanico nao encontrado." << std::endl;
                pausar();
                break;
            }

            std::cout << "Mecanico encontrado: " << it->get()->getNome() << " - " << it->get()->getEspecialidade() << std::endl;

            pausar();
            break;
        }
        case '3':
        {
            limparTela();
            std::cout << "-------------- ALTERAR ESPECIALIDADE --------------" << std::endl;

            if (listaDeMecanicos.empty())
            {
                std::cout << "Nenhum mecanico cadastrado." << std::endl;
                pausar();
                break;
            }

            std::cout << "Digite o ID do mecanico que deseja alterar a especialidade ou digite '0' para voltar: " << std::endl;
            int id = 0;
            if (verificarID(id))
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            auto it = std::find_if(listaDeMecanicos.begin(), listaDeMecanicos.end(), [id](const std::shared_ptr<Mecanico> &m)
                                   { return m->getId() == id; });

            if (it == listaDeMecanicos.end())
            {
                std::cout << "Mecanico nao encontrado." << std::endl;
                pausar();
                break;
            }

            std::cout << "Mecanico encontrado: " << it->get()->getNome() << " - " << it->get()->getEspecialidade() << std::endl;

            std::cout << "Digite a nova especialidade do mecanico ou '0' para voltar: " << std::endl;
            std::string novaEspecialidade;
            std::getline(std::cin, novaEspecialidade);
            if (testarVolta(novaEspecialidade))
            {
                break;
            }

            if (novaEspecialidade.empty())
            {
                std::cout << "Especialidade nao pode ser vazia." << std::endl;
                pausar();
                break;
            }

            it->get()->alterarEspecialidade(novaEspecialidade);
            std::cout << "Especialidade alterada com sucesso!" << std::endl;
            std::cout << "Novo registro: " << it->get()->getNome() << " - " << it->get()->getEspecialidade() << std::endl;

            pausar();
            break;
        }
        case '4':
        {
            limparTela();
            std::cout << "-------------- REGISTRAR DIAGNOSTICO --------------" << std::endl;

            if (listaDeMecanicos.empty())
            {
                std::cout << "Nenhum mecanico cadastrado." << std::endl;
                pausar();
                break;
            }

            std::cout << "Digite o ID do mecanico ou '0' para voltar: " << std::endl;
            int id = 0;
            if (verificarID(id))
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            auto it = std::find_if(listaDeMecanicos.begin(), listaDeMecanicos.end(), [id](const std::shared_ptr<Mecanico> &m)
                                   { return m->getId() == id; });

            if (it == listaDeMecanicos.end())
            {
                std::cout << "Mecanico nao encontrado." << std::endl;
                pausar();
                break;
            }

            auto ordens = (*it)->consultarOrdensAtribuidas();

            if (ordens.empty())
            {
                std::cout << "Este mecanico nao possui ordens atribuidas para diagnosticar." << std::endl;
                pausar();
                break;
            }

            std::cout << "\nOrdens atribuidas ao mecanico " << (*it)->getNome() << ":\n";
            // Lista as ordens na tela começando do número 1 para facilitar para o usuário
            for (size_t i = 0; i < ordens.size(); ++i)
            {
                std::cout << "[" << i + 1 << "] Ordem com " << ordens[i]->getServicos().size() << " servico(s)" << " atribuida ao veiculo de placa " << ordens[i]->getVeiculo()->getPlaca() << std::endl;
            }

            std::cout << "\nDigite o numero entre colchetes [ ] da ordem que deseja diagnosticar (ou 0 para cancelar): ";
            int escolha;
            std::cin >> escolha;
            std::cin.clear();
            std::cin.ignore(5000, '\n');

            if (escolha == 0)
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            size_t indiceVetor = escolha - 1; // Converte a escolha de volta para o índice padrao do vetor

            if (indiceVetor >= ordens.size())
            {
                std::cout << "Opcao invalida!" << std::endl;
                pausar();
                break;
            }

            std::cout << "Digite o diagnostico tecnico:\n";
            std::string diagnostico;
            std::getline(std::cin, diagnostico);

            try
            {
                (*it)->registrarDiagnostico(ordens[indiceVetor], diagnostico);
                std::cout << "Diagnostico registrado com sucesso!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "Erro ao registrar: " << e.what() << std::endl;
            }

            pausar();
            break;
        }
        case '5':
        {
            limparTela();
            std::cout << "-------------- ATUALIZAR STATUS --------------" << std::endl;

            if (listaDeMecanicos.empty())
            {
                std::cout << "Nenhum mecanico cadastrado." << std::endl;
                pausar();
                break;
            }

            std::cout << "Digite o ID do mecanico que deseja acessar a ordem de servico para alterar seu Status ou digite '0' para voltar: " << std::endl;
            int id = 0;
            if (verificarID(id))
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            auto it = std::find_if(listaDeMecanicos.begin(), listaDeMecanicos.end(), [id](const std::shared_ptr<Mecanico> &m)
                                   { return m->getId() == id; });

            if (it == listaDeMecanicos.end())
            {
                std::cout << "Mecanico nao encontrado." << std::endl;
                pausar();
                break;
            }

            std::cout << "Mecanico encontrado: " << it->get()->getNome() << " - " << it->get()->getEspecialidade() << std::endl;

            auto ordens = (*it)->consultarOrdensAtribuidas();

            if (ordens.empty())
            {
                std::cout << "Este mecanico nao possui ordens atribuidas para atualizar o Status." << std::endl;
                pausar();
                break;
            }

            std::cout << "\nOrdens atribuidas ao mecanico " << (*it)->getNome() << ":\n";
            // Lista as ordens na tela começando do número 1 para facilitar para o usuário
            for (size_t i = 0; i < ordens.size(); ++i)
            {
                std::cout << "[" << i + 1 << "] Ordem com " << ordens[i]->getServicos().size() << " servico(s)" << " atribuida ao veiculo de placa " << ordens[i]->getVeiculo()->getPlaca() << " com o atual Status: " << converterStatusParaString(ordens[i]->getStatus()) << std::endl;
            }

            std::cout << "\nDigite o numero entre colchetes [ ] da ordem que deseja alterar o Status (ou 0 para cancelar): ";
            int escolha;
            std::cin >> escolha;
            std::cin.clear();
            std::cin.ignore(5000, '\n');

            if (escolha == 0)
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            size_t indiceVetor = escolha - 1; // Converte a escolha de volta para o índice padrão do vetor

            if (indiceVetor >= ordens.size())
            {
                std::cout << "Opcao invalida!" << std::endl;
                pausar();
                break;
            }

            std::cout << "Digite o novo Status da ordem (0 - ABERTA, 1 - EM DIAGNOSTICO, 2 - EM EXECUCAO, 3 - AGUARDANDO PECA, 4 - AGUARDANDO RETIRADA, 5 - FINALIZADA): ";
            int novoStatus;
            if (!(std::cin >> novoStatus) || novoStatus < 0 || novoStatus > 5)
            {
                std::cin.clear();
                std::cin.ignore(5000, '\n');
                std::cout << "Status invalido." << std::endl;
                pausar();
                break;
            }
            std::cin.ignore(5000, '\n');

            try
            {
                (*it)->atualizarAndamento(ordens[indiceVetor], static_cast<StatusOrdem>(novoStatus));
                std::cout << "Status atualizado com sucesso!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "Erro ao atualizar status: " << e.what() << std::endl;
            }

            pausar();
            break;
        }
        case '6':
        {
            limparTela();
            std::cout << "-------------- CONSULTAR ORDEM ATRIBUIDA --------------" << std::endl;

            if (listaDeMecanicos.empty())
            {
                std::cout << "Nenhum mecanico cadastrado." << std::endl;
                pausar();
                break;
            }
            std::cout << "Digite o ID do mecanico que deseja consultar suas ordens atribuidas ou digite '0' para voltar: " << std::endl;
            int id = 0;
            if (verificarID(id))
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            auto it = std::find_if(listaDeMecanicos.begin(), listaDeMecanicos.end(), [id](const std::shared_ptr<Mecanico> &m)
                                   { return m->getId() == id; });

            if (it == listaDeMecanicos.end())
            {
                std::cout << "Mecanico nao encontrado." << std::endl;
                pausar();
                break;
            }

            std::cout << "Mecanico encontrado: " << it->get()->getNome() << " - " << it->get()->getEspecialidade() << std::endl;
            std::vector<OrdemDeServico *> ordens = it->get()->consultarOrdensAtribuidas();

            if (ordens.empty())
            {
                std::cout << "Nenhuma ordem atribuida a este mecanico." << std::endl;
            }
            else
            {
                for (const auto &ordem : ordens)
                {
                    std::cout << "Status da Ordem: " << converterStatusParaString(ordem->getStatus())
                              << " | Quantidade de servicos: " << ordem->getServicos().size()
                              << " | Veiculo de placa: " << ordem->getVeiculo()->getPlaca()
                              << " | Diagnostico: "
                              << (ordem->getDiagnostico().empty()
                                      ? "Nenhum diagnostico registrado."
                                      : ordem->getDiagnostico())
                              << std::endl;
                }
            }

            pausar();
            break;
        }
        case '7':
        {
            limparTela();
            std::cout << "-------------- LISTAR MECANICOS --------------" << std::endl;

            if (listaDeMecanicos.empty())
            {
                std::cout << "Nenhum mecanico cadastrado no sistema." << std::endl;
            }
            else
            {
                for (const auto &mecanico : listaDeMecanicos)
                {
                    std::cout << "ID: " << mecanico->getId()
                              << " | Nome: " << mecanico->getNome()
                              << " | Especialidade: " << mecanico->getEspecialidade() << std::endl;
                }
            }

            pausar();
            break;
        }
        case '0':
        {
            limparTela();
            try
            {
                Mecanico::salvarMecanicosNoArquivo(listaDeMecanicos, "mecanicos.txt");
                std::cout << "Dados salvos com sucesso!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << e.what() << std::endl;
                std::cout << "Por favor, verifique se o arquivo nao esta aberto em outro programa." << std::endl;
            }
            std::cout << "Voltando ao menu principal..." << std::endl;
            pausar();
            break;
        }

        default:
            limparTela();
            std::cout << "Opcao invalida. Tente novamente." << std::endl;
            pausar();
            break;
        }
    } while (opcao != '0');
}

//============================================= ORDENS DE SERVICO =============================================
// COLE ESTE BLOCO INTEIRO NO main.cpp, DEPOIS da funcao mecanicos() e ANTES do int main().
// Precisa tambem de:  #include <iomanip>  la no topo, junto com os outros includes.

std::vector<std::shared_ptr<OrdemDeServico>> armazenamentoOrdens; // Armazena as ordens de servico criadas

// Le um numero inteiro digitado pelo usuario. Retorna false se nao for um numero valido.
bool lerInteiroOrdem(int &valor)
{
    std::string entrada;
    std::getline(std::cin, entrada);

    try
    {
        std::size_t pos = 0;
        valor = std::stoi(entrada, &pos);
        return pos == entrada.size();
    }
    catch (...)
    {
        return false;
    }
}

// Mostra uma linha de resumo para cada ordem cadastrada
void mostrarResumoOrdens()
{
    for (size_t i = 0; i < armazenamentoOrdens.size(); ++i)
    {
        const auto &ordem = armazenamentoOrdens[i];

        std::cout << "[" << i + 1 << "] Placa: " << ordem->getVeiculo()->getPlaca()
                  << " | Entrada: " << ordem->getDataEntrada()
                  << " | Status: " << converterStatusParaString(ordem->getStatus())
                  << std::endl;
    }
}

// Mostra as ordens e pede para o usuario escolher uma.
// Retorna nullptr se nao houver ordens, se cancelar ou se a escolha for invalida.
std::shared_ptr<OrdemDeServico> escolherOrdem()
{
    if (armazenamentoOrdens.empty())
    {
        std::cout << "Nenhuma ordem de servico cadastrada." << std::endl;
        return nullptr;
    }

    mostrarResumoOrdens();

    std::cout << "\nDigite o numero da ordem (ou 0 para cancelar): ";
    int escolha = 0;

    if (!lerInteiroOrdem(escolha))
    {
        std::cout << "Entrada invalida." << std::endl;
        return nullptr;
    }

    if (escolha == 0)
    {
        std::cout << "Operacao cancelada." << std::endl;
        return nullptr;
    }

    if (escolha < 0 || escolha > static_cast<int>(armazenamentoOrdens.size()))
    {
        std::cout << "Numero de ordem invalido." << std::endl;
        return nullptr;
    }

    return armazenamentoOrdens[escolha - 1];
}

// Avisa e retorna true se a ordem ja foi finalizada (nao pode mais ser alterada)
bool ordemFinalizada(const std::shared_ptr<OrdemDeServico> &ordem)
{
    if (ordem->getStatus() == StatusOrdem::FINALIZADA)
    {
        std::cout << "Esta ordem ja foi finalizada e nao pode ser alterada." << std::endl;
        return true;
    }
    return false;
}

// Mostra todos os dados de uma ordem
void mostrarDetalhesOrdem(const OrdemDeServico &ordem)
{
    std::cout << std::fixed << std::setprecision(2);

    Veiculo *veiculo = ordem.getVeiculo();
    std::cout << "Veiculo: " << veiculo->getMarca() << " " << veiculo->getModelo()
              << " (placa " << veiculo->getPlaca() << ")" << std::endl;

    Cliente *cliente = veiculo->getCliente();
    std::cout << "Cliente: ";
    if (cliente != nullptr)
    {
        std::cout << cliente->getNome();
    }
    else
    {
        std::cout << "Nao associado";
    }
    std::cout << std::endl;

    std::cout << "Status: " << converterStatusParaString(ordem.getStatus()) << std::endl;
    std::cout << "Data de entrada: " << ordem.getDataEntrada() << std::endl;

    std::cout << "Data de saida: ";
    if (ordem.getDataSaida().empty())
    {
        std::cout << "ainda nao finalizada";
    }
    else
    {
        std::cout << ordem.getDataSaida();
    }
    std::cout << std::endl;

    std::cout << "Diagnostico: ";
    if (ordem.getDiagnostico().empty())
    {
        std::cout << "Nenhum diagnostico registrado.";
    }
    else
    {
        std::cout << ordem.getDiagnostico();
    }
    std::cout << std::endl;

    std::cout << "Mecanico: ";
    if (ordem.getMecanico() != nullptr)
    {
        std::cout << ordem.getMecanico()->getNome();
    }
    else
    {
        std::cout << "Nao associado";
    }
    std::cout << std::endl;

    std::cout << "\nServicos:" << std::endl;
    const std::vector<Servico *> &servicos = ordem.getServicos();
    if (servicos.empty())
    {
        std::cout << "  Nenhum servico adicionado." << std::endl;
    }
    for (size_t i = 0; i < servicos.size(); ++i)
    {
        std::cout << "  - " << servicos[i]->getNome()
                  << " | R$ " << servicos[i]->getPreco() << std::endl;
    }

    std::cout << "\nPecas:" << std::endl;
    const std::vector<Peca *> &pecasUsadas = ordem.getPecas();
    if (pecasUsadas.empty())
    {
        std::cout << "  Nenhuma peca adicionada." << std::endl;
    }
    for (size_t i = 0; i < pecasUsadas.size(); ++i)
    {
        std::cout << "  - " << pecasUsadas[i]->getCodigo()
                  << " (" << pecasUsadas[i]->getDescricao() << ")"
                  << " | R$ " << pecasUsadas[i]->getPreco() << std::endl;
    }

    std::cout << "\nValor total: R$ " << ordem.calcularValorTotal() << std::endl;
}

void ordensDeServico(std::vector<std::shared_ptr<Mecanico>> &listaDeMecanicos)
{
    char opcao;

    do
    {
        limparTela();

        std::cout << "-------------- ORDENS DE SERVICO --------------" << std::endl;
        std::cout << "1 - Abrir ordem de servico" << std::endl;
        std::cout << "2 - Listar ordens" << std::endl;
        std::cout << "3 - Consultar ordem" << std::endl;
        std::cout << "4 - Adicionar servico" << std::endl;
        std::cout << "5 - Adicionar peca" << std::endl;
        std::cout << "6 - Associar mecanico" << std::endl;
        std::cout << "7 - Atualizar status" << std::endl;
        std::cout << "8 - Calcular valor total" << std::endl;
        std::cout << "9 - Finalizar ordem" << std::endl;
        std::cout << "X - Voltar" << std::endl;

        std::cout << std::endl
                  << "Digite uma opcao: ";
        std::cin >> opcao;
        std::cin.ignore(5000, '\n');

        switch (opcao)
        {
        case '1':
        {
            limparTela();
            std::cout << "-------------- ABRIR ORDEM DE SERVICO --------------" << std::endl;

            std::string placa;
            std::cout << "Digite a placa do veiculo (ou 0 para voltar): ";
            std::getline(std::cin, placa);

            if (testarVolta(placa))
            {
                break;
            }

            // Procura o veiculo pela placa
            Veiculo *veiculoEncontrado = nullptr;

            for (const auto &veiculo : armazenamentoVeiculos)
            {
                if (veiculo->getPlaca() == placa)
                {
                    veiculoEncontrado = veiculo.get();
                    break;
                }
            }

            if (veiculoEncontrado == nullptr)
            {
                std::cout << "\nNao existe veiculo cadastrado com essa placa." << std::endl;
                pausar();
                break;
            }

            Cliente *clienteEncontrado = veiculoEncontrado->getCliente();

            if (clienteEncontrado == nullptr)
            {
                std::cout << "\nO veiculo nao possui um cliente associado." << std::endl;
                pausar();
                break;
            }

            std::string dataEntrada;
            std::cout << "Digite a data de entrada (dd/mm/aaaa): ";
            std::getline(std::cin, dataEntrada);

            if (dataEntrada.empty())
            {
                std::cout << "\nA data de entrada nao pode ser vazia." << std::endl;
                pausar();
                break;
            }

            std::string problema;
            std::cout << "Digite o problema relatado (ou aperte Enter para deixar em branco): ";
            std::getline(std::cin, problema);

            try
            {
                std::shared_ptr<OrdemDeServico> novaOrdem =
                    std::make_shared<OrdemDeServico>(veiculoEncontrado, dataEntrada);

                if (!problema.empty())
                {
                    novaOrdem->registrarDiagnostico(problema);
                }

                // Registra a ordem no cliente, para aparecer em "Consultar ordens de servico do cliente"
                clienteEncontrado->setServicos(novaOrdem.get());
                veiculoEncontrado->adicionarOrdemServico(novaOrdem.get());

                armazenamentoOrdens.push_back(novaOrdem);

                std::cout << "\nOrdem de servico aberta com sucesso!" << std::endl;
                std::cout << "Cliente: " << clienteEncontrado->getNome() << std::endl;
                std::cout << "Veiculo: " << veiculoEncontrado->getPlaca() << std::endl;
                std::cout << "Status: " << converterStatusParaString(novaOrdem->getStatus()) << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "\nErro ao abrir ordem: " << e.what() << std::endl;
            }

            pausar();
            break;
        }

        case '2':
        {
            limparTela();
            std::cout << "-------------- LISTA DE ORDENS --------------" << std::endl;

            if (armazenamentoOrdens.empty())
            {
                std::cout << "Nenhuma ordem de servico cadastrada." << std::endl;
            }
            else
            {
                mostrarResumoOrdens();
                std::cout << "\nTotal de ordens: " << armazenamentoOrdens.size() << std::endl;
            }

            pausar();
            break;
        }

        case '3':
        {
            limparTela();
            std::cout << "-------------- CONSULTAR ORDEM --------------" << std::endl;

            std::shared_ptr<OrdemDeServico> ordem = escolherOrdem();

            if (ordem == nullptr)
            {
                pausar();
                break;
            }

            limparTela();
            std::cout << "-------------- DADOS DA ORDEM --------------" << std::endl;
            mostrarDetalhesOrdem(*ordem);

            pausar();
            break;
        }

        case '4':
        {
            limparTela();
            std::cout << "-------------- ADICIONAR SERVICO --------------" << std::endl;

            std::shared_ptr<OrdemDeServico> ordem = escolherOrdem();

            if (ordem == nullptr || ordemFinalizada(ordem))
            {
                pausar();
                break;
            }

            if (armazenamentoServicos.empty())
            {
                std::cout << "\nNenhum servico cadastrado." << std::endl;
                pausar();
                break;
            }

            std::cout << std::fixed << std::setprecision(2);
            std::cout << "\nServicos disponiveis:" << std::endl;
            for (size_t i = 0; i < armazenamentoServicos.size(); ++i)
            {
                std::cout << "[" << i + 1 << "] " << armazenamentoServicos[i]->getNome()
                          << " | R$ " << armazenamentoServicos[i]->getPreco() << std::endl;
            }

            std::cout << "\nDigite o numero do servico (ou 0 para cancelar): ";
            int escolha = 0;

            if (!lerInteiroOrdem(escolha))
            {
                std::cout << "Entrada invalida." << std::endl;
                pausar();
                break;
            }

            if (escolha == 0)
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            if (escolha < 0 || escolha > static_cast<int>(armazenamentoServicos.size()))
            {
                std::cout << "Numero de servico invalido." << std::endl;
                pausar();
                break;
            }

            try
            {
                std::shared_ptr<Servico> servico = armazenamentoServicos[escolha - 1];

                ordem->adicionarServico(servico.get());
                servico->associarOrdem(ordem.get());

                std::cout << "\nServico adicionado com sucesso!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "\nErro ao adicionar servico: " << e.what() << std::endl;
            }

            pausar();
            break;
        }

        case '5':
        {
            limparTela();
            std::cout << "-------------- ADICIONAR PECA --------------" << std::endl;

            std::shared_ptr<OrdemDeServico> ordem = escolherOrdem();

            if (ordem == nullptr || ordemFinalizada(ordem))
            {
                pausar();
                break;
            }

            std::string codigo;
            std::cout << "\nDigite o codigo da peca (ou 0 para cancelar): ";
            std::getline(std::cin, codigo);

            if (testarVolta(codigo))
            {
                break;
            }

            Peca *peca = Peca::pesquisarPorCodigo(codigo);

            if (peca == nullptr)
            {
                std::cout << "\nNao existe peca com esse codigo." << std::endl;
                pausar();
                break;
            }

            if (!peca->disponivel())
            {
                std::cout << "\nPeca sem estoque suficiente." << std::endl;
                pausar();
                break;
            }

            try
            {
                // Usa uma unidade da peca: baixa o estoque e adiciona na ordem
                peca->setQuantidade(peca->getQuantidade() - 1);
                ordem->adicionarPeca(peca);

                std::cout << "\nPeca adicionada com sucesso!" << std::endl;
                std::cout << "Estoque restante: " << peca->getQuantidade() << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "\nErro ao adicionar peca: " << e.what() << std::endl;
            }

            pausar();
            break;
        }

        case '6':
        {
            limparTela();
            std::cout << "-------------- ASSOCIAR MECANICO --------------" << std::endl;

            std::shared_ptr<OrdemDeServico> ordem = escolherOrdem();

            if (ordem == nullptr || ordemFinalizada(ordem))
            {
                pausar();
                break;
            }

            if (listaDeMecanicos.empty())
            {
                std::cout << "\nNenhum mecanico cadastrado." << std::endl;
                pausar();
                break;
            }

            std::cout << "\nMecanicos cadastrados:" << std::endl;
            for (const auto &mecanico : listaDeMecanicos)
            {
                std::cout << "ID: " << mecanico->getId()
                          << " | Nome: " << mecanico->getNome()
                          << " | Especialidade: " << mecanico->getEspecialidade() << std::endl;
            }

            std::cout << "\nDigite o ID do mecanico (ou 0 para cancelar): ";
            int id = 0;

            if (!lerInteiroOrdem(id))
            {
                std::cout << "Entrada invalida." << std::endl;
                pausar();
                break;
            }

            if (id == 0)
            {
                std::cout << "Operacao cancelada." << std::endl;
                pausar();
                break;
            }

            auto it = std::find_if(listaDeMecanicos.begin(), listaDeMecanicos.end(),
                                   [id](const std::shared_ptr<Mecanico> &m)
                                   { return m->getId() == id; });

            if (it == listaDeMecanicos.end())
            {
                std::cout << "\nMecanico nao encontrado." << std::endl;
                pausar();
                break;
            }

            try
            {
                // O mecanico assume a ordem (ela passa a aparecer nas ordens dele)
                (*it)->assumirOrdem(ordem.get());

                // Garante que a ordem tambem sabe quem e o mecanico responsavel
                if (ordem->getMecanico() != it->get())
                {
                    ordem->associarMecanico(it->get());
                }

                std::cout << "\nMecanico " << (*it)->getNome()
                          << " associado a ordem com sucesso!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "\nErro ao associar mecanico: " << e.what() << std::endl;
            }

            pausar();
            break;
        }

        case '7':
        {
            limparTela();
            std::cout << "-------------- ATUALIZAR STATUS --------------" << std::endl;

            std::shared_ptr<OrdemDeServico> ordem = escolherOrdem();

            if (ordem == nullptr || ordemFinalizada(ordem))
            {
                pausar();
                break;
            }

            std::cout << "\nStatus atual: " << converterStatusParaString(ordem->getStatus()) << std::endl;
            std::cout << "\n0 - ABERTA" << std::endl;
            std::cout << "1 - EM DIAGNOSTICO" << std::endl;
            std::cout << "2 - EM EXECUCAO" << std::endl;
            std::cout << "3 - AGUARDANDO PECA" << std::endl;
            std::cout << "4 - AGUARDANDO RETIRADA" << std::endl;
            std::cout << "(Para finalizar a ordem, use a opcao 9 do menu)" << std::endl;

            std::cout << "\nDigite o novo status: ";
            int novoStatus = 0;

            if (!lerInteiroOrdem(novoStatus) || novoStatus < 0 || novoStatus > 4)
            {
                std::cout << "Status invalido." << std::endl;
                pausar();
                break;
            }

            try
            {
                ordem->alterarStatus(static_cast<StatusOrdem>(novoStatus));
                std::cout << "\nStatus atualizado para "
                          << converterStatusParaString(ordem->getStatus()) << "!" << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "\nErro ao atualizar status: " << e.what() << std::endl;
            }

            pausar();
            break;
        }

        case '8':
        {
            limparTela();
            std::cout << "-------------- CALCULAR VALOR TOTAL --------------" << std::endl;

            std::shared_ptr<OrdemDeServico> ordem = escolherOrdem();

            if (ordem == nullptr)
            {
                pausar();
                break;
            }

            std::cout << std::fixed << std::setprecision(2);
            std::cout << "\nValor total da ordem: R$ " << ordem->calcularValorTotal() << std::endl;

            pausar();
            break;
        }

        case '9':
        {
            limparTela();
            std::cout << "-------------- FINALIZAR ORDEM --------------" << std::endl;

            std::shared_ptr<OrdemDeServico> ordem = escolherOrdem();

            if (ordem == nullptr || ordemFinalizada(ordem))
            {
                pausar();
                break;
            }

            std::cout << "\nStatus atual: " << converterStatusParaString(ordem->getStatus()) << std::endl;

            std::string dataSaida;
            std::cout << "Digite a data de saida/entrega do veiculo (dd/mm/aaaa): ";
            std::getline(std::cin, dataSaida);

            if (dataSaida.empty())
            {
                std::cout << "\nA data de saida nao pode ser vazia." << std::endl;
                pausar();
                break;
            }

            try
            {
                ordem->finalizar(dataSaida);

                std::cout << std::fixed << std::setprecision(2);
                std::cout << "\nOrdem finalizada com sucesso!" << std::endl;
                std::cout << "Valor total: R$ " << ordem->calcularValorTotal() << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cout << "\nErro ao finalizar ordem: " << e.what() << std::endl;
                std::cout << "(A ordem precisa estar com status AGUARDANDO RETIRADA.)" << std::endl;
            }

            pausar();
            break;
        }

        case 'X':
        case 'x':
        {
            limparTela();
            std::cout << "Voltando ao menu principal..." << std::endl;
            break;
        }

        default:
        {
            limparTela();
            std::cout << "Opcao invalida. Tente novamente." << std::endl;
            pausar();
            break;
        }
        }

    } while (opcao != 'X' && opcao != 'x');
}

int main()
{

    std::vector<std::shared_ptr<Mecanico>> listaDeMecanicos;

    Cliente::carregarDoArquivo("listaClientes.txt");
    carregarVeiculos();
    carregarAgendamentos();
    carregarServicos();

    cliente();
    veiculos();
    agendamento();
    ordensDeServico(listaDeMecanicos);
    pecas();
    mecanicos(listaDeMecanicos);
    servicos();

    salvarAgendamentos();
    salvarVeiculos();
    salvarServicos();

    return 0;
}
