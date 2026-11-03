#include "doctest.h"
#include "Cliente.hpp"
#include "Veiculo.hpp"
#include "OrdemDeServico.hpp"
#include "Agendamento.hpp"
#include <stdexcept>


TEST_CASE("Cliente: Verificar dados pessoais") {
    Cliente c("Carlos Silva", "123.456.789-00", "11987654321", "Rua das Flores, 100");

    SUBCASE("verificar nome") {
        CHECK(c.getNome() == "Carlos Silva");
    }
    SUBCASE("verificar CPF") {
        CHECK(c.getCpf() == "123.456.789-00");
    }
    SUBCASE("verificar contato") {
        CHECK(c.getContato() == "11987654321");
    }
    SUBCASE("verificar endereco") {
        CHECK(c.getEndereco() == "Rua das Flores, 100");
    }
}

TEST_CASE("Cliente: Alterar contato") {
    Cliente c("Carlos Silva", "123.456.789-00", "11987654321", "Rua das Flores, 100");
    
    c.setContato("11911112222");
    CHECK(c.getContato() == "11911112222");
}

TEST_CASE("Cliente: Consultar veiculos de cliente sem veiculos devolve lista vazia") {
    Cliente c("Dorival Junior", "123.456.789-00", "11987654321", "Rua das Dunas, 100");
    CHECK(c.getVeiculos().empty());
}

TEST_CASE("Cliente: Associar e consultar veiculos") {
    Cliente c("Kaio Jorge Silva", "123.456.789-00", "11987654321", "Rua das Flores, 100");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);

    c.setVeiculos(&v); 

    auto veiculos = c.getVeiculos();
    REQUIRE(veiculos.size() == 1);
    CHECK(veiculos[0] == &v);
}

TEST_CASE("Cliente: Associar e consultar ordens de servico") {
    Cliente c("Mateus Pereira", "123.456.789-00", "1198987441", "Rua das Flores, 100");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    c.setServicos(&ordem);

    auto servicos = c.getServicos();
    REQUIRE(servicos.size() == 1);
    CHECK(servicos[0] == &ordem);
}

TEST_CASE("Cliente: Associar agendamento") {
    Cliente c("Carlos Silva", "123.456.789-00", "11987654321", "Rua das Flores, 100"); 
}