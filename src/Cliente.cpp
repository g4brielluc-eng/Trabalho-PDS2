

#include "Cliente.hpp"
#include <iostream>
#include <string>
#include <vector>

Cliente::Cliente(std::string nome, std::string cpf, std::string contato, std::string endereco)
    : nome(nome) 
{
    this->cpf = cpf;    
    this->contato = contato;
    this->endereco = endereco;
}

void Cliente::setContato(std::string contato){
    this->contato = contato;
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
