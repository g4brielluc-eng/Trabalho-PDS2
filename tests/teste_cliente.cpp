#include "doctest.h"
#include "Cliente.hpp"
#include "Veiculo.hpp"
#include "OrdemDeServico.hpp"
#include "Agendamento.hpp"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class EntradaFalsa {
public:
    EntradaFalsa(const std::string& texto)
        : entrada(texto), antigo(std::cin.rdbuf(entrada.rdbuf())) {}

    ~EntradaFalsa() {
        std::cin.rdbuf(antigo);
        std::cin.clear();
    }

private:
    std::istringstream entrada;
    std::streambuf* antigo;
};

std::string lerArquivo(const std::string& nome) {
    std::ifstream arquivo(nome);
    std::stringstream ss;
    ss << arquivo.rdbuf();
    return ss.str();
}

void apagarCliente(const std::string& cpf) {
    Cliente* c = Cliente::pesquisarPorCpf(cpf);
    if (c == nullptr) return;

    for (Veiculo* v : c->getVeiculos()) {
        delete v;
    }
    c->getVeiculos().clear();
    delete c;
}

TEST_CASE("Cliente: dados do construtor") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua das Flores, 100");

    CHECK(c.getNome() == "Carlos Silva");
    CHECK(c.getCpf() == "12345678900");
    CHECK(c.getContato() == "11987654321");
    CHECK(c.getEndereco() == "Rua das Flores, 100");
}

TEST_CASE("Cliente: construtor com dados invalidos") {
    SUBCASE("nome com numeros") {
        CHECK_THROWS_AS(Cliente("Joao123", "12345678900", "11987654321", "Rua A"), std::invalid_argument);
    }
    SUBCASE("cpf com letras") {
        CHECK_THROWS_AS(Cliente("Joao Silva", "1234567890a", "11987654321", "Rua A"), std::invalid_argument);
    }
    SUBCASE("contato vazio") {
        CHECK_THROWS_AS(Cliente("Joao Silva", "12345678900", "", "Rua A"), std::invalid_argument);
    }
    SUBCASE("cpf repetido") {
        Cliente a("Ana Souza", "11122233344", "31988887777", "Rua A");
        CHECK_THROWS_AS(Cliente("Bia Costa", "11122233344", "31977776666", "Rua B"), std::invalid_argument);
        CHECK(Cliente::pesquisarPorCpf("11122233344") == &a);
    }
}

TEST_CASE("Cliente: cpf fica livre depois que o cliente e destruido") {
    {
        Cliente a("Ana Souza", "11122233344", "31988887777", "Rua A");
    }
    CHECK(Cliente::pesquisarPorCpf("11122233344") == nullptr);
    CHECK_NOTHROW(Cliente("Bia Costa", "11122233344", "31977776666", "Rua B"));
}

TEST_CASE("Cliente: construtor padrao") {
    EntradaFalsa entrada("Joao Silva\n12345678900\nRua A, 10\n31999998888\n");

    Cliente c;

    CHECK(c.getNome() == "Joao Silva");
    CHECK(c.getCpf() == "12345678900");
    CHECK(c.getEndereco() == "Rua A, 10");
    CHECK(c.getContato() == "31999998888");
}

TEST_CASE("Cliente: construtor padrao repete a pergunta quando o dado e invalido") {
    Cliente existente("Maria Souza", "11122233344", "31988887777", "Rua B");
    EntradaFalsa entrada(
        "Joao123\nJoao Silva\n"
        "123abc\n11122233344\n55566677788\n"
        "Rua A\n"
        "abc\n31999998888\n");

    Cliente c;

    CHECK(c.getNome() == "Joao Silva");
    CHECK(c.getCpf() == "55566677788");
    CHECK(c.getContato() == "31999998888");
}

TEST_CASE("Cliente: construtor padrao com entrada vazia") {
    EntradaFalsa entrada("");

    CHECK_THROWS_AS(Cliente(), std::runtime_error);
}

TEST_CASE("Cliente: alterar contato") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua A");

    c.setContato("11911112222");
    CHECK(c.getContato() == "11911112222");

    CHECK_THROWS_AS(c.setContato("abc"), std::invalid_argument);
    CHECK(c.getContato() == "11911112222");
}

TEST_CASE("Cliente: veiculos") {
    Cliente c("Kaio Silva", "12345678900", "11987654321", "Rua A");
    Veiculo v1("FIAT", "Palio", 2010, "ABC1234", 150000);
    Veiculo v2("FORD", "Ka", 2015, "XYZ9876", 50000);

    CHECK(c.getVeiculos().empty());

    c.setVeiculos(&v1);
    c.setVeiculos(&v2);
    c.setVeiculos(&v1);

    CHECK(c.getVeiculos().size() == 2);
    CHECK_THROWS_AS(c.setVeiculos(nullptr), std::invalid_argument);
}

