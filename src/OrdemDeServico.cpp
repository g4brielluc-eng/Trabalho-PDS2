#include "OrdemDeServico.hpp"

OrdemDeServico::OrdemDeServico(Veiculo* veiculo, const std::string& dataEntrada)
    : status(StatusOrdem::ABERTA),
      dataEntrada(dataEntrada),
      veiculo(veiculo),
      mecanico(nullptr) {}

void OrdemDeServico::associarVeiculo(Veiculo* veiculo) {}
void OrdemDeServico::adicionarServico(Servico* servico) {}
void OrdemDeServico::adicionarPeca(Peca* peca) {}
void OrdemDeServico::alterarStatus(StatusOrdem novoStatus) {}
void OrdemDeServico::registrarDiagnostico(const std::string& diagnostico) {}
float OrdemDeServico::calcularValorTotal() const { return 0.0f; }
void OrdemDeServico::definirDataEntrada(const std::string& data) {}
void OrdemDeServico::associarMecanico(Mecanico* mecanico) {}
void OrdemDeServico::finalizar(const std::string& dataSaida) {}

StatusOrdem OrdemDeServico::getStatus() const { return status; }
const std::string& OrdemDeServico::getDiagnostico() const { return diagnostico; }
const std::string& OrdemDeServico::getDataEntrada() const { return dataEntrada; }
const std::string& OrdemDeServico::getDataSaida() const { return dataSaida; }
Veiculo* OrdemDeServico::getVeiculo() const { return veiculo; }
Mecanico* OrdemDeServico::getMecanico() const { return mecanico; }
const std::vector<Servico*>& OrdemDeServico::getServicos() const { return servicos; }
const std::vector<Peca*>& OrdemDeServico::getPecas() const { return pecas; }