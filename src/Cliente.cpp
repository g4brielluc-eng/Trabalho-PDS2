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
#include <algorithm>
#include <cctype>

std::vector<Cliente*> Cliente::listaClientes;


void Cliente::ValidarTexto(const std::string& texto, char tipo) {
    
    if (texto.empty()) {
            throw std::invalid_argument("O texto não pode ser vazio.");
    }

    for (char c : texto) {
        if (tipo == 'T' && !std::isalpha((unsigned char)c) && c != ' ') {
            throw std::invalid_argument("Só são aceitos letras.");
        }
        if (tipo == 'N' && !std::isdigit((unsigned char)c) && c != ' ') {
             throw std::invalid_argument("Só são aceitos números.");
        }
    }
}

    
    void validarCpf(const std::vector<Cliente*>& lista, const std::string& cpf) {
        for (Cliente* c : lista) {
            if (c->getCpf() == cpf) {
                throw std::invalid_argument("Já existe um cliente com esse CPF.");
            }
        }
    }


    std::string lerLinha(const std::string& mensagem) {
        std::cout << mensagem << std::endl;
        std::string linha;
        if (!std::getline(std::cin, linha)) {
            throw std::runtime_error("Entrada encerrada durante o cadastro.");
        }
        return linha;
    }



Cliente::Cliente(std::string nome, std::string cpf, std::string contato, std::string endereco)
    : nome(nome),
      cpf(cpf),
      contato(contato),
      endereco(endereco) {

    
    ValidarTexto(nome, 'T');
    ValidarTexto(cpf, 'N');
    ValidarTexto(contato, 'N');
        
    
    validarCpf(listaClientes, this->cpf);

    listaClientes.push_back(this);
}


Cliente::Cliente() {

    while (true) {
        std::string entrada = lerLinha("\nDigite o nome do cliente: ");
        try {
                ValidarTexto(entrada, 'T');            
                this->nome = entrada;
                break;
        } catch (const std::invalid_argument& e) {
            std::cout << "Erro: " << e.what() << std::endl;
        }
    }

    while (true) {
        std::string entrada = lerLinha("Digite o CPF do cliente: ");
        try {
            ValidarTexto(entrada, 'N');
            validarCpf(listaClientes, entrada);
            this->cpf = entrada;
            break;
        } catch (const std::invalid_argument& e) {
            std::cout << "Erro: " << e.what() << std::endl;
        }
    }

    while (true) {
        std::string entrada = lerLinha("Digite o endereço do cliente: ");
        try {
            this->endereco = entrada;
            break;
        } catch (const std::invalid_argument& e) {
            std::cout << "Erro: " << e.what() << std::endl;
        }
    }

    while (true) {
        std::string entrada = lerLinha("Digite o contato do cliente: ");
        try {
            ValidarTexto(entrada, 'N');
            this->contato = entrada;
            break;
        } catch (const std::invalid_argument& e) {
            std::cout << "Erro: " << e.what() << std::endl;
        }
    }

    listaClientes.push_back(this);
    std::cout << this->nome << " cadastrado com sucesso! " << std::endl;
}

void Cliente::excluirCliente() {

    listaClientes.erase(
        std::remove(listaClientes.begin(), listaClientes.end(), this),
        listaClientes.end());

}

Cliente::~Cliente() {
   Cliente::excluirCliente();
}


void Cliente::listarTodas() {
    if (listaClientes.empty()) {
        std::cout << "Nenhum cliente cadastrado no sistema.\n";
        return;
    }

    std::cout << "\n-------------- LISTA DE CLIENTES --------------\n";
    for (Cliente* p : listaClientes) {
    std::cout << "Nome: " << p->getNome()
              << " | CPF: " << p->getCpf()
              << " | Endereço: " << p->getEndereco()
              << " | Contato: " << p->getContato()
              << " | Ordens de serviço: " << p->getServicos().size() << "\n";
}
    std::cout << "--------------------------------\n";
}


