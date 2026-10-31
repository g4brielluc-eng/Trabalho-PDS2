
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Veiculo.hpp"
#include "Cliente.hpp"
#include "OrdemDeServico.hpp"

TEST_CASE("Verificar dados iniciais do veiculo") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getMarca() == "FIAT");
    CHECK(v.getModelo() == "Toro");
    CHECK(v.getAno() == 2025);
    CHECK(v.getPlaca() == "ABC1234");
    CHECK(v.getQuilometragem() == 80000);
    CHECK(v.getCliente() == nullptr);
}

TEST_CASE("Associar cliente") {
    Cliente c("Arthur", "12345678900", "31999999999", "Rua Argueles");
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    v.associarCliente(&c);

    CHECK(v.getCliente() == &c);
}

TEST_CASE("Impedir associacao de cliente nulo") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK_THROWS_AS(v.associarCliente(nullptr), std::invalid_argument);
    CHECK(v.getCliente() == nullptr);
}

TEST_CASE("Atualizar placa do veiculo") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    v.atualizarInformacoes("XYZ9876");

    CHECK(v.getPlaca() == "XYZ9876");
}

TEST_CASE("Impedir placa vazia") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK_THROWS_AS(v.atualizarInformacoes(""), std::invalid_argument);
    CHECK(v.getPlaca() == "ABC1234");
}

TEST_CASE("Registrar nova quilometragem") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    v.registrarQuilometragem(82000);

    CHECK(v.getQuilometragem() == 82000);
}

TEST_CASE("Impedir quilometragem menor que a atual") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK_THROWS_AS(v.registrarQuilometragem(70000), std::invalid_argument);
    CHECK(v.getQuilometragem() == 80000);
}

TEST_CASE("Permitir registrar a mesma quilometragem") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    v.registrarQuilometragem(80000);

    CHECK(v.getQuilometragem() == 80000);
}

TEST_CASE("Impedir dados invalidos no construtor") {
    CHECK_THROWS_AS(
        Veiculo("", "Toro", 2025, "ABC1234", 80000),
        std::invalid_argument
    );

    CHECK_THROWS_AS(
        Veiculo("FIAT", "", 2025, "ABC1234", 80000),
        std::invalid_argument
    );

    CHECK_THROWS_AS(
        Veiculo("FIAT", "Toro", 0, "ABC1234", 80000),
        std::invalid_argument
    );

    CHECK_THROWS_AS(
        Veiculo("FIAT", "Toro", 2025, "", 80000),
        std::invalid_argument
    );

    CHECK_THROWS_AS(
        Veiculo("FIAT", "Toro", 2025, "ABC1234", -500),
        std::invalid_argument
    );
}

TEST_CASE("Adicionar ordem de servico ao veiculo") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);
    OrdemDeServico ordem(&v, "20/10/2026");

    v.adicionarOrdemServico(&ordem);

    const auto& ordens = v.consultarOrdensServico();

    REQUIRE(ordens.size() == 1);
    CHECK(ordens[0] == &ordem);
}

TEST_CASE("Impedir ordem de servico nula") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK_THROWS_AS(
        v.adicionarOrdemServico(nullptr),
        std::invalid_argument
    );

    CHECK(v.consultarOrdensServico().empty());
}

TEST_CASE("Impedir ordem de servico duplicada") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);
    OrdemDeServico ordem(&v, "20/10/2026");

    v.adicionarOrdemServico(&ordem);

    CHECK_THROWS_AS(
        v.adicionarOrdemServico(&ordem),
        std::logic_error
    );

    CHECK(v.consultarOrdensServico().size() == 1);
}

TEST_CASE("Consultar ordens de servico") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);
    OrdemDeServico ordem1(&v, "20/10/2026");
    OrdemDeServico ordem2(&v, "21/10/2026");

    v.adicionarOrdemServico(&ordem1);
    v.adicionarOrdemServico(&ordem2);

    const auto& ordens = v.consultarOrdensServico();

    REQUIRE(ordens.size() == 2);
    CHECK(ordens[0] == &ordem1);
    CHECK(ordens[1] == &ordem2);
}

TEST_CASE("Verificar getter da marca") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getMarca() == "FIAT");
}

TEST_CASE("Verificar getter do modelo") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getModelo() == "Toro");
}

TEST_CASE("Verificar getter do ano") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getAno() == 2025);
}

TEST_CASE("Verificar getter da placa") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getPlaca() == "ABC1234");
}

TEST_CASE("Verificar getter da quilometragem") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getQuilometragem() == 80000);
}
