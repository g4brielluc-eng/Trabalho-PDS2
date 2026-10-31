#include "Veiculo.hpp"
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





// TODO: Implementar a consulta do histórico de manutenções
// após a disponibilização de um método público em OrdemDeServico
// que permita acessar e exibir os dados de cada ordem.





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