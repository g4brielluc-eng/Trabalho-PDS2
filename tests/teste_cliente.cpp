#include "doctest.h"
#include "Cliente.hpp"
#include "Veiculo.hpp"
#include "OrdemDeServico.hpp"
#include "Agendamento.hpp"
#include <stdexcept>
#include <sstream>
#include <fstream>


TEST_CASE("Cliente: Verificar dados pessoais") {
    Cliente* c = new Cliente("Carlos Silva", "123.456.789-00", "11987654321", "Rua das Flores, 100");

    SUBCASE("verificar nome") { CHECK(c->getNome() == "Carlos Silva"); }
    SUBCASE("verificar CPF") { CHECK(c->getCpf() == "123.456.789-00"); }
    SUBCASE("verificar contato") { CHECK(c->getContato() == "11987654321"); }
    SUBCASE("verificar endereco") { CHECK(c->getEndereco() == "Rua das Flores, 100"); }
}

TEST_CASE("Cliente: Alterar contato") {
    Cliente* c = new Cliente("Carlos Silva", "123.456.789-00", "11987654321", "Rua das Flores, 100");
    c->setContato("11911112222");
    CHECK(c->getContato() == "11911112222");
}

TEST_CASE("Cliente: Consultar veiculos de cliente sem veiculos devolve lista vazia") {
    Cliente* c = new Cliente("Dorival Junior", "123.456.789-00", "11987654321", "Rua das Dunas, 100");
    CHECK(c->getVeiculos().empty());
}

TEST_CASE("Cliente: Associar e consultar veiculos") {
    Cliente* c = new Cliente("Kaio Jorge Silva", "123.456.789-00", "11987654321", "Rua das Flores, 100");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);

    c->setVeiculos(&v); 

    auto veiculos = c->getVeiculos();
    REQUIRE(veiculos.size() == 1);
    CHECK(veiculos[0] == &v);
}

TEST_CASE("Cliente: Associar e consultar ordens de servico") {
    Cliente* c = new Cliente("Mateus Pereira", "123.456.789-00", "1198987441", "Rua das Flores, 100");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    c->setServicos(&ordem);

    auto servicos = c->getServicos();
    REQUIRE(servicos.size() == 1);
    CHECK(servicos[0] == &ordem);
}

TEST_CASE("Cliente: Associar agendamento") {
    Cliente* c = new Cliente("Carlos Silva", "123.456.789-00", "11987654321", "Rua das Flores, 100"); 
}

TEST_CASE("Cliente: Pesquisar por CPF") {
    Cliente* c = new Cliente("Busca Teste", "000.111.222-33", "99999", "Rua X");
    
    SUBCASE("Pesquisar cliente existente") {
        Cliente* achado = Cliente::pesquisarPorCpf("000.111.222-33");
        REQUIRE(achado != nullptr);
        CHECK(achado->getNome() == "Busca Teste");
    }
    
    SUBCASE("Pesquisar cliente inexistente") {
        Cliente* achado = Cliente::pesquisarPorCpf("999.999.999-99");
        CHECK(achado == nullptr);
    }
}

TEST_CASE("Cliente: Listar Todos (Cobertura de impressão)") {
    Cliente::listarTodas(); 
}

TEST_CASE("Cliente: Persistencia em Arquivo") {
    std::string arquivoTeste = "teste_clientes_cov.txt";
    
    SUBCASE("Salvar arquivo com sucesso") {
        Cliente* c = new Cliente("Persistencia", "123.123.123-12", "111", "Rua Y");
        Cliente::salvarNoArquivo(arquivoTeste);
        
        std::ifstream file(arquivoTeste);
        CHECK(file.is_open());
    }
    
    SUBCASE("Carregar arquivo existente (Evita duplicatas)") {
        Cliente::carregarDoArquivo(arquivoTeste);
    }

    SUBCASE("Carregar arquivo inexistente") {
        Cliente::carregarDoArquivo("arquivo_q_nao_existe.txt");
    }
    
    SUBCASE("Falha ao salvar arquivo (Caminho invalido)") {
        #ifdef _WIN32
            Cliente::salvarNoArquivo("Z:\\caminho_impossivel\\test.txt");
        #else
            Cliente::salvarNoArquivo("/caminho_impossivel_linux/test.txt");
        #endif
    }
}

TEST_CASE("Cliente: Adicionar Agendamento") {
    Cliente* c = new Cliente("Agendamento Teste", "444", "444", "Rua Z");
    Agendamento a("10/10/2026", "10:00");
    c->setAgendamento(&a); 
}

TEST_CASE("Cliente: Construtor Vazio (Simulacao de Teclado com std::cin)") {
  
    std::istringstream inputSimulado("João Sem Fim\n999.888.777-66\nRua Mockada, 10\n1199999999\n");
    
    std::streambuf* cinOriginal = std::cin.rdbuf(); 
    
    std::cin.rdbuf(inputSimulado.rdbuf()); 

    Cliente* cVazio = new Cliente(); 

    std::cin.clear();
    std::cin.rdbuf(cinOriginal);

    CHECK(cVazio->getNome() == "João Sem Fim");
    CHECK(cVazio->getCpf() == "999.888.777-66");
}