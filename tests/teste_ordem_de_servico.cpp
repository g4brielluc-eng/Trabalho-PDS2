#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "OrdemDeServico.hpp"
#include "Veiculo.hpp"
#include "Mecanico.hpp"
#include "Servico.hpp"
#include <stdexcept>

TEST_CASE("OrdemDeServico: Criar ordem de servico") {
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    CHECK(ordem.getStatus() == StatusOrdem::ABERTA);
    CHECK(ordem.getDataEntrada() == "06/10/2026");
    CHECK(ordem.getVeiculo() == &v);
    CHECK(ordem.getMecanico() == nullptr);
    CHECK(ordem.getDiagnostico().empty());
    CHECK(ordem.getDataSaida().empty());
    CHECK(ordem.getServicos().empty());
    CHECK(ordem.getPecas().empty());
}

TEST_CASE("OrdemDeServico: Associar veiculo") {
    Veiculo v1("FIAT", "Palio", 2010, "ABC1234", 150000);
    Veiculo v2("VW", "Gol", 2015, "DEF5678", 100000);

    OrdemDeServico ordem(&v1, "06/10/2026");
    ordem.associarVeiculo(&v2);

    CHECK(ordem.getVeiculo() == &v2);
}

TEST_CASE("OrdemDeServico: Associar mecanico") {
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    Mecanico m(123456, "Rickelve", "Freios");

    OrdemDeServico ordem(&v, "06/10/2026");
    ordem.associarMecanico(&m);

    CHECK(ordem.getMecanico() == &m);
}

TEST_CASE("OrdemDeServico: Adicionar servico") {
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    Servico s("Troca de oleo", "Troca de oleo do motor", 100.00, 60);

    OrdemDeServico ordem(&v, "06/10/2026");
    ordem.adicionarServico(&s);

    REQUIRE(ordem.getServicos().size() == 1);
    CHECK(ordem.getServicos()[0] == &s);
}

TEST_CASE("OrdemDeServico: Registrar diagnostico") {
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    std::string diagnostico = "Problema no sistema de freios";

    ordem.registrarDiagnostico(diagnostico);

    CHECK(ordem.getDiagnostico() == diagnostico);
}

TEST_CASE("OrdemDeServico: Alterar status") {
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    ordem.alterarStatus(StatusOrdem::EM_DIAGNOSTICO);

    CHECK(ordem.getStatus() == StatusOrdem::EM_DIAGNOSTICO);
}

TEST_CASE("OrdemDeServico: Nao permitir alterar diretamente para FINALIZADA") {
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    CHECK_THROWS_AS(
        ordem.alterarStatus(StatusOrdem::FINALIZADA),
        std::invalid_argument
    );
}

TEST_CASE("OrdemDeServico: Calcular valor total dos servicos") {
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    Servico s1("Troca de oleo", "Troca de oleo do motor", 100.00, 60);
    Servico s2("Alinhamento", "Alinhamento das rodas", 80.00, 40);

    OrdemDeServico ordem(&v, "06/10/2026");

    CHECK(ordem.calcularValorTotal() == doctest::Approx(0.0));

    ordem.adicionarServico(&s1);
    ordem.adicionarServico(&s2);

    CHECK(ordem.calcularValorTotal() == doctest::Approx(180.00));
}

TEST_CASE("OrdemDeServico: Definir data de entrada") {
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    ordem.definirDataEntrada("07/10/2026");

    CHECK(ordem.getDataEntrada() == "07/10/2026");
}

TEST_CASE("OrdemDeServico: Finalizar ordem de servico") {
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    ordem.alterarStatus(StatusOrdem::AGUARDANDO_RETIRADA);
    ordem.finalizar("08/10/2026");

    CHECK(ordem.getStatus() == StatusOrdem::FINALIZADA);
    CHECK(ordem.getDataSaida() == "08/10/2026");
}

TEST_CASE("OrdemDeServico: Nao finalizar ordem que nao aguarda retirada") {
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    CHECK_THROWS_AS(
        ordem.finalizar("08/10/2026"),
        std::logic_error
    );
}
TEST_CASE("OrdemDeServico: Adicionar peca") {
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    Peca* peca = nullptr;

    ordem.adicionarPeca(peca);

    REQUIRE(ordem.getPecas().size() == 1);
    CHECK(ordem.getPecas()[0] == nullptr);
}