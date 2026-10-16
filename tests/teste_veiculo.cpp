#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Veiculo.hpp"
#include "Cliente.hpp"

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

TEST_CASE("Atualizar placa do veiculo") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    v.atualizarInformacoes("XYZ9876");

    CHECK(v.getPlaca() == "XYZ9876");
}

TEST_CASE("Registrar nova quilometragem") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    v.registrarQuilometragem(82000);

    CHECK(v.getQuilometragem() == 82000);
}

TEST_CASE("getMarca retorna a marca") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getMarca() == "FIAT");
}

TEST_CASE("getModelo retorna o modelo") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getModelo() == "Toro");
}

TEST_CASE("getAno retorna o ano") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getAno() == 2025);
}

TEST_CASE("getPlaca retorna a placa") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getPlaca() == "ABC1234");
}

TEST_CASE("getQuilometragem retorna a quilometragem") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getQuilometragem() == 80000);
}

