#include "Veiculo.hpp"
#include "OrdemDeServico.hpp"
#include <iostream>
#include <stdexcept>
#include <cctype>
#include <algorithm>

Veiculo::Veiculo(const std::string& marca, const std::string& modelo, int ano, const std::string& placa, int quilometragem)
    : marca(marca), modelo(modelo), ano(ano), placa(placa), quilometragem(quilometragem), cliente(nullptr) {

    if (marca.empty()) {
        throw std::invalid_argument("A marca do veiculo nao pode ser vazia.");
    }

    if (modelo.empty()) {
        throw std::invalid_argument("O modelo do veiculo nao pode ser vazio.");
    }

    if (ano <= 1883) {
        throw std::invalid_argument("O ano do veiculo deve ser positivo.");
    }

    if (placa.empty()) {
        throw std::invalid_argument("A placa do veiculo nao pode ser vazia.");
    }

    if (quilometragem < 0) {
        throw std::invalid_argument("A quilometragem nao pode ser negativa.");
    }
}


void Veiculo::associarCliente(Cliente* cliente) {
    if (cliente == nullptr) {
        throw std::invalid_argument("O cliente nao pode ser nulo.");
    }
    this->cliente = cliente;
}

void Veiculo::atualizarInformacoes(const std::string& placa) {
    if (placa.empty()) {
        throw std::invalid_argument("A placa nao pode ser vazia.");
    }

    this->placa = placa;
}

void Veiculo::registrarQuilometragem(int quilometragem) {
    if (quilometragem < this->quilometragem) {
        throw std::invalid_argument(
            "A quilometragem nao pode ser menor que a atual."
        );
    }

    this->quilometragem = quilometragem;
}

void Veiculo::adicionarOrdemServico(OrdemDeServico* ordem) {
    if (ordem == nullptr) {
        throw std::invalid_argument("A ordem de servico nao pode ser nula.");
    }

    if (std::find(ordensServico.begin(), ordensServico.end(), ordem)
        != ordensServico.end()) {
        throw std::logic_error("Essa ordem de servico ja esta associada ao veiculo.");
    }

    ordensServico.push_back(ordem);
}

const std::vector<OrdemDeServico*>& Veiculo::consultarOrdensServico() const {
    return ordensServico;
}

void Veiculo::consultarHistoricoManutencoes() const {
    if (ordensServico.empty()) {
        std::cout << "Nenhuma ordem de servico registrada para este veiculo."
                  << std::endl;
        return;
    }

    for (const OrdemDeServico* ordem : ordensServico) {
        std::cout << std::endl << "--- Ordem de Servico ---" << std::endl;

        std::cout << "Data de entrada: "
                  << ordem->getDataEntrada() << std::endl;

        if (ordem->getDataSaida().empty()) {
            std::cout << "Data de saida: Ainda nao finalizada" << std::endl;
        } else {
            std::cout << "Data de saida: "
                      << ordem->getDataSaida() << std::endl;
        }

        std::cout << "Diagnostico: "
                  << ordem->getDiagnostico() << std::endl;

        std::cout << "Status: ";

        switch (ordem->getStatus()) {
            case StatusOrdem::ABERTA:
                std::cout << "Aberta";
                break;

            case StatusOrdem::EM_DIAGNOSTICO:
                std::cout << "Em diagnostico";
                break;

            case StatusOrdem::EM_EXECUCAO:
                std::cout << "Em execucao";
                break;

            case StatusOrdem::AGUARDANDO_PECA:
                std::cout << "Aguardando peca";
                break;

            case StatusOrdem::AGUARDANDO_RETIRADA:
                std::cout << "Aguardando retirada";
                break;

            case StatusOrdem::FINALIZADA:
                std::cout << "Finalizada";
                break;
        }

        std::cout << std::endl;

        std::cout << "Valor total: R$ "
                  << ordem->calcularValorTotal() << std::endl;
    }
}

const std::string& Veiculo::getMarca() const {
    return marca;
}

const std::string& Veiculo::getModelo() const {
    return modelo;
}

int Veiculo::getAno() const {
    return ano;
}

const std::string& Veiculo::getPlaca() const {
    return placa;
}

int Veiculo::getQuilometragem() const {
    return quilometragem;
}

Cliente* Veiculo::getCliente() const {
    return cliente;
}