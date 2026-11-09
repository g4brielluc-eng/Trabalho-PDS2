#include "doctest.h"
#include "Peca.hpp"
#include "Veiculo.hpp"
#include "OrdemDeServico.hpp"
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>

TEST_CASE("Peça: Cadastrar uma peça") {
    Peca* p = new Peca("0001", 12.50, 100);

    CHECK(p->getCodigo() == "0001");
    CHECK(p->getPreco() ==12.50);
    CHECK(p->getQuantidade() == 100);
    CHECK(p->getDescricao() == "Peça sem descrição");
}

TEST_CASE("Peça: Cadastrar com dados inválidos") {
    CHECK_THROWS_AS(Peca("0002", 0.0, 10), std::invalid_argument);
    CHECK_THROWS_AS(Peca("0002", -5.0, 10), std::invalid_argument);
    CHECK_THROWS_AS(Peca("0002", 5.0, -1), std::invalid_argument);
}

TEST_CASE("Peça: Não aceita código repetido") {
    Peca* p = new Peca("0003", 10.0, 5);

    CHECK_THROWS_AS(Peca("0003", 20.0, 1), std::invalid_argument);
    CHECK(Peca::pesquisarPorCodigo("0003") == p);
}

TEST_CASE("Peça: Alterar dados") {
    Peca* p = new Peca("0004", 12.50, 100);
    p->setCodigo("0005");
    p->setPreco(2.35);
    p->setQuantidade(40);
    p->setDescricao("coxim do amortecedor dianteiro");

    CHECK(p->getCodigo() == "0005");
    CHECK(p->getPreco() == doctest::Approx(2.35));
    CHECK(p->getQuantidade() == 40);
    CHECK(p->getDescricao() == "coxim do amortecedor dianteiro");
}

TEST_CASE("Peça: Alterar para dados inválidos") {
    Peca* p = new Peca("0006", 12.50, 100);
    new Peca("0007", 3.0, 1);

    CHECK_THROWS_AS(p->setPreco(0.0), std::invalid_argument);
    CHECK_THROWS_AS(p->setQuantidade(-1), std::invalid_argument);
    CHECK_THROWS_AS(p->setCodigo("0007"), std::invalid_argument);

    CHECK(p->getCodigo() == "0006");
    CHECK(p->getPreco() == doctest::Approx(12.50));
    CHECK(p->getQuantidade() == 100);
}

TEST_CASE("Peça: Disponibilidade") {
    Peca* p = new Peca("0008", 8.0, 0);
    CHECK_FALSE(p->disponivel());

    p->setQuantidade(3);
    CHECK(p->disponivel());
}

TEST_CASE("Peça: Pesquisar por código") {
    Peca* p = new Peca("0009", 8.0, 2);

    CHECK(Peca::pesquisarPorCodigo("0009") == p);
    CHECK(Peca::pesquisarPorCodigo("9999") == nullptr);
}

TEST_CASE("Peça: Ordens de serviço") {
    Peca* p = new Peca("0010", 8.0, 2);
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    CHECK(p->getServicos().empty());

    p->setServicos(&ordem);

    CHECK(p->getServicos().size() == 1);
    CHECK(p->getServicos()[0] == &ordem);
}

TEST_CASE("Peça: Listar peças") {
    new Peca("0011", 15.0, 4);

    CHECK_NOTHROW(Peca::listarTodas());
    CHECK_NOTHROW(Peca::listarTodas(true));
}

TEST_CASE("Peça: Salvar no arquivo") {
    Peca* p = new Peca("0013", 10.5, 4);
    p->setDescricao("filtro de oleo");

    Peca::salvarNoArquivo("teste_pecas.txt");

    std::ifstream arquivo("teste_pecas.txt");
    std::stringstream conteudo;
    conteudo << arquivo.rdbuf();
    arquivo.close();
    std::remove("teste_pecas.txt");

    CHECK(conteudo.str().find("0013,filtro de oleo,10.5,4") != std::string::npos);
}

TEST_CASE("Peça: Carregar do arquivo") {
    std::ofstream saida("teste_pecas.txt");
    saida << "0014,pastilha de freio,89.9,4\n";
    saida << "0015,vela de ignicao,25,10\n";
    saida.close();

    Peca::carregarDoArquivo("teste_pecas.txt");

    Peca* p = Peca::pesquisarPorCodigo("0014");
    REQUIRE(p != nullptr);
    CHECK(p->getDescricao() == "pastilha de freio");
    CHECK(p->getPreco() == doctest::Approx(89.9));
    CHECK(p->getQuantidade() == 4);
    CHECK(Peca::pesquisarPorCodigo("0015") != nullptr);

    Peca::carregarDoArquivo("teste_pecas.txt");
    CHECK(Peca::pesquisarPorCodigo("0014") == p);

    std::remove("teste_pecas.txt");
}
