#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Servico.hpp"
#include "OrdemDeServico.hpp"
#include "Veiculo.hpp"
#include <stdexcept>

TEST_CASE("Servico: Verificar dados iniciais do servico") {
    Servico s("Troca de oleo", "Troca de oleo e filtro", 120.0f, 40);

    CHECK(s.getNome() == "Troca de oleo");
    CHECK(s.getDescricao() == "Troca de oleo e filtro");
    CHECK(s.getPreco() == doctest::Approx(120.0f));
    CHECK(s.getDuracaoEstimada() == 40);
    CHECK(s.getOrdens().empty());
}

TEST_CASE("Servico: Cadastrar com nome vazio lanca") {
    CHECK_THROWS_AS(Servico("", "Descricao qualquer", 100.0f, 30), std::invalid_argument);
}

TEST_CASE("Servico: Cadastrar com preco negativo lanca") {
    CHECK_THROWS_AS(Servico("Alinhamento", "Alinhamento e balanceamento", -10.0f, 30), std::invalid_argument);
}

TEST_CASE("Servico: Cadastrar com duracao nao positiva lanca") {
    CHECK_THROWS_AS(Servico("Revisao", "Revisao geral", 200.0f, 0), std::invalid_argument);
}

TEST_CASE("Servico: Atualizar descricao") {
    Servico s("Balanceamento", "Balanceamento de rodas", 60.0f, 20);
    s.atualizarDescricao("Balanceamento das 4 rodas");
    CHECK(s.getDescricao() == "Balanceamento das 4 rodas");
}

TEST_CASE("Servico: Atualizar preco valido nao lanca") {
    Servico s("Revisao geral", "Revisao completa", 200.0f, 120);
    CHECK_NOTHROW(s.atualizarPreco(250.0f));
    CHECK(s.getPreco() == doctest::Approx(250.0f));
}

TEST_CASE("Servico: Atualizar preco negativo lanca") {
    Servico s("Revisao geral", "Revisao completa", 200.0f, 120);
    CHECK_THROWS_AS(s.atualizarPreco(-50.0f), std::invalid_argument);
    CHECK(s.getPreco() == doctest::Approx(200.0f));
}

TEST_CASE("Servico: Registrar duracao estimada valida") {
    Servico s("Diagnostico eletronico", "Diagnostico via scanner", 100.0f, 30);
    s.registrarDuracaoEstimada(45);
    CHECK(s.getDuracaoEstimada() == 45);
}

TEST_CASE("Servico: Registrar duracao nao positiva lanca") {
    Servico s("Diagnostico eletronico", "Diagnostico via scanner", 100.0f, 30);
    CHECK_THROWS_AS(s.registrarDuracaoEstimada(0), std::invalid_argument);
    CHECK(s.getDuracaoEstimada() == 30);
}

TEST_CASE("Servico: Consultar informacoes retorna dados do servico") {
    Servico s("Troca de pastilhas", "Troca de pastilhas de freio", 150.0f, 60);
    std::string info = s.consultarInformacoes();

    CHECK(info.find("Troca de pastilhas") != std::string::npos);
    CHECK(info.find("Troca de pastilhas de freio") != std::string::npos);
}

TEST_CASE("Servico: Associar ordem de servico valida nao lanca") {
    Servico s("Troca de correia", "Troca de correia dentada", 300.0f, 90);
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    CHECK_NOTHROW(s.associarOrdem(&ordem));
    CHECK(s.getOrdens().size() == 1);
    CHECK(s.getOrdens()[0] == &ordem);
}

TEST_CASE("Servico: Associar a mesma ordem duas vezes nao duplica") {
    Servico s("Troca de correia", "Troca de correia dentada", 300.0f, 90);
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    s.associarOrdem(&ordem);
    s.associarOrdem(&ordem);
    CHECK(s.getOrdens().size() == 1);
}

TEST_CASE("Servico: Associar ordem nula lanca") {
    Servico s("Troca de correia", "Troca de correia dentada", 300.0f, 90);
    CHECK_THROWS_AS(s.associarOrdem(nullptr), std::invalid_argument);
}

TEST_CASE("Servico: Calcular custo com quantidade padrao") {
    Servico s("Alinhamento", "Alinhamento 3D", 80.0f, 30);
    CHECK(s.calcularCusto() == doctest::Approx(80.0f));
}

TEST_CASE("Servico: Calcular custo com quantidade maior que um") {
    Servico s("Troca de vela", "Troca do jogo de velas", 40.0f, 20);
    CHECK(s.calcularCusto(4) == doctest::Approx(160.0f));
}

TEST_CASE("Servico: Calcular custo com quantidade invalida lanca") {
    Servico s("Troca de vela", "Troca do jogo de velas", 40.0f, 20);
    CHECK_THROWS_AS(s.calcularCusto(0), std::invalid_argument);
}
