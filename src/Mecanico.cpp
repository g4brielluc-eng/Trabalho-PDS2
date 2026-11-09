#include "Mecanico.hpp"
#include "Status_Ordem.hpp"
#include "OrdemDeServico.hpp"
#include "Servico.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>

Mecanico::Mecanico(int id, const std::string& nome, const std::string& especialidade)
    : id(id), nome(nome), especialidade(especialidade) {
    if (id <= 0) {
        throw std::invalid_argument("O ID do mecanico deve ser um numero positivo.");
    }
    if (nome == "") {
        throw std::invalid_argument("O nome do mecanico nao pode ser vazio.");
    }
    if (especialidade == "") {
        throw std::invalid_argument("A especialidade do mecanico nao pode ser vazia.");
    }
}

int Mecanico::getId() const {
    return id;
}    

const std::string& Mecanico::getNome() const {
    return nome;
}

const std::string& Mecanico::getEspecialidade() const {
    return especialidade;
}

void Mecanico::alterarEspecialidade(const std::string& especialidade_nova) {
    especialidade = especialidade_nova;
}

std::vector<OrdemDeServico*> Mecanico::consultarOrdensAtribuidas() const {
    return ordens;
}

void Mecanico::assumirOrdem(OrdemDeServico* ordem) { 
    if(ordem == nullptr) {
        throw std::invalid_argument("A ordem de servico nao pode ser nula.");
    }
    else {
        ordens.push_back(ordem); //Cria uma posição nova no vetor e adiciona a ordem na lista de ordens atribuídas ao mecânico.
    }
}

void Mecanico::registrarDiagnostico(OrdemDeServico* ordem, const std::string& diagnostico) {
    if(std::find(ordens.begin(), ordens.end(), ordem) == ordens.end()) {
        throw std::logic_error("Ordem de servico nao atribuida a este mecanico.");
    }
    else {
        ordem->registrarDiagnostico(diagnostico);
    }
}

void Mecanico::registrarServicoRealizado(OrdemDeServico* ordem, Servico* servico) {
   if(std::find(ordens.begin(), ordens.end(), ordem) == ordens.end()) { //find procura a ordem no vetor de ordens atribuídas ao mecânico. Se não encontrar, lança exceção.
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
    if(std::find(ordens.begin(), ordens.end(), ordem) == ordens.end()) { 
        throw std::logic_error("Ordem de servico nao atribuida a este mecanico.");
    }
    else if(novoStatus == StatusOrdem::FINALIZADA) {
        throw std::invalid_argument("O mecanico nao pode finalizar a ordem de servico.");
    }
    else {
        ordem->alterarStatus(novoStatus);
    }
}

void Mecanico::carregarMecanicosDoArquivo(std::vector<std::shared_ptr<Mecanico>>& lista, const std::string& nomeArquivo) {

    std::ifstream arquivo(nomeArquivo);
    std::string linha;

    if (arquivo.is_open()) {
        while (std::getline(arquivo, linha)) {
            std::stringstream ss(linha);
            std::string idStr, nome, especialidade;

            std::getline(ss, idStr, ';');
            std::getline(ss, nome, ';');
            std::getline(ss, especialidade, ';');

            if (!idStr.empty() && !nome.empty() && !especialidade.empty()) {
                try {
                    int id = std::stoi(idStr);

                    auto it = std::find_if(lista.begin(), lista.end(), [id](const std::shared_ptr<Mecanico>& m) {
                        return m->getId() == id;
                    });
                    
                    if (it == lista.end()) {
                        auto novoMecanico = std::make_shared<Mecanico>(id, nome, especialidade);
                        lista.push_back(novoMecanico);
                    }
                } catch (...) { 
                    continue; 
                }
            }
        }
        arquivo.close();
    }
}

void Mecanico::salvarMecanicosNoArquivo(const std::vector<std::shared_ptr<Mecanico>>& lista, const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        throw std::runtime_error("Erro critico: Falha de permissao ao salvar " + nomeArquivo);
    }

    for (const auto& mecanico : lista) {
        arquivo << mecanico->getId() << ";" 
                << mecanico->getNome() << ";" 
                << mecanico->getEspecialidade() << "\n";
    }
    arquivo.close();
}
