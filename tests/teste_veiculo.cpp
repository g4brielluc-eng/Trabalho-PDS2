
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Veiculo.hpp"
#include "Cliente.hpp"
#include "OrdemDeServico.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

TEST_CASE("Verificar dados iniciais do veiculo") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK(v.getMarca() == "FIAT");
    CHECK(v.getModelo() == "Toro");
    CHECK(v.getAno() == 2025);
    CHECK(v.getPlaca() == "ABC1234");
    CHECK(v.getQuilometragem() == 80000);
    CHECK(v.getCliente() == nullptr);
}

TEST_CASE("Impedir dados invalidos no construtor") {
    CHECK_THROWS_AS(Veiculo("", "Toro", 2025, "ABC1234", 80000), std::invalid_argument);
    CHECK_THROWS_AS(Veiculo("FIAT", "", 2025, "ABC1234", 80000), std::invalid_argument);
    CHECK_THROWS_AS(Veiculo("FIAT", "Toro", 0, "ABC1234", 80000), std::invalid_argument);
    CHECK_THROWS_AS(Veiculo("FIAT", "Toro", 2025, "", 80000), std::invalid_argument);
    CHECK_THROWS_AS(Veiculo("FIAT", "Toro", 2025, "ABC1234", -500), std::invalid_argument);
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

TEST_CASE("Associar outro cliente ao veiculo") {
    Cliente c1("Arthur", "12345678900", "31999999999", "Rua A");
    Cliente c2("Carlos", "98765432100", "31988888888", "Rua B");

    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    v.associarCliente(&c1);
    CHECK(v.getCliente() == &c1);

    v.associarCliente(&c2);
    CHECK(v.getCliente() == &c2);
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

TEST_CASE("Atualizar placa mais de uma vez") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    v.atualizarInformacoes("XYZ9876");
    v.atualizarInformacoes("DEF4567");

    CHECK(v.getPlaca() == "DEF4567");
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

TEST_CASE("Adicionar ordem de servico ao veiculo") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);
    OrdemDeServico ordem(&v, "20/10/2026");

    const auto& ordens = v.consultarOrdensServico();

    REQUIRE(ordens.size() == 1);
    CHECK(ordens[0] == &ordem);
}

TEST_CASE("Impedir ordem de servico nula") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    CHECK_THROWS_AS(v.adicionarOrdemServico(nullptr), std::invalid_argument);
}

TEST_CASE("Impedir ordem de servico duplicada") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);
    OrdemDeServico ordem(&v, "20/10/2026");

    CHECK_THROWS_AS(v.adicionarOrdemServico(&ordem), std::logic_error);
    CHECK(v.consultarOrdensServico().size() == 1);
}

TEST_CASE("Consultar ordens de servico") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);
    OrdemDeServico ordem1(&v, "20/10/2026");
    OrdemDeServico ordem2(&v, "21/10/2026");

    const auto& ordens = v.consultarOrdensServico();

    REQUIRE(ordens.size() == 2);
    CHECK(ordens[0] == &ordem1);
    CHECK(ordens[1] == &ordem2);
}

TEST_CASE("Consultar historico sem ordens de servico") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    std::ostringstream saida;
    std::streambuf* saidaOriginal = std::cout.rdbuf(saida.rdbuf());

    v.consultarHistoricoManutencoes();

    std::cout.rdbuf(saidaOriginal);

    CHECK(saida.str().find("Nenhuma ordem de servico registrada para este veiculo.") != std::string::npos);
}

TEST_CASE("Consultar historico com ordem ainda nao finalizada") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);
    OrdemDeServico ordem(&v, "20/10/2026");

    ordem.registrarDiagnostico("Problema no motor");

    std::ostringstream saida;
    std::streambuf* saidaOriginal = std::cout.rdbuf(saida.rdbuf());

    v.consultarHistoricoManutencoes();

    std::cout.rdbuf(saidaOriginal);

    CHECK(saida.str().find("20/10/2026") != std::string::npos);
    CHECK(saida.str().find("Ainda nao finalizada") != std::string::npos);
    CHECK(saida.str().find("Problema no motor") != std::string::npos);
    CHECK(saida.str().find("Aberta") != std::string::npos);
    CHECK(saida.str().find("Valor total: R$") != std::string::npos);
}

TEST_CASE("Consultar historico com ordem finalizada") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);
    OrdemDeServico ordem(&v, "20/10/2026");

    ordem.registrarDiagnostico("Troca de oleo");
    ordem.alterarStatus(StatusOrdem::AGUARDANDO_RETIRADA);
    ordem.finalizar("22/10/2026");

    std::ostringstream saida;
    std::streambuf* saidaOriginal = std::cout.rdbuf(saida.rdbuf());

    v.consultarHistoricoManutencoes();

    std::cout.rdbuf(saidaOriginal);

    CHECK(saida.str().find("22/10/2026") != std::string::npos);
    CHECK(saida.str().find("Finalizada") != std::string::npos);
}

TEST_CASE("Consultar historico com todos os status") {
    Veiculo v("FIAT", "Toro", 2025, "ABC1234", 80000);

    OrdemDeServico aberta(&v, "01/10/2026");
    OrdemDeServico diagnostico(&v, "02/10/2026");
    OrdemDeServico execucao(&v, "03/10/2026");
    OrdemDeServico aguardandoPeca(&v, "04/10/2026");
    OrdemDeServico aguardandoRetirada(&v, "05/10/2026");
    OrdemDeServico finalizada(&v, "06/10/2026");

    diagnostico.alterarStatus(StatusOrdem::EM_DIAGNOSTICO);
    execucao.alterarStatus(StatusOrdem::EM_EXECUCAO);
    aguardandoPeca.alterarStatus(StatusOrdem::AGUARDANDO_PECA);
    aguardandoRetirada.alterarStatus(StatusOrdem::AGUARDANDO_RETIRADA);

    finalizada.alterarStatus(StatusOrdem::AGUARDANDO_RETIRADA);
    finalizada.finalizar("07/10/2026");

    std::ostringstream saida;
    std::streambuf* saidaOriginal = std::cout.rdbuf(saida.rdbuf());

    v.consultarHistoricoManutencoes();

    std::cout.rdbuf(saidaOriginal);

    CHECK(saida.str().find("Aberta") != std::string::npos);
    CHECK(saida.str().find("Em diagnostico") != std::string::npos);
    CHECK(saida.str().find("Em execucao") != std::string::npos);
    CHECK(saida.str().find("Aguardando peca") != std::string::npos);
    CHECK(saida.str().find("Aguardando retirada") != std::string::npos);
    CHECK(saida.str().find("Finalizada") != std::string::npos);
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
