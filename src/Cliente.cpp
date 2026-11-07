

#include "Cliente.hpp"
#include "Veiculo.hpp"
#include "OrdemDeServico.hpp"
#include "Agendamento.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>  

std::vector<Cliente*> Cliente::listaClientes;


Cliente::Cliente(std::string nome, std::string cpf, std::string contato, std::string endereco)
    : nome(nome), cpf(cpf), contato(contato), endereco(endereco){

        listaClientes.push_back(this);

    }


Cliente::Cliente(){

    std::string nome,cpf,contato,endereco;
                                    
    std::cout << "\nDigite o nome do cliente: " << std::endl;
    std::getline(std::cin, nome);

    std::cout << "Digite o CPF do cliente: " << std::endl;
    std::getline(std::cin, cpf);

    std::cout << "Digite o endereço do cliente: " << std::endl;
    std::getline(std::cin, endereco);

    std::cout << "Digite o contato do cliente: " << std::endl;
    std::getline(std::cin, contato); 

    std::cin.ignore(5000, '\n');

    try {
            this -> nome = nome;
            this -> cpf = cpf;
            this-> contato= contato;
            this -> endereco = endereco;
                        
            std::cout << this->nome << " cadastrado com sucesso! " << std::endl;
    } catch (const std::invalid_argument& e) {
                    std::cout << "Erro: " << e.what() << std::endl;
    }

    listaClientes.push_back(this);

}


void Cliente::listarTodas() {
    if (listaClientes.empty()) {
        std::cout << "Nenhuma cliente cadastrado no sistema.\n";
        return;
    }

  
    std::cout << "\n-------------- LISTA DE CLIENTES --------------\n";
    for (const auto& p : listaClientes){
        std::cout << "Nome: " << p->getNome() 
            << " | CPF: " << p->getCpf() 
            << " | Endereço: " << p->getEndereco() 
            << " | Contato: " << p->getContato() << "\n";
        }
    std::cout << "--------------------------------\n";
}


Cliente* Cliente::pesquisarPorCpf(const std::string& cpfBuscado) {
            for (const auto& p : listaClientes) {
                if (p->getCpf() == cpfBuscado) {
                    return p; 
                }
            }
            return nullptr;
        }
     
void Cliente::salvarNoArquivo(const std::string& nomeArquivo) {
        std::ofstream arquivo(nomeArquivo);
        if (arquivo.is_open()) {
                for (const auto& p : listaClientes) {
                    arquivo << p->nome << "," 
                            << p->cpf << "," 
                            << p->contato << "," 
                            << p->endereco << "\n";
                }
                arquivo.close();
                std::cout << "Clientes salvos com sucesso em '" << nomeArquivo << "'!\n";
            } else {
                std::cout << "Erro ao abrir o arquivo para escrita!\n";
            }
        }



        void Cliente::carregarDoArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    std::string linha;

    if (arquivo.is_open()) {
        int carregados = 0;
        while (std::getline(arquivo, linha)) {
            std::stringstream ss(linha);
            std::string nome, cpf, contato, endereco; 

            std::getline(ss, nome, ',');
            std::getline(ss, cpf, ',');
            std::getline(ss, contato, ',');
            std::getline(ss, endereco);

            if (!nome.empty() && !cpf.empty()) {
                bool jaExiste = false;
                for (const auto& p : listaClientes) {
                    if (p->getCpf() == cpf) {
                        jaExiste = true;
                        break;
                    }
                }

                if (!jaExiste) {
                    new Cliente(nome, cpf, contato, endereco);
                    carregados++;
                }
            }
        }
        
        arquivo.close();
        std::cout << carregados << " Clientes carregados do arquivo com sucesso!\n";
    } else {
        std::cout << "Nenhum cliente anterior encontrado. Iniciando com lista vazia.\n";
    }
}
std::string Cliente::getNome(){
    return this->nome;
}

std::string Cliente::getCpf(){
    return this->cpf;
}

std::string Cliente::getEndereco(){
    return this->endereco;
}

void Cliente::setContato(std::string contato){
    this->contato = contato;
}

std::string Cliente::getContato(){
    return this->contato;
}

std::vector<Veiculo*>& Cliente::getVeiculos(){
    return this->veiculos;
}
    

void Cliente::setVeiculos(Veiculo* veiculo){
    this->veiculos.push_back(veiculo);
}


std::vector<OrdemDeServico*>& Cliente::getServicos(){
    return this->servicos;
}

void Cliente::setServicos(OrdemDeServico* ordemdeservico){
    this->servicos.push_back(ordemdeservico);
}
  

void Cliente::setAgendamento(Agendamento* agendamento){
    this->agendamentos.push_back(agendamento);
}

Cliente::~Cliente() {}