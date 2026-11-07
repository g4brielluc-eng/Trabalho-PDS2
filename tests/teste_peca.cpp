#include "doctest.h"
#include "Peca.hpp"
#include "Veiculo.hpp"
#include "OrdemDeServico.hpp"
#include <stdexcept>

TEST_CASE("Peça: Cadastrar uma peça") {
    
    Peca p("0001",12.50 , 100);
    
    SUBCASE("verificar codigo") {
        CHECK(p.getCodigo() == "0001");
    }

    SUBCASE("verificar preço") {    
        CHECK(p.getPreco() == 12.50);
    }

    SUBCASE("verificar quantidade") {
        CHECK(p.getQuantidade() == 100);
    }

     SUBCASE("verificar descrição") {
        CHECK(p.getDescricao() == "Peça sem descrição");
    }


}

TEST_CASE("Peça: Alterar dados"){

    Peca p("0001",12.50,100);
    p.setCodigo("0002");
    p.setPreco(2.35);
    p.setQuantidade(40);
    p.setDescricao("coxim do amortecedor dianteiro");

    SUBCASE("verificar codigo") {
        CHECK(p.getCodigo() == "0002");
    }

    SUBCASE("verificar preço") {    
        CHECK(p.getPreco() ==  doctest::Approx(2.35));
    }

    SUBCASE("verificar quantidade") {
        CHECK(p.getQuantidade() == 40);
    }

    SUBCASE("verificar descrição") {
        CHECK(p.getDescricao() == "coxim do amortecedor dianteiro");
    }
}

TEST_CASE("Peça: Verificar disponibilidade") {
    Peca p("0001", 12.50, 10);
    CHECK(p.disponivel() == true);

    p.setQuantidade(0);
    CHECK(p.disponivel() == false);
}

TEST_CASE("Peça: Alterar preço inválido") {
    Peca p("0001", 12.50, 10);
    CHECK_THROWS_AS(p.setPreco(-5.0), std::invalid_argument);
}

TEST_CASE("Peça: Alterar quantidade inválida") {
    Peca p("0001", 12.50, 10);
    CHECK_THROWS_AS(p.setQuantidade(-10), std::invalid_argument);
}

TEST_CASE("Peça: Pesquisar peça existente") {
    Peca p("0001", 12.50, 10);
    Peca* achado = Peca::pesquisarPorCodigo("0001");
    CHECK(achado == &p);
}

