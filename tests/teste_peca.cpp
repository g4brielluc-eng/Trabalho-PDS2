#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
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
        CHECK(p.getPreco() == 2.35);
    }

    SUBCASE("verificar quantidade") {
        CHECK(p.getQuantidade() == 40);
    }

    SUBCASE("verificar descrição") {
        CHECK(p.getDescricao() == "coxim do amortecedor dianteiro");
    }

}

