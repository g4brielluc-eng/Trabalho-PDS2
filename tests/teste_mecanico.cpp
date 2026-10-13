#include "doctest.h" 
#include "Mecanico.hpp"
#include "Veiculo.hpp"
#include "OrdemDeServico.hpp"
#include "Servico.hpp"
#include <stdexcept>

TEST_CASE("Mecanico: Verificar dados do mecanico") {
    Mecanico m(123456, "Rickelve", "Suspensao");
    SUBCASE("verificar ID") {
        CHECK(m.getId() == 123456);
    }
    SUBCASE("verificar nome") {
        CHECK(m.getNome() == "Rickelve");
    }
    SUBCASE("verificar especialidade") {
        CHECK(m.getEspecialidade() == "Suspensao");
    }
}

TEST_CASE("Mecanico: Alterar especialidade") {
    Mecanico m(123456, "Rickelve", "Suspensao");
    m.alterarEspecialidade("Freios");
    CHECK(m.getEspecialidade() == "Freios");
}

TEST_CASE("Mecanico: Consultar ordens de mecanico sem ordens devolve lista vazia") {
    Mecanico m(123456, "Rickelve", "Freios");
    CHECK(m.consultarOrdensAtribuidas().empty());
}

TEST_CASE("Mecanico: Consultar ordens atribuidas apos assumir ordens") {
    Mecanico m(123456, "Rickelve", "Freios");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem1(&v, "06/10/2026");
    OrdemDeServico ordem2(&v, "07/10/2026");
    m.assumirOrdem(&ordem1);
    m.assumirOrdem(&ordem2);

    auto ordens = m.consultarOrdensAtribuidas();
    REQUIRE(ordens.size() == 2);
    CHECK(ordens[0] == &ordem1);
    CHECK(ordens[1] == &ordem2);
}

TEST_CASE("Mecanico: Assumir ordem de servico valida nao lanca") {
   Mecanico m(123456, "Rickelve", "Suspensao");
   Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
   OrdemDeServico ordem(&v, "06/10/2026");
   CHECK_NOTHROW(m.assumirOrdem(&ordem));
   CHECK(m.consultarOrdensAtribuidas().size() == 1);
}

TEST_CASE("Mecanico: Assumir ordem de servico nula lanca") {
  Mecanico m(123456, "Rickelve", "Suspensao");
  CHECK_THROWS_AS(m.assumirOrdem(nullptr), std::invalid_argument);  
}

TEST_CASE("Mecanico: Registrar diagnostico em uma ordem atribuida ao mecanico nao lanca") {
   Mecanico m(123456, "Rickelve", "Freios");
   Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
   OrdemDeServico ordem(&v, "06/10/2026");
   m.assumirOrdem(&ordem);

   std::string diagnostico = "Pastilha de freio desgastada";
   CHECK_NOTHROW(m.registrarDiagnostico(&ordem, diagnostico));
}

TEST_CASE("Mecanico: Registrar diagnostico em uma ordem nao atribuida ao mecanico lanca") {
    Mecanico m(123456, "Rickelve", "Freios");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    std::string diagnostico = "Pastilha de freio desgastada";
    CHECK_THROWS_AS(m.registrarDiagnostico(&ordem, diagnostico), std::logic_error);
}

TEST_CASE("Mecanico: Registrar servico realizado em uma ordem atribuida ao mecanico soma o preco no total da ordem") {
    Mecanico m(123456, "Rickelve", "Freios");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");
    m.assumirOrdem(&ordem);
    Servico s("Troca pastilha de freio", "Trocar pastilha de freio desgastada", 100.00, 120);

    CHECK(ordem.calcularValorTotal() == doctest::Approx(0.0));

    m.registrarServicoRealizado(&ordem, &s);
    CHECK(ordem.calcularValorTotal() == doctest::Approx(100.00));
}

TEST_CASE("Mecanico: Registrar servico realizado em uma ordem nao atribuida lanca") {
    Mecanico m(123456, "Rickelve", "Freios");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");
    Servico s("Troca pastilha de freio", "Trocar pastilha de freio desgastada", 100.00, 120);

    CHECK_THROWS_AS(m.registrarServicoRealizado(&ordem, &s), std::logic_error);

    CHECK(ordem.calcularValorTotal() == doctest::Approx(0.0));
}

TEST_CASE("Mecanico: Atualizar o andamento da ordem atribuida ao mecanico nao lanca") {
    Mecanico m(123456, "Rickelve", "Freios");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");
    m.assumirOrdem(&ordem);
    StatusOrdem novoStatus = StatusOrdem::EM_DIAGNOSTICO;
 
    CHECK_NOTHROW(m.atualizarAndamento(&ordem, novoStatus));
}

TEST_CASE("Mecanico: Atualizar o andamento da ordem atribuida ao mecanico para FINALIZADA lanca") {
    Mecanico m(123456, "Rickelve", "Freios");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");
    m.assumirOrdem(&ordem);
    StatusOrdem novoStatus = StatusOrdem::FINALIZADA;
 
    CHECK_THROWS_AS(m.atualizarAndamento(&ordem, novoStatus), std::invalid_argument);
}

TEST_CASE("Mecanico: Atualizar o andamento da ordem nao atribuida ao mecanico lanca") {
    Mecanico m(123456, "Rickelve", "Freios");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");
    StatusOrdem novoStatus = StatusOrdem::EM_DIAGNOSTICO;
 
    CHECK_THROWS_AS(m.atualizarAndamento(&ordem, novoStatus), std::logic_error);
}