#include "OrdemDeServico.hpp"

#include <stdexcept>

#include "Veiculo.hpp"
#include "Servico.hpp"
#include "Peca.hpp"
#include "Mecanico.hpp"

OrdemDeServico::OrdemDeServico(Veiculo* veiculo, const std::string& dataEntrada)
    : status(StatusOrdem::ABERTA),
      dataEntrada(dataEntrada),
      veiculo(nullptr),
      mecanico(nullptr) {
    associarVeiculo(veiculo);
}

void OrdemDeServico::associarVeiculo(Veiculo* veiculo) {
    if (veiculo == nullptr) {
        throw std::invalid_argument("Veiculo nao pode ser nulo");
    }
    this->veiculo = veiculo;
    veiculo->adicionarOrdemServico(this);
}

void OrdemDeServico::adicionarServico(Servico* servico) {
    if (servico == nullptr) {
        throw std::invalid_argument("Servico nao pode ser nulo");
    }
    servicos.push_back(servico);
}

void OrdemDeServico::adicionarPeca(Peca* peca) {
    if (peca == nullptr) {
        throw std::invalid_argument("Peca nao pode ser nula");
    }
    pecas.push_back(peca);
}

void OrdemDeServico::alterarStatus(StatusOrdem novoStatus) {
    if (novoStatus == StatusOrdem::FINALIZADA) {
        throw std::invalid_argument("Use finalizar() para finalizar a ordem");
    }
    status = novoStatus;
}

void OrdemDeServico::registrarDiagnostico(const std::string& diagnostico) {
    this->diagnostico = diagnostico;
}

float OrdemDeServico::calcularValorTotal() const {
    float total = 0.0f;
    for (std::size_t i = 0; i < servicos.size(); ++i) {
        total += servicos[i]->getPreco();
    }
    for (std::size_t i = 0; i < pecas.size(); ++i) {
        total += pecas[i]->getPreco();
    }
    return total;
}

void OrdemDeServico::definirDataEntrada(const std::string& data) {
    dataEntrada = data;
}

void OrdemDeServico::associarMecanico(Mecanico* mecanico) {
    if (mecanico == nullptr) {
        throw std::invalid_argument("Mecanico nao pode ser nulo");
    }
    this->mecanico = mecanico;
}

void OrdemDeServico::finalizar(const std::string& dataSaida) {
    if (status != StatusOrdem::AGUARDANDO_RETIRADA) {
        throw std::logic_error("A ordem precisa estar AGUARDANDO_RETIRADA");
    }
    status = StatusOrdem::FINALIZADA;
    this->dataSaida = dataSaida;
}

StatusOrdem OrdemDeServico::getStatus() const { return status; }
const std::string& OrdemDeServico::getDiagnostico() const { return diagnostico; }
const std::string& OrdemDeServico::getDataEntrada() const { return dataEntrada; }
const std::string& OrdemDeServico::getDataSaida() const { return dataSaida; }
Veiculo* OrdemDeServico::getVeiculo() const { return veiculo; }
Mecanico* OrdemDeServico::getMecanico() const { return mecanico; }
const std::vector<Servico*>& OrdemDeServico::getServicos() const { return servicos; }
const std::vector<Peca*>& OrdemDeServico::getPecas() const { return pecas; }