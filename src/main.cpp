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

//============================================= Cliente =============================================

void cliente()
{
    Cliente::carregarDoArquivo("listaClientes.txt");
    char opcao;

    do
    {
        limparTela(); // Utilizar essa função sempre que abrir o do {}

        std::cout << "-------------- CLIENTE --------------" << std::endl;
        std::cout << "1 - Cadastrar Ciente" << std::endl;
        std::cout << "2 - Consultar agendamentos do cliente" << std::endl;
        std::cout << "3 - Consultar informações de cliente" << std::endl;
        std::cout << "4 - Consultar ordens de servico do cliente" << std::endl;
        std::cout << "5 - Consultar veiculos do cliente" << std::endl;
        std::cout << "6 - Listar clientes" << std::endl;
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
            std::cout << "-------------- CADASTRAR CLIENTE --------------" << std::endl;

            char continuar = 's';
            while (continuar == 's' || continuar == 'S')
            {
                try
                {
                    Cliente *novocliente = new Cliente();
                }
                catch (const std::invalid_argument &e)
                {
                    std::cout << "Erro capturado: " << e.what() << std::endl;
                }

                std::cout << "\nDeseja cadastrar outro cliente? (S/N): ";
                std::cin >> continuar;
                std::cin.ignore(5000, '\n');
            }

            break;
        }

        case '2':
        {
            limparTela();
            std::cout << "-------------- CONSULTAR AGENDAMENTO --------------" << std::endl;

            // NÃO FAÇO IDEIA DE COMO FAZER ISSO
            break;
        }

        case '3':
        {
            limparTela();
            std::cout << "-------------- CONSULTAR INFORMAÇÕES DE CLIENTE --------------" << std::endl;

            Cliente *consulta = nullptr;
            std::string cod;

            do
            {
                std::cout << "Digite o cpf do cliente que deseja consultar (ou 'X' para voltar): " << std::endl;
                std::getline(std::cin, cod);

                if (cod == "X" || cod == "x")
                {
                    break;
                }

                consulta = Cliente::pesquisarPorCpf(cod);
                if (consulta == nullptr)
                {
                    std::cout << "Não existe cliente com esse cpf.\n"
                              << std::endl;
                }
            } while (consulta == nullptr);

            if (consulta == nullptr)
            {
                break;
            }

            std::cout << "\n Cliente encontrado!\n";
            std::cout << "Nome: " << consulta->getNome() << std::endl;
            std::cout << "Cpf: " << consulta->getCpf() << std::endl;
            std::cout << "Contato: R$ " << consulta->getContato() << std::endl;
            std::cout << "Endereco: " << consulta->getEndereco() << std::endl;

            break;
        }

        case '4':
        {
            limparTela();
            std::cout << "-------------- CONSULTAR ORDENS DE SERVIÇO DO CLIENTE --------------" << std::endl;
            // NÃO FAÇO IDEIA DE COMO FAZER ISSO

            break;
        }

        case '5':
        {
            limparTela();
            std::cout << "-------------- CONSULTAR VEICULOS DO CLIENTE --------------" << std::endl;
            // NÃO FAÇO IDEIA DE COMO FAZER ISSO
            break;
        }

        case '6':
        {
            limparTela();
            Cliente::listarTodas();
            break;
        }

        case 'X':
        case 'x':
        {
            limparTela();
            Cliente::salvarNoArquivo("listaClientes.txt");
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
        std::cout << "X- Voltar" << std::endl;

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

            std::cout << "Digite a data (dd/mm/aaaa) ou digite 'X' para voltar: " << std::endl;
            std::string data;
            std::getline(std::cin, data);

            if (data == "X" || data == "x")
            {
                std::cout << "Operacao cancelada" << std::endl;
                pausar();
                break; // Sai do case '1' e volta ao menu de agendamentos
            }

            std::cout << "Digite o horario (hh:mm) ou digite 'X' para voltar: " << std::endl;
            std::string horario;
            std::getline(std::cin, horario);

            if (horario == "X" || horario == "x")
            {
                std::cout << "Operacao cancelada" << std::endl;
                pausar(); // Usar essa função sempre antes do break
                break;
            }

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

            std::cout << "Digite a data do agendamento: ";
            std::getline(std::cin, data);

            std::cout << "Digite o horario do agendamento: ";
            std::getline(std::cin, horario);

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

            size_t indice;

            std::cout << std::endl
                      << "Digite o numero do agendamento: ";
            std::cin >> indice;
            std::cin.ignore(5000, '\n');

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

            std::cout << "\nDigite o numero do agendamento que deseja confirmar: ";
            std::cin >> indice;
            std::cin.ignore(5000, '\n');

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

            std::cout << "\nDigite o numero do agendamento que deseja cancelar: ";
            std::cin >> indice;
            std::cin.ignore(5000, '\n');

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

        case 'X':
        case 'x':
            limparTela();
            std::cout << "Voltando ao menu principal..." << std::endl;
            break;

        default:
            limparTela();
            std::cout << "Opcao invalida. Tente novamente." << std::endl;
            pausar();
            break;
        }

    } while (opcao != 'X' && opcao != 'x');
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
        std::cout << "3 - Associar cliente ao veiculo" << std::endl;
        std::cout << "4 - Atualizar placa" << std::endl;
        std::cout << "5 - Registrar quilometragem" << std::endl;
        std::cout << "6 - Consultar historico de manutencoes" << std::endl;
        std::cout << "7 - Listar veiculos" << std::endl;
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

            std::cout << "-------------- CONSULTAR VEICULO --------------" << std::endl;

            if (armazenamentoVeiculos.empty())
            {
                std::cout << "Nenhum veiculo cadastrado." << std::endl;
                pausar();
                break;
            }

            std::string placa;
            std::cout << "Digite a placa do veiculo: ";
            std::getline(std::cin, placa);

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
            int ano, quilometragem;

            std::cout << "Digite a marca do veiculo: ";
            std::getline(std::cin, marca);

            std::cout << "Digite o modelo do veiculo: ";
            std::getline(std::cin, modelo);

            std::cout << "Digite o ano do veiculo: ";
            std::cin >> ano;
            std::cin.ignore(5000, '\n');

            std::cout << "Digite a placa do veiculo: ";
            std::getline(std::cin, placa);

            std::cout << "Digite a quilometragem do veiculo: ";
            std::cin >> quilometragem;
            std::cin.ignore(5000, '\n');

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
            // Associar cliente ao veiculo
            break;
        }

        case '4':
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

            std::cout << "Digite a placa atual do veiculo: ";
            std::getline(std::cin, placaAtual);

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

        case '5':
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

            std::cout << "Digite a placa do veiculo: ";
            std::getline(std::cin, placa);

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
                int novaQuilometragem;

                std::cout << "Quilometragem atual: "
                          << veiculoEncontrado->getQuilometragem() << " km" << std::endl;

                std::cout << "Digite a nova quilometragem: ";
                std::cin >> novaQuilometragem;
                std::cin.ignore(5000, '\n');

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

        case '6':
        {
            // Consultar historico de manutencoes
            break;
        }

        case '7':
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

int main()
{

    std::vector<std::shared_ptr<Mecanico>> listaDeMecanicos;

    Cliente::carregarDoArquivo("listaClientes.txt");
    carregarVeiculos();
    carregarAgendamentos();

    cliente();
    veiculos();
    agendamento();
    // pecas();
    // mecanicos(listaDeMecanicos);

    salvarAgendamentos();
    salvarVeiculos();

    return 0;
}