Cliente* Cliente::pesquisarPorCpf(const std::string& cpfBuscado) {
    ValidarTexto(cpfBuscado, 'N');
    const std::string cpf = cpfBuscado;

    for (Cliente* p : listaClientes) {
        if (p->getCpf() == cpf) {
            return p;
        }
    }
    return nullptr;
}


void Cliente::salvarNoArquivo(const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cout << "Erro ao abrir o arquivo para escrita!\n";
        return;
    }

    for (Cliente* p : listaClientes) {
        arquivo << p->nome << ","
                << p->cpf << ","
                << p->contato << ","
                << p->endereco << "\n";
    }
    arquivo.close();

    if (arquivo.fail()) {
        std::cout << "Erro ao gravar os clientes em '" << nomeArquivo << "'!\n";
        return;
    }
    std::cout << "Clientes salvos com sucesso em '" << nomeArquivo << "'!\n";
}


void Cliente::carregarDoArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cout << "Nenhum cliente anterior encontrado. Iniciando com lista vazia.\n";
        return;
    }

    std::string linha;
    int numeroLinha = 0;
    int carregados = 0;
    int ignorados = 0;

    while (std::getline(arquivo, linha)) {
        numeroLinha++;
        

        std::stringstream ss(linha);
        std::string nome, cpf, contato, endereco;

        std::getline(ss, nome, ',');
        std::getline(ss, cpf, ',');
        std::getline(ss, contato, ',');
        std::getline(ss, endereco);

        try {
            if (pesquisarPorCpf(cpf) != nullptr) {
            continue;   // já carregado antes
            }
            new Cliente(nome, cpf, contato, endereco);
            carregados++;
        } catch (const std::invalid_argument& e) {
            ignorados++;
            std::cerr << "Linha " << numeroLinha << " ignorada: " << e.what() << "\n";
        }
    }

    arquivo.close();
    std::cout << carregados << " Clientes carregados do arquivo com sucesso!\n";
    if (ignorados > 0) {
        std::cout << ignorados << " linha(s) ignorada(s) por dados inválidos ou duplicados.\n";
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
    ValidarTexto(contato, 'N');
    this->contato = contato;
}

std::string Cliente::getContato(){
    return this->contato;
}

std::vector<Veiculo*>& Cliente::getVeiculos(){
    return this->veiculos;
}

void Cliente::setVeiculos(Veiculo* veiculo){
    if (veiculo == nullptr) {
        throw std::invalid_argument("Veículo inválido.");
    }
    if (std::find(veiculos.begin(), veiculos.end(), veiculo) == veiculos.end()) {
        this->veiculos.push_back(veiculo);
    }
}

std::vector<OrdemDeServico*>& Cliente::getServicos(){
    return this->servicos;
}

void Cliente::setServicos(OrdemDeServico* ordemdeservico){
    if (ordemdeservico == nullptr) {
        throw std::invalid_argument("Ordem de serviço inválida.");
    }
    if (std::find(servicos.begin(), servicos.end(), ordemdeservico) == servicos.end()) {
        this->servicos.push_back(ordemdeservico);
    }
}

void Cliente::listarServicos() {
    if (servicos.empty()) {
        std::cout << "Nenhuma ordem de serviço desse cliente cadastrada no sistema.\n";
        return;
    }

    std::cout << "\n-------------- ORDENS DE SERVIÇO --------------\n";
    for (OrdemDeServico* o : servicos) {
        std::string placa = " ";
        if (o->getVeiculo() != nullptr) {
            placa = o->getVeiculo()->getPlaca();
        }
        std::cout << "Veículo de placa: " << placa
                  << " | Qtd. serviços: " << o->getServicos().size()
                  << " | Diagnóstico: "
                  << (o->getDiagnostico().empty() ? "Nenhum" : o->getDiagnostico())
                  << "\n";
    }
    std::cout << "--------------------------------\n";
}

void Cliente::setAgendamento(Agendamento* agendamento){
    if (agendamento == nullptr) {
        throw std::invalid_argument("Agendamento inválido.");
    }
    if (std::find(agendamentos.begin(), agendamentos.end(), agendamento) == agendamentos.end()) {
        this->agendamentos.push_back(agendamento);
    }
}
