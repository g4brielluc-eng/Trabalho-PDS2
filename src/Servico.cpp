/**
 * @file Servico.cpp
 * @brief Implementação da classe Servico.
 */

#include "Servico.hpp"
#include "OrdemDeServico.hpp"
#include <stdexcept>

Servico::Servico(const std::string& nomeServico,
                  const std::string& descricaoServico,
                  float precoServico,
                  int duracaoMinutos) {
    if (nomeServico.empty()) {
        throw std::invalid_argument("O nome do servico nao pode ser vazio.");
    }
    if (precoServico < 0) {
        throw std::invalid_argument("O preco do servico nao pode ser negativo.");
    }
    if (duracaoMinutos <= 0) {
        throw std::invalid_argument("A duracao estimada deve ser maior que zero.");
    }

    nome = nomeServico;
    descricao = descricaoServico;
    preco = precoServico;
    duracaoEstimada = duracaoMinutos;
}

const std::string& Servico::getNome() const {
    return nome;
}

const std::string& Servico::getDescricao() const {
    return descricao;
}

float Servico::getPreco() const {
    return preco;
}

int Servico::getDuracaoEstimada() const {
    return duracaoEstimada;
}

const std::vector<OrdemDeServico*>& Servico::getOrdens() const {
    return ordens;
}

void Servico::atualizarDescricao(const std::string& novaDescricao) {
    descricao = novaDescricao;
}

void Servico::atualizarPreco(float novoPreco) {
    if (novoPreco < 0) {
        throw std::invalid_argument("O novo preco nao pode ser negativo.");
    }
    preco = novoPreco;
}

void Servico::registrarDuracaoEstimada(int minutos) {
    if (minutos <= 0) {
        throw std::invalid_argument("A duracao estimada deve ser maior que zero.");
    }
    duracaoEstimada = minutos;
}

std::string Servico::consultarInformacoes() const {
    return nome + " - " + descricao +
           " | Preco: " + std::to_string(preco) +
           " | Duracao estimada: " + std::to_string(duracaoEstimada) + " min";
}

void Servico::associarOrdem(OrdemDeServico* ordem) {
    if (ordem == nullptr) {
        throw std::invalid_argument("A ordem de servico nao pode ser nula.");
    }
    for (OrdemDeServico* o : ordens) {
        if (o == ordem) {
            return;
        }
    }
    ordens.push_back(ordem);
}

float Servico::calcularCusto(int quantidade) const {
    if (quantidade < 1) {
        throw std::invalid_argument("A quantidade deve ser pelo menos 1.");
    }
    return preco * quantidade;
}
