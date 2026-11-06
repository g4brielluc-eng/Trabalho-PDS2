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

#include "Agendamento.hpp"
#include "Peca.hpp"
#include "Cliente.hpp"
#include "Veiculo.hpp"
#include "OrdemDeServico.hpp"
#include "Mecanico.hpp"
#include "Status_Ordem.hpp"
#include "Servico.hpp"


// Função para limpar a tela do console
void limparTela() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

// Função para pausar a execução até que o usuário pressione Enter
void pausar() {
    std::cout << std::endl << "Pressione Enter para continuar...";
    std::cin.get();  
    
}


void agendamento() {

    char opcao;
    
    do {

        limparTela(); //Utilizar essa função sempre que abrir o do {}

        std::cout << "-------------- AGENDAMENTOS --------------" << std::endl;
        std::cout << "1- Verificar disponibilidade" << std::endl;
        std::cout << "2- Criar agendamento" << std::endl;
        std::cout << "3- Alterar data e horario" << std::endl;
        std::cout << "4- Confirmar agendamento" << std::endl;
        std::cout << "5- Cancelar agendamento" << std::endl;
        std::cout << "6- Consultar agendamentos" << std::endl;
        std::cout << "X- Voltar" << std::endl;

        std::cout << std::endl << "Digite uma opcao:" << std::endl;

        std::cin >> opcao;
        std::cin.ignore(5000, '\n'); // Limpa o buffer de entrada 
        
        switch (opcao) {
            case '1': { 
                limparTela(); //Utilizar essa função sempre que abrir um case novo 
                std::cout << "Verificando disponibilidade" << std::endl;

                std::cout << "Digite a data (dd/mm/aaaa) ou digite 'X' para voltar: "<< std::endl;
                std::string data;
                std::getline(std::cin, data);

                if (data == "X" || data == "x") {
                    std::cout << "Operacao cancelada" << std::endl;
                    pausar();
                    break; // Sai do case '1' e volta ao menu de agendamentos
                }

                std::cout << "Digite o horario (hh:mm) ou digite 'X' para voltar: " << std::endl;
                std::string horario;
                std::getline(std::cin, horario);  

                if (horario == "X" || horario == "x") {
                    std::cout << "Operacao cancelada" << std::endl;
                    pausar(); //Usar essa função sempre antes do break
                    break; 
                }

                try {
                    bool disponivel = Agendamento::verificarDisponibilidade(data, horario);
                    if (disponivel) {
                        std::cout << "O horario " << horario << " do dia " << data << " esta disponivel." << std::endl;
                    } else {
                        std::cout << "O horario " << horario << " do dia " << data << " nao esta disponivel." << std::endl;
                    }
                } catch (const std::invalid_argument& e) {
                    std::cout << "Erro: " << e.what() << std::endl;
                }
                
                pausar();
                break;
            } 

            case '2':
                limparTela();
                std::cout << "Criando agendamento" << std::endl;
                pausar();
                break;

            case '3':
                limparTela();
                std::cout << "Alterando data e horario" << std::endl;
                pausar();
                break;

            case '4':
                limparTela();
                std::cout << "Confirmando agendamento" << std::endl;
                pausar();
                break;

            case '5':
                limparTela();
                std::cout << "Cancelando agendamento" << std::endl;
                pausar();
                break;

            case '6':
                limparTela();
                std::cout << "Consultando agendamentos" << std::endl;
                pausar();
                break;

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

    } while(opcao != 'X' && opcao != 'x');
}



//-------------- PEÇAS  --------------

void pecas() {
    Peca::carregarDoArquivo("estoque.txt");
    char opcao;
    
    do {
        limparTela(); // Utilizar essa função sempre que abrir o do {}

        std::cout << "-------------- PEÇAS --------------" << std::endl;
        std::cout << "1 - Atualizar preço" << std::endl;
        std::cout << "2 - Atualizar quantidade" << std::endl;
        std::cout << "3 - Cadastrar peça" << std::endl;
        std::cout << "4 - Consultar informações sobre peça" << std::endl;
        std::cout << "5 - Listar peças" << std::endl;
        std::cout << "6 - Listar peças em falta" << std::endl;
        std::cout << "X - Voltar" << std::endl;

        std::cout << std::endl << "Digite uma opcao:" << std::endl;
        std::cin >> opcao;
        std::cin.ignore(5000, '\n'); // Limpa o buffer de entrada 
        
        switch (opcao) {
            case '1': { 
                limparTela(); 
                std::cout << "-------------- ATUALIZAR PREÇO --------------" << std::endl;
                
                Peca* atualizando = nullptr;
                std::string cod;
                
                do {
                    std::cout << "Digite o código da peça que deseja alterar o preço (ou 'X' para voltar): " << std::endl;
                    std::getline(std::cin, cod);

                    if (cod == "X" || cod == "x") {
                        break; // Sai do do-while se o usuário quiser voltar
                    }

                    atualizando = Peca::pesquisarPorCodigo(cod);
                    if (atualizando == nullptr) {
                        std::cout << "Não existe peça com esse código.\n" << std::endl;
                    }
                } while (atualizando == nullptr);

                if (atualizando == nullptr) {
                    break;
                }
                
                std::cout << "Descrição: " << atualizando->getDescricao() << std::endl;
                std::cout << "Digite o novo preço da peça " << atualizando->getCodigo() << ":" << std::endl;
                float a;
                std::cin >> a;
                std::cin.ignore(5000, '\n');

                try {
                    atualizando->setPreco(a);
                    std::cout << "Preço atualizado com sucesso, a peça " << atualizando->getCodigo() << " custa R$" << atualizando->getPreco() << std::endl;
                } catch (const std::invalid_argument& e) {
                    std::cout << "Erro capturado: " << e.what() << std::endl;
                }
                
                break;
            }

            case '2': {
                limparTela();
                std::cout << "-------------- ATUALIZAR QUANTIDADE --------------" << std::endl;
                
                Peca* atualizando = nullptr;
                std::string cod;
                
                do {
                    std::cout << "Digite o código da peça que deseja alterar a quantidade (ou 'X' para voltar): " << std::endl;
                    std::getline(std::cin, cod);

                    if (cod == "X" || cod == "x") {
                        break;
                    }

                    atualizando = Peca::pesquisarPorCodigo(cod);
                    if (atualizando == nullptr) {
                        std::cout << "Não existe peça com esse código.\n" << std::endl;
                    }
                } while (atualizando == nullptr);

                if (atualizando == nullptr) {
                    break;
                }
                
                std::cout << "Descrição: " << atualizando->getDescricao() << std::endl;
                std::cout << "Digite a nova quantidade de peças para " << atualizando->getCodigo() << ":" << std::endl;
                int novaQtd;
                std::cin >> novaQtd;
                std::cin.ignore(5000, '\n');

                try {
                    atualizando->setQuantidade(novaQtd);
                    std::cout << "Quantidade atualizada com sucesso, a peça " << atualizando->getCodigo() << " possui " << atualizando->getQuantidade() << " unidades." << std::endl;
                } catch (const std::invalid_argument& e) {
                    std::cout << "Erro capturado: " << e.what() << std::endl;
                }
                
                break;
            }

            case '3': { 
                limparTela();
                std::cout << "-------------- CADASTRAR PEÇA --------------" << std::endl;
                
                char continuar = 's';
                while (continuar == 's' || continuar == 'S') {
                    std::string cod;
                    float prec;
                    int quantidad;
                    
                    std::cout << "\nDigite o código da peça que deseja cadastrar: " << std::endl;
                    std::getline(std::cin, cod);

                    std::cout << "Digite o preço da peça: " << std::endl;
                    std::cin >> prec;

                    std::cout << "Digite quantas unidades temos em estoque: " << std::endl;
                    std::cin >> quantidad;
                    std::cin.ignore(5000, '\n');

                    try {
                        Peca* novaPeca = new Peca(cod, prec, quantidad);
                        std::cout << "Peça cadastrada com sucesso! Deseja adicionar descrição? (S/N): " << std::endl;
                        char l;
                        std::cin >> l;
                        std::cin.ignore(5000, '\n');

                        if (l == 's' || l == 'S') {
                            std::string descric;
                            std::cout << "Descreva a peça: " << std::endl;
                            std::getline(std::cin, descric);
                            novaPeca->setDescricao(descric);
                        }
                        std::cout << "Peça salva na memória com sucesso!\n";
                    } 
                    catch (const std::invalid_argument& e) {
                        std::cout << "Erro capturado: " << e.what() << std::endl;
                    }

                    std::cout << "\nDeseja cadastrar outra peça? (S/N): ";
                    std::cin >> continuar;
                    std::cin.ignore(5000, '\n');
                }
                
                break; 
            }

            case '4': {
                limparTela();
                std::cout << "-------------- CONSULTAR INFORMAÇÕES DE PEÇA --------------" << std::endl;
                
                Peca* consulta = nullptr;
                std::string cod;
                
                do {
                    std::cout << "Digite o código da peça que deseja consultar (ou 'X' para voltar): " << std::endl;
                    std::getline(std::cin, cod);

                    if (cod == "X" || cod == "x") {
                        break;
                    }

                    consulta = Peca::pesquisarPorCodigo(cod);
                    if (consulta == nullptr) {
                        std::cout << "Não existe peça com esse código.\n" << std::endl;
                    }
                } while (consulta == nullptr);

                if (consulta == nullptr) {
                    break;
                }
                
                std::cout << "\nPeça encontrada!\n";
                std::cout << "Descrição: " << consulta->getDescricao() << std::endl;
                std::cout << "Código: " << consulta->getCodigo() << std::endl;
                std::cout << "Preço: R$ " << consulta->getPreco() << std::endl;
                std::cout << "Quantidade: " << consulta->getQuantidade() << std::endl;
                
                break;
            }

            case '5': {
                limparTela();
                Peca::listarTodas(false);
                break;
            }

            case '6': {
                limparTela();
                Peca::listarTodas(true);
                break;
            }

            case 'X':
            case 'x': {
                limparTela();
                Peca::salvarNoArquivo("estoque.txt"); 
                std::cout << "\nAlterações salvas com sucesso! Voltando ao menu principal..." << std::endl;
                return;
            }

            default: {
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

int main() {

    pecas();
    return 0;
}
    