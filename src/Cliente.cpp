

#include "Cliente.hpp"
#include "Veiculo.hpp"
#include "OrdemDeServico.hpp"
#include "Agendamento.hpp"
#include <iostream>
#include <string>
#include <vector>

Cliente::Cliente(std::string nome, std::string cpf, std::string contato, std::string endereco)
    : nome(nome), cpf(cpf), contato(contato), endereco(endereco){}

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