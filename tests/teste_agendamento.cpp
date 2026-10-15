#include "doctest.h"
#include "Agendamento.hpp"
#include "Cliente.hpp"
#include "Veiculo.hpp"
#include "Servico.hpp"

TEST_CASE("Verificar dados iniciais") {
    Agendamento a("20/10/2026", "14:00");

    CHECK(a.getData() == "20/10/2026");
    CHECK(a.getHorario() == "14:00");
    CHECK(a.getStatus() == Agendamento::Status::PENDENTE);
    CHECK(a.getCliente() == nullptr);
    CHECK(a.getVeiculo() == nullptr);
    CHECK(a.getServico() == nullptr);
}

TEST_CASE("Alterar data e horario") {
    Agendamento a("20/10/2026", "14:00");

    a.alterarDataHorario("21/10/2026", "15:00");

    CHECK(a.getData() == "21/10/2026");
    CHECK(a.getHorario() == "15:00");
}

TEST_CASE("Associar cliente") {
    Cliente c("Arthur", "12345678900", "31999999999", "Rua Argueles");
    Agendamento a("20/10/2026", "14:00");

    a.associarCliente(&c);

    CHECK(a.getCliente() == &c);
}

TEST_CASE("Associar veiculo") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);
    Agendamento a("20/10/2026", "14:00");

    a.associarVeiculo(&v);

    CHECK(a.getVeiculo() == &v);
}

TEST_CASE("Definir servico") {
    Servico s("Troca de oleo", "Troca de oleo do motor", 150.0f, 30);
    Agendamento a("20/10/2026", "14:00");

    a.definirServico(&s);

    CHECK(a.getServico() == &s);
}

TEST_CASE("Confirmar agendamento") {
    Agendamento a("20/10/2026", "14:00");

    a.confirmar();

    CHECK(a.getStatus() == Agendamento::Status::CONFIRMADO);
}

TEST_CASE("Cancelar agendamento") {
    Agendamento a("20/10/2026", "14:00");

    a.cancelar();

    CHECK(a.getStatus() == Agendamento::Status::CANCELADO);
}