TEST_CASE("Cliente: ordens de servico") {
    Cliente c("Mateus Pereira", "12345678900", "11987654321", "Rua A");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem1(&v, "06/10/2026");
    OrdemDeServico ordem2(&v, "07/10/2026");

    CHECK_NOTHROW(c.listarServicos());

    c.setServicos(&ordem1);
    c.setServicos(&ordem2);
    c.setServicos(&ordem1);

    CHECK(c.getServicos().size() == 2);
    CHECK_THROWS_AS(c.setServicos(nullptr), std::invalid_argument);
    CHECK_NOTHROW(c.listarServicos());
}

TEST_CASE("Cliente: agendamentos") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua A");
    Agendamento pendente("10/12/2026", "14:30");
    Agendamento confirmado("11/12/2026", "09:00");
    Agendamento cancelado("12/12/2026", "16:00");
    confirmado.confirmar();
    cancelado.cancelar();

    CHECK_NOTHROW(c.listarAgendamentos());

    c.setAgendamento(&pendente);
    c.setAgendamento(&confirmado);
    c.setAgendamento(&cancelado);
    c.setAgendamento(&pendente);

    CHECK_THROWS_AS(c.setAgendamento(nullptr), std::invalid_argument);
    CHECK_NOTHROW(c.listarAgendamentos());
}

TEST_CASE("Cliente: pesquisarPorCpf") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua A");

    CHECK(Cliente::pesquisarPorCpf("12345678900") == &c);
    CHECK(Cliente::pesquisarPorCpf("99999999999") == nullptr);
    CHECK_THROWS_AS(Cliente::pesquisarPorCpf("abc"), std::invalid_argument);
}

TEST_CASE("Cliente: listarTodas e excluirCliente") {
    CHECK_NOTHROW(Cliente::listarTodas());

    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua A");
    CHECK_NOTHROW(Cliente::listarTodas());

    c.excluirCliente();
    CHECK(Cliente::pesquisarPorCpf("12345678900") == nullptr);
}

TEST_CASE("Cliente: salvarNoArquivo") {
    Cliente c("Ana Souza", "11122233344", "31988887777", "Rua A");
    Veiculo v("FORD", "Ka", 2015, "XYZ9876", 50000);
    Agendamento confirmado("11/12/2026", "09:00");
    Agendamento cancelado("12/12/2026", "16:00");
    confirmado.confirmar();
    cancelado.cancelar();
    c.setVeiculos(&v);
    c.setAgendamento(&confirmado);
    c.setAgendamento(&cancelado);

    Cliente::salvarNoArquivo("teste_clientes.txt");
    std::string texto = lerArquivo("teste_clientes.txt");
    std::remove("teste_clientes.txt");

    CHECK(texto.find("C;Ana Souza;11122233344;31988887777;Rua A") != std::string::npos);
    CHECK(texto.find("V;") == std::string::npos);
    CHECK(texto.find("A;11122233344;11/12/2026;09:00;CONFIRMADO") != std::string::npos);
    CHECK(texto.find("A;11122233344;12/12/2026;16:00;CANCELADO") != std::string::npos);

    CHECK_NOTHROW(Cliente::salvarNoArquivo("pasta_que_nao_existe/clientes.txt"));
}

TEST_CASE("Cliente: carregarDoArquivo") {
    CHECK_NOTHROW(Cliente::carregarDoArquivo("arquivo_que_nao_existe.txt"));

    std::ofstream arquivo("teste_carregar.txt");
    arquivo << "C;Ana Souza;22233344455;31988887777;Rua A\n"
            << "V;22233344455;FORD;Ka;2015;XYZ9876;50000\n"
            << "V;22233344455;FORD;Ka;2015;XYZ9876;50000\n"
            << "A;22233344455;15/12/2026;10:00;CONFIRMADO\n"
            << "A;22233344455;16/12/2026;11:00;CANCELADO\n"
            << "\n"
            << "C;Joao123;33344455566;31999998888;Rua B\n";
    arquivo.close();

    Cliente::carregarDoArquivo("teste_carregar.txt");

    Cliente* ana = Cliente::pesquisarPorCpf("22233344455");
    REQUIRE(ana != nullptr);
    CHECK(ana->getNome() == "Ana Souza");
    CHECK(ana->getVeiculos().size() == 0);
    CHECK(Cliente::pesquisarPorCpf("33344455566") == nullptr);

    Cliente::carregarDoArquivo("teste_carregar.txt");
    CHECK(Cliente::pesquisarPorCpf("22233344455") == ana);
    CHECK(ana->getVeiculos().size() == 0);

    apagarCliente("22233344455");
    std::remove("teste_carregar.txt");
}
