#include "Agendamento.hpp"
#include <algorithm>
#include <stdexcept>

std::vector<Agendamento*> Agendamento::agendamentos;

Agendamento::Agendamento(const std::string& data, const std::string& horario)
    : data(data), horario(horario), cliente(nullptr), veiculo(nullptr), servico(nullptr), status(Status::PENDENTE) {
        if (data.empty() || horario.empty()) {
        throw std::invalid_argument("Data e horario nao podem ser vazios.");
        }
    }

Agendamento::~Agendamento() {
    auto it = std::find(agendamentos.begin(), agendamentos.end(), this);

    if (it != agendamentos.end()) {
        agendamentos.erase(it);
    }
}

void Agendamento::registrarAgendamento() {
    auto it = std::find(agendamentos.begin(), agendamentos.end(), this);

    if (it != agendamentos.end()) {
        throw std::logic_error("Este agendamento ja esta registrado.");
    }
    if (!verificarDisponibilidade(data, horario)) {
        throw std::logic_error("Horario indisponivel para agendamento.");
    }
    agendamentos.push_back(this);
}

void Agendamento::associarCliente(Cliente* cliente) {
    if (cliente == nullptr) {
        throw std::invalid_argument("Cliente invalido.");
    }
    this->cliente = cliente;
}

void Agendamento::associarVeiculo(Veiculo* veiculo) {
    if (veiculo == nullptr) {
        throw std::invalid_argument("Veiculo invalido.");
    }
    this->veiculo = veiculo;
}

void Agendamento::definirServico(Servico* servico) {
    if (servico == nullptr) {
        throw std::invalid_argument("Servico invalido.");
    }
    this->servico = servico;
}

void Agendamento::alterarDataHorario(const std::string& data, const std::string& horario) {
    if (data.empty() || horario.empty()) {
        throw std::invalid_argument("Data e horario nao podem ser vazios.");
    }
    if (this->data == data && this->horario == horario) {
        throw std::logic_error("A data e o horario informados sao iguais aos atuais.");
    }
    if (!verificarDisponibilidade(data, horario)) {
        throw std::logic_error("Horario indisponivel para agendamento.");
    }
    this->data = data;
    this->horario = horario;
}

void Agendamento::confirmar() {
    if (status == Status::CANCELADO) {
        throw std::logic_error("Nao e possivel confirmar um agendamento cancelado.");
    }
    status = Status::CONFIRMADO;
}

void Agendamento::cancelar() {
    if (status == Status::CANCELADO) {
        throw std::logic_error("O agendamento ja esta cancelado.");
    }

    status = Status::CANCELADO;
}

const std::vector<Agendamento*>& Agendamento::consultarAgendamentos() {
    return agendamentos;
}

bool Agendamento::verificarDisponibilidade(const std::string& data, const std::string& horario) {
    if (data.empty() || horario.empty()) {
        throw std::invalid_argument("Data e horario nao podem ser vazios.");
    }

    for (Agendamento* agendamento : agendamentos) {
        if (agendamento->getData() == data && agendamento->getHorario() == horario && agendamento->getStatus() != Status::CANCELADO) {
            return false;
        }
    }
    return true;
}

const std::string& Agendamento::getData() const {
    return data;
}

const std::string& Agendamento::getHorario() const {
    return horario;
}

Agendamento::Status Agendamento::getStatus() const {
    return status;
}

Cliente* Agendamento::getCliente() const {
    return cliente;
}

Veiculo* Agendamento::getVeiculo() const {
    return veiculo;
}

Servico* Agendamento::getServico() const {
    return servico;
}