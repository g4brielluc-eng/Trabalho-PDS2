#include "Mecanico.hpp"
#include "Status_Ordem.hpp"
#include "OrdemDeServico.hpp"
#include "Servico.hpp"

#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

Mecanico::Mecanico(int id, const std::string& nome, const std::string& especialidade)
    : id_(id), nome_(nome), especialidade_(especialidade) {} //Construtor da classe.

int Mecanico::getId() const {
    return id_;
}    

std::string Mecanico::getNome() const {
    return nome_;
}

std::string Mecanico::getEspecialidade() const {
    return especialidade_;
}

void Mecanico::alterarEspecialidade(const std::string& especialidade) {
    especialidade_ = especialidade;
}

std::vector<OrdemDeServico*> Mecanico::consultarOrdensAtribuidas() const {
    return ordens_;
}

void Mecanico::assumirOrdem(OrdemDeServico* ordem) { 
    if(ordem == nullptr) {
        throw std::invalid_argument("A ordem de servico nao pode ser nula.");
    }
    else {
        ordens_.push_back(ordem); //Cria uma posição nova no vetor e adiciona a ordem na lista de ordens atribuídas ao mecânico.
    }
}

void Mecanico::registrarDiagnostico(OrdemDeServico* ordem, const std::string& diagnostico) {
    if(std::find(ordens_.begin(), ordens_.end(), ordem) == ordens_.end()) {
        throw std::logic_error("Ordem de servico nao atribuida a este mecanico.");
    }
    else {
        ordem->registrarDiagnostico(diagnostico);
    }
}

void Mecanico::registrarServicoRealizado(OrdemDeServico* ordem, Servico* servico) {
   if(std::find(ordens_.begin(), ordens_.end(), ordem) == ordens_.end()) { //find procura a ordem no vetor de ordens atribuídas ao mecânico. Se não encontrar, lança exceção.
        throw std::logic_error("Ordem de servico nao atribuida a este mecanico.");
    }
    else if(servico == nullptr) {
        throw std::invalid_argument("Servico nao pode ser nulo.");
    }
    else { 
        ordem->adicionarServico(servico);
    }
}

void Mecanico::atualizarAndamento(OrdemDeServico* ordem, StatusOrdem novoStatus) {
    if(std::find(ordens_.begin(), ordens_.end(), ordem) == ordens_.end()) { 
        throw std::logic_error("Ordem de servico nao atribuida a este mecanico.");
    }
    else if(novoStatus == StatusOrdem::FINALIZADA) {
        throw std::invalid_argument("O mecanico nao pode finalizar a ordem de servico.");
    }
    else {
        ordem->alterarStatus(novoStatus);
    }
}