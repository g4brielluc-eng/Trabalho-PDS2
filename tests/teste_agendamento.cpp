#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Agendamento.hpp"
#include "Cliente.hpp"
#include "Veiculo.hpp"
#include "Servico.hpp"

#include <stdexcept>

TEST_CASE("Verificar dados iniciais") {
    Agendamento a("20/10/2026", "14:00");

    CHECK(a.getData() == "20/10/2026");
    CHECK(a.getHorario() == "14:00");
    CHECK(a.getStatus() == Agendamento::Status::PENDENTE);
    CHECK(a.getCliente() == nullptr);
    CHECK(a.getVeiculo() == nullptr);
    CHECK(a.getServico() == nullptr);
}

TEST_CASE("Impedir data ou horario vazio no construtor") {
    CHECK_THROWS_AS(Agendamento("", "14:00"), std::invalid_argument);
    CHECK_THROWS_AS(Agendamento("20/10/2026", ""), std::invalid_argument);
}

TEST_CASE("Alterar data e horario") {
    Agendamento a("20/10/2026", "14:00");

    a.alterarDataHorario("21/10/2026", "15:00");

    CHECK(a.getData() == "21/10/2026");
    CHECK(a.getHorario() == "15:00");
}

TEST_CASE("Impedir alteracao para data ou horario vazio") {
    Agendamento a("20/10/2026", "14:00");

    CHECK_THROWS_AS(
        a.alterarDataHorario("", "15:00"),
        std::invalid_argument
    );

    CHECK_THROWS_AS(
        a.alterarDataHorario("21/10/2026", ""),
        std::invalid_argument
    );

    CHECK(a.getData() == "20/10/2026");
    CHECK(a.getHorario() == "14:00");
}

TEST_CASE("Impedir alteracao para a mesma data e horario") {
    Agendamento a("20/10/2026", "14:00");

    CHECK_THROWS_AS(
        a.alterarDataHorario("20/10/2026", "14:00"),
        std::logic_error
    );
}

TEST_CASE("Associar cliente") {
    Cliente c("Arthur", "12345678900", "31999999999", "Rua Argueles");
    Agendamento a("20/10/2026", "14:00");

    a.associarCliente(&c);

    CHECK(a.getCliente() == &c);
}

TEST_CASE("Impedir associacao de cliente nulo") {
    Agendamento a("20/10/2026", "14:00");

    CHECK_THROWS_AS(
        a.associarCliente(nullptr),
        std::invalid_argument
    );

    CHECK(a.getCliente() == nullptr);
}

TEST_CASE("Associar veiculo") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);
    Agendamento a("20/10/2026", "14:00");

    a.associarVeiculo(&v);

    CHECK(a.getVeiculo() == &v);
}

TEST_CASE("Impedir associacao de veiculo nulo") {
    Agendamento a("20/10/2026", "14:00");

    CHECK_THROWS_AS(
        a.associarVeiculo(nullptr),
        std::invalid_argument
    );

    CHECK(a.getVeiculo() == nullptr);
}

TEST_CASE("Definir servico") {
    Servico s("Troca de oleo", "Troca de oleo do motor", 150.0f, 30);
    Agendamento a("20/10/2026", "14:00");

    a.definirServico(&s);

    CHECK(a.getServico() == &s);
}

