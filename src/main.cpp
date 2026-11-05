// MOSTRA O MENU
        // Escreve na tela as opções de C,M,S,L,P


// X sempre volta pra tela anterior 

// Lê a entrada digitada
    // C -> Verificar clientes
        // 1 -> Consultar clientes
            //  a -> Listar suas ordens de serviço
            //  b -> Listar seus Agendamentos 
        // 2 -> Criar cliente
            
    // M -> Verificar mecânicos
        // 1 -> Consultar mecânico
        // 2 -> Adicionar mecânico

    // S -> Verificar ordens de serviço
        // 1-> Listar ordens de serviço
        // 2-> Cria ordem de serviço

    // L -> Verificar peças
        // 1 -> Pesquisar por peça
            // a -> Atualizar Peça
            // b -> Excluir Peça
        // 2 -> Peças em falta
        // 3 -> Adicionar Peça
    // P -> Verificar agendamentos
        // 1 -> listar agendamentos
        // 2 -> adicionar agendamentos

//Vai pra tela da letra digitada
//Pede pra digitar o número






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


CLIENTES -- Rickelve

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

int main() {
    agendamento();
    return 0;
}
    