TEST_CASE("Impedir associacao de servico nulo") {
    Agendamento a("20/10/2026", "14:00");

    CHECK_THROWS_AS(
        a.definirServico(nullptr),
        std::invalid_argument
    );

    CHECK(a.getServico() == nullptr);
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

TEST_CASE("Impedir confirmar agendamento cancelado") {
    Agendamento a("20/10/2026", "14:00");

    a.cancelar();

    CHECK_THROWS_AS(a.confirmar(), std::logic_error);
    CHECK(a.getStatus() == Agendamento::Status::CANCELADO);
}

TEST_CASE("Impedir cancelar agendamento duas vezes") {
    Agendamento a("20/10/2026", "14:00");

    a.cancelar();

    CHECK_THROWS_AS(a.cancelar(), std::logic_error);
}

TEST_CASE("Registrar agendamento") {
    Agendamento a("20/10/2026", "14:00");

    a.registrarAgendamento();

    const auto& lista = Agendamento::consultarAgendamentos();

    REQUIRE(lista.size() == 1);
    CHECK(lista[0] == &a);
}

TEST_CASE("Impedir registrar o mesmo agendamento duas vezes") {
    Agendamento a("20/10/2026", "14:00");

    a.registrarAgendamento();

    CHECK_THROWS_AS(a.registrarAgendamento(), std::logic_error);
    CHECK(Agendamento::consultarAgendamentos().size() == 1);
}

TEST_CASE("Impedir dois agendamentos no mesmo horario") {
    Agendamento a1("20/10/2026", "14:00");
    Agendamento a2("20/10/2026", "14:00");

    a1.registrarAgendamento();

    CHECK_THROWS_AS(a2.registrarAgendamento(), std::logic_error);
    CHECK(Agendamento::consultarAgendamentos().size() == 1);
}

TEST_CASE("Consultar agendamentos registrados") {
    Agendamento a1("20/10/2026", "14:00");
    Agendamento a2("21/10/2026", "15:00");

    a1.registrarAgendamento();
    a2.registrarAgendamento();

    const auto& lista = Agendamento::consultarAgendamentos();

    REQUIRE(lista.size() == 2);
    CHECK(lista[0] == &a1);
    CHECK(lista[1] == &a2);
}

TEST_CASE("Verificar disponibilidade de horario livre") {
    Agendamento a("20/10/2026", "14:00");

    CHECK(Agendamento::verificarDisponibilidade("20/10/2026", "14:00"));
}

TEST_CASE("Verificar indisponibilidade de horario ocupado") {
    Agendamento a("20/10/2026", "14:00");

    a.registrarAgendamento();

    CHECK_FALSE(
        Agendamento::verificarDisponibilidade("20/10/2026", "14:00")
    );
}

TEST_CASE("Horario fica disponivel apos cancelamento") {
    Agendamento a("20/10/2026", "14:00");

    a.registrarAgendamento();
    a.cancelar();

    CHECK(
        Agendamento::verificarDisponibilidade("20/10/2026", "14:00")
    );
}

TEST_CASE("Impedir alteracao para horario ocupado") {
    Agendamento a1("20/10/2026", "14:00");
    Agendamento a2("20/10/2026", "15:00");

    a1.registrarAgendamento();
    a2.registrarAgendamento();

    CHECK_THROWS_AS(
        a1.alterarDataHorario("20/10/2026", "15:00"),
        std::logic_error
    );

    CHECK(a1.getHorario() == "14:00");
}

TEST_CASE("Permitir alterar para horario liberado por cancelamento") {
    Agendamento a1("20/10/2026", "14:00");
    Agendamento a2("20/10/2026", "15:00");

    a1.registrarAgendamento();
    a2.registrarAgendamento();

    a2.cancelar();

    a1.alterarDataHorario("20/10/2026", "15:00");

    CHECK(a1.getHorario() == "15:00");
}

TEST_CASE("Impedir verificar disponibilidade com dados vazios") {
    CHECK_THROWS_AS(
        Agendamento::verificarDisponibilidade("", "14:00"),
        std::invalid_argument
    );

    CHECK_THROWS_AS(
        Agendamento::verificarDisponibilidade("20/10/2026", ""),
        std::invalid_argument
    );
}

TEST_CASE("Remover agendamento do vetor ao destruir objeto") {
    Agendamento* a = new Agendamento("20/10/2026", "14:00");

    a->registrarAgendamento();

    REQUIRE(Agendamento::consultarAgendamentos().size() == 1);

    delete a;

    CHECK(Agendamento::consultarAgendamentos().empty());
}
