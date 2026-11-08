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

// ============================================================================
// Funcoes auxiliares dos testes
// ============================================================================
namespace {

// Redireciona um fluxo de saida (cout/cerr) para um buffer ate parar()
// ou ate o fim do escopo. Os CHECKs devem ficar DEPOIS de parar(), senao a
// mensagem de falha do doctest tambem seria engolida pelo buffer.
class CapturaSaida {
public:
    explicit CapturaSaida(std::ostream& fluxo)
        : fluxo_(fluxo), antigo_(fluxo.rdbuf(buffer_.rdbuf())), ativa_(true) {}

    ~CapturaSaida() { parar(); }

    void parar() {
        if (ativa_) {
            fluxo_.rdbuf(antigo_);
            ativa_ = false;
        }
    }

    std::string texto() const { return buffer_.str(); }

private:
    std::ostream& fluxo_;
    std::ostringstream buffer_;
    std::streambuf* antigo_;
    bool ativa_;
};

// Faz o std::cin ler de um texto fixo (simula o usuario digitando)
class EntradaFalsa {
public:
    explicit EntradaFalsa(const std::string& texto)
        : entrada_(texto), antigo_(std::cin.rdbuf(entrada_.rdbuf())) {}

    ~EntradaFalsa() {
        std::cin.rdbuf(antigo_);
        std::cin.clear();
    }

private:
    std::istringstream entrada_;
    std::streambuf* antigo_;
};

bool contem(const std::string& texto, const std::string& trecho) {
    return texto.find(trecho) != std::string::npos;
}

int contarOcorrencias(const std::string& texto, const std::string& trecho) {
    int total = 0;
    std::string::size_type pos = texto.find(trecho);
    while (pos != std::string::npos) {
        total++;
        pos = texto.find(trecho, pos + trecho.size());
    }
    return total;
}

bool existeCliente(const std::string& cpf) {
    return Cliente::pesquisarPorCpf(cpf) != nullptr;
}

void escreverArquivo(const std::string& nome, const std::string& conteudo) {
    std::ofstream arquivo(nome.c_str());
    arquivo << conteudo;
}

std::vector<std::string> lerLinhasDoArquivo(const std::string& nome) {
    std::vector<std::string> linhas;
    std::ifstream arquivo(nome.c_str());
    std::string linha;
    while (std::getline(arquivo, linha)) {
        linhas.push_back(linha);
    }
    return linhas;
}

// Garante que o arquivo temporario nao existe antes e some depois do teste
struct ArquivoTemporario {
    std::string nome;
    explicit ArquivoTemporario(const std::string& n) : nome(n) { std::remove(nome.c_str()); }
    ~ArquivoTemporario() { std::remove(nome.c_str()); }
};

// carregarDoArquivo cria clientes (e veiculos) com "new" e ninguem os apaga.
// Como a lista de clientes e estatica, eles contaminariam os outros testes.
// Este guard apaga os clientes indicados (e os veiculos deles) ao sair do escopo.
struct LimpezaClientes {
    std::vector<std::string> cpfs;

    explicit LimpezaClientes(const std::vector<std::string>& c) : cpfs(c) {}

    ~LimpezaClientes() {
        for (size_t i = 0; i < cpfs.size(); ++i) {
            Cliente* c = Cliente::pesquisarPorCpf(cpfs[i]);
            if (c != nullptr) {
                std::vector<Veiculo*>& veiculos = c->getVeiculos();
                for (size_t j = 0; j < veiculos.size(); ++j) {
                    delete veiculos[j];
                }
                veiculos.clear();
                delete c;   // o destrutor tira o cliente da lista
            }
        }
    }
};

}  // namespace

// ============================================================================
// Dados pessoais e construtor
// ============================================================================

TEST_CASE("Cliente: Verificar dados pessoais") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua das Flores, 100");

    SUBCASE("verificar nome") {
        CHECK(c.getNome() == "Carlos Silva");
    }
    SUBCASE("verificar CPF") {
        CHECK(c.getCpf() == "12345678900");
    }
    SUBCASE("verificar contato") {
        CHECK(c.getContato() == "11987654321");
    }
    SUBCASE("verificar endereco") {
        CHECK(c.getEndereco() == "Rua das Flores, 100");
    }
}

TEST_CASE("Cliente: Validacoes do construtor") {
    SUBCASE("nome vazio") {
        CHECK_THROWS_AS(Cliente("", "12345678900", "11987654321", "Rua A"), std::invalid_argument);
    }
    SUBCASE("nome com numeros") {
        CHECK_THROWS_AS(Cliente("Joao123", "12345678900", "11987654321", "Rua A"), std::invalid_argument);
    }
    SUBCASE("cpf vazio") {
        CHECK_THROWS_AS(Cliente("Joao Silva", "", "11987654321", "Rua A"), std::invalid_argument);
    }
    SUBCASE("cpf com letras") {
        CHECK_THROWS_AS(Cliente("Joao Silva", "1234567890a", "11987654321", "Rua A"), std::invalid_argument);
    }
    SUBCASE("cpf formatado com pontos e traco nao e aceito") {
        CHECK_THROWS_AS(Cliente("Joao Silva", "123.456.789-00", "11987654321", "Rua A"), std::invalid_argument);
    }
    SUBCASE("contato vazio") {
        CHECK_THROWS_AS(Cliente("Joao Silva", "12345678900", "", "Rua A"), std::invalid_argument);
    }
    SUBCASE("contato com letras") {
        CHECK_THROWS_AS(Cliente("Joao Silva", "12345678900", "abc", "Rua A"), std::invalid_argument);
    }
}

TEST_CASE("Cliente: Nao permite dois clientes com o mesmo CPF") {
    Cliente a("Ana Souza", "11122233344", "31988887777", "Rua A");

    CHECK_THROWS_AS(Cliente("Bia Costa", "11122233344", "31977776666", "Rua B"), std::invalid_argument);

    // o cliente original continua cadastrado
    CHECK(Cliente::pesquisarPorCpf("11122233344") == &a);
}

TEST_CASE("Cliente: CPF fica livre depois que o cliente e destruido") {
    {
        Cliente a("Ana Souza", "11122233344", "31988887777", "Rua A");
        CHECK(existeCliente("11122233344"));
    }

    CHECK_FALSE(existeCliente("11122233344"));
    CHECK_NOTHROW(Cliente("Bia Costa", "11122233344", "31977776666", "Rua B"));
}

// ============================================================================
// Construtor padrao (le os dados do teclado)
// ============================================================================

TEST_CASE("Cliente: Construtor padrao le os dados digitados") {
    EntradaFalsa entrada("Joao Silva\n12345678900\nRua A, 10\n31999998888\n");
    CapturaSaida saida(std::cout);

    Cliente c;
    saida.parar();

    CHECK(c.getNome() == "Joao Silva");
    CHECK(c.getCpf() == "12345678900");
    CHECK(c.getEndereco() == "Rua A, 10");
    CHECK(c.getContato() == "31999998888");
    CHECK(contem(saida.texto(), "Joao Silva cadastrado com sucesso"));
    CHECK(Cliente::pesquisarPorCpf("12345678900") == &c);
}

TEST_CASE("Cliente: Construtor padrao repete a pergunta quando o dado e invalido") {
    // nome invalido -> valido, cpf invalido -> valido, contato invalido -> valido
    EntradaFalsa entrada(
        "Joao123\nJoao Silva\n"
        "123abc\n12345678900\n"
        "Rua A\n"
        "abc\n31999998888\n");
    CapturaSaida saida(std::cout);

    Cliente c;
    saida.parar();

    CHECK(c.getNome() == "Joao Silva");
    CHECK(c.getCpf() == "12345678900");
    CHECK(c.getContato() == "31999998888");
    CHECK(contarOcorrencias(saida.texto(), "Erro:") == 3);
}

TEST_CASE("Cliente: Construtor padrao nao aceita CPF ja cadastrado") {
    Cliente existente("Maria Souza", "11122233344", "31988887777", "Rua B");

    EntradaFalsa entrada("Joao Silva\n11122233344\n55566677788\nRua A\n31999998888\n");
    CapturaSaida saida(std::cout);

    Cliente c;
    saida.parar();

    CHECK(c.getCpf() == "55566677788");
    CHECK(contarOcorrencias(saida.texto(), "Erro:") == 1);
    CHECK(Cliente::pesquisarPorCpf("11122233344") == &existente);
}

TEST_CASE("Cliente: Construtor padrao lanca erro quando a entrada acaba") {
    SUBCASE("entrada vazia") {
        EntradaFalsa entrada("");
        CapturaSaida saida(std::cout);

        bool lancou = false;
        try {
            Cliente c;
        } catch (const std::runtime_error&) {
            lancou = true;
        }
        saida.parar();

        CHECK(lancou);
    }
    SUBCASE("entrada interrompida no meio do cadastro") {
        EntradaFalsa entrada("Joao Silva\n");
        CapturaSaida saida(std::cout);

        bool lancou = false;
        try {
            Cliente c;
        } catch (const std::runtime_error&) {
            lancou = true;
        }
        saida.parar();

        CHECK(lancou);
        // cadastro incompleto nao deve deixar cliente na lista
        CHECK_FALSE(existeCliente("12345678900"));
    }
}

// ============================================================================
// Contato
// ============================================================================

TEST_CASE("Cliente: Alterar contato") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua das Flores, 100");

    c.setContato("11911112222");
    CHECK(c.getContato() == "11911112222");
}

TEST_CASE("Cliente: Alterar contato para valor invalido nao altera o contato") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua das Flores, 100");

    CHECK_THROWS_AS(c.setContato("abc"), std::invalid_argument);
    CHECK_THROWS_AS(c.setContato(""), std::invalid_argument);
    CHECK(c.getContato() == "11987654321");
}

// ============================================================================
// Veiculos
// ============================================================================

TEST_CASE("Cliente: Consultar veiculos de cliente sem veiculos devolve lista vazia") {
    Cliente c("Dorival Junior", "12345678900", "11987654321", "Rua das Dunas, 100");
    CHECK(c.getVeiculos().empty());
}

TEST_CASE("Cliente: Associar e consultar veiculos") {
    Cliente c("Kaio Jorge Silva", "12345678900", "11987654321", "Rua das Flores, 100");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);

    c.setVeiculos(&v);

    const std::vector<Veiculo*>& veiculos = c.getVeiculos();
    REQUIRE(veiculos.size() == 1);
    CHECK(veiculos[0] == &v);
}

TEST_CASE("Cliente: Associar veiculo nulo lanca erro") {
    Cliente c("Kaio Jorge Silva", "12345678900", "11987654321", "Rua das Flores, 100");

    CHECK_THROWS_AS(c.setVeiculos(nullptr), std::invalid_argument);
    CHECK(c.getVeiculos().empty());
}

TEST_CASE("Cliente: Nao duplica o mesmo veiculo") {
    Cliente c("Kaio Jorge Silva", "12345678900", "11987654321", "Rua das Flores, 100");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);

    c.setVeiculos(&v);
    c.setVeiculos(&v);

    CHECK(c.getVeiculos().size() == 1);
}

TEST_CASE("Cliente: Nao duplica veiculos com a mesma placa") {
    Cliente c("Kaio Jorge Silva", "12345678900", "11987654321", "Rua das Flores, 100");
    Veiculo v1("FIAT", "Palio", 2010, "ABC1234", 150000);
    Veiculo copia("FIAT", "Palio", 2010, "ABC1234", 150000);  // outro objeto, mesma placa

    c.setVeiculos(&v1);
    c.setVeiculos(&copia);

    REQUIRE(c.getVeiculos().size() == 1);
    CHECK(c.getVeiculos()[0] == &v1);
}

TEST_CASE("Cliente: Aceita varios veiculos com placas diferentes") {
    Cliente c("Kaio Jorge Silva", "12345678900", "11987654321", "Rua das Flores, 100");
    Veiculo v1("FIAT", "Palio", 2010, "ABC1234", 150000);
    Veiculo v2("FORD", "Ka", 2015, "XYZ9876", 50000);

    c.setVeiculos(&v1);
    c.setVeiculos(&v2);

    CHECK(c.getVeiculos().size() == 2);
}

TEST_CASE("Cliente: listarVeiculos sem veiculos") {
    Cliente c("Kaio Jorge Silva", "12345678900", "11987654321", "Rua das Flores, 100");

    CapturaSaida saida(std::cout);
    c.listarVeiculos();
    saida.parar();

    CHECK(contem(saida.texto(), "associado a esse cliente"));
    CHECK_FALSE(contem(saida.texto(), "Total:"));
}

TEST_CASE("Cliente: listarVeiculos mostra todas as informacoes de cada veiculo") {
    Cliente c("Kaio Jorge Silva", "12345678900", "11987654321", "Rua das Flores, 100");
    Veiculo v1("FIAT", "Palio", 2010, "ABC1234", 150000);
    Veiculo v2("FORD", "Ka", 2015, "XYZ9876", 50000);
    c.setVeiculos(&v1);
    c.setVeiculos(&v2);

    CapturaSaida saida(std::cout);
    c.listarVeiculos();
    saida.parar();
    const std::string texto = saida.texto();

    CHECK(contem(texto, "Total: 2"));

    CHECK(contem(texto, "Marca: FIAT"));
    CHECK(contem(texto, "Modelo: Palio"));
    CHECK(contem(texto, "Ano: 2010"));
    CHECK(contem(texto, "Placa: ABC1234"));
    CHECK(contem(texto, "Quilometragem: 150000 km"));

    CHECK(contem(texto, "Marca: FORD"));
    CHECK(contem(texto, "Modelo: Ka"));
    CHECK(contem(texto, "Ano: 2015"));
    CHECK(contem(texto, "Placa: XYZ9876"));
    CHECK(contem(texto, "Quilometragem: 50000 km"));
}



TEST_CASE("Cliente: Associar e consultar ordens de servico") {
    Cliente c("Mateus Pereira", "12345678900", "1198987441", "Rua das Flores, 100");
    Veiculo v("FIAT", "Palio", 2010, "ABC1234", 150000);
    OrdemDeServico ordem(&v, "06/10/2026");

    c.setServicos(&ordem);

    const std::vector<OrdemDeServico*>& servicos = c.getServicos();
    REQUIRE(servicos.size() == 1);
    CHECK(servicos[0] == &ordem);
}




TEST_CASE("Cliente: listarServicos sem ordens") {
    Cliente c("Mateus Pereira", "12345678900", "1198987441", "Rua das Flores, 100");

    CapturaSaida saida(std::cout);
    c.listarServicos();
    saida.parar();

    CHECK(contem(saida.texto(), "desse cliente"));
}

TEST_CASE("Cliente: listarServicos mostra o veiculo de cada ordem") {
    Cliente c("Mateus Pereira", "12345678900", "1198987441", "Rua das Flores, 100");
    Veiculo v1("FIAT", "Palio", 2010, "ABC1234", 150000);
    Veiculo v2("FORD", "Ka", 2015, "XYZ9876", 50000);
    OrdemDeServico ordem1(&v1, "06/10/2026");
    OrdemDeServico ordem2(&v2, "07/10/2026");
    c.setServicos(&ordem1);
    c.setServicos(&ordem2);

    CapturaSaida saida(std::cout);
    c.listarServicos();
    saida.parar();
    const std::string texto = saida.texto();

    CHECK(contem(texto, "ABC1234"));
    CHECK(contem(texto, "XYZ9876"));
    CHECK(contem(texto, "Nenhum"));   // nenhuma das duas tem diagnostico
}



TEST_CASE("Cliente: Associar agendamento") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua das Flores, 100");
    Agendamento a("10/12/2026", "14:30");

    c.setAgendamento(&a);

    // Cliente nao tem getAgendamentos(); a verificacao e feita pela listagem
    CapturaSaida saida(std::cout);
    c.listarAgendamentos();
    saida.parar();

    CHECK(contem(saida.texto(), "Total: 1"));
    CHECK(contem(saida.texto(), "10/12/2026"));
    CHECK(contem(saida.texto(), "14:30"));
}




TEST_CASE("Cliente: listarAgendamentos sem agendamentos") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua das Flores, 100");

    CapturaSaida saida(std::cout);
    c.listarAgendamentos();
    saida.parar();

    CHECK(contem(saida.texto(), "Nenhum agendamento associado a esse cliente"));
    CHECK_FALSE(contem(saida.texto(), "Total:"));
}

TEST_CASE("Cliente: listarAgendamentos mostra o status de cada agendamento") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua das Flores, 100");
    Agendamento pendente("10/12/2026", "14:30");
    Agendamento confirmado("11/12/2026", "09:00");
    Agendamento cancelado("12/12/2026", "16:00");
    confirmado.confirmar();
    cancelado.cancelar();

    c.setAgendamento(&pendente);
    c.setAgendamento(&confirmado);
    c.setAgendamento(&cancelado);

    CapturaSaida saida(std::cout);
    c.listarAgendamentos();
    saida.parar();
    const std::string texto = saida.texto();

    CHECK(contem(texto, "Total: 3"));
    CHECK(contem(texto, "Data: 10/12/2026"));
    CHECK(contem(texto, "Pendente"));
    CHECK(contem(texto, "Data: 11/12/2026"));
    CHECK(contem(texto, "Confirmado"));
    CHECK(contem(texto, "Data: 12/12/2026"));
    CHECK(contem(texto, "Cancelado"));
}


TEST_CASE("Cliente: pesquisarPorCpf encontra o cliente cadastrado") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua das Flores, 100");

    CHECK(Cliente::pesquisarPorCpf("12345678900") == &c);
}



TEST_CASE("Cliente: pesquisarPorCpf recusa CPF invalido") {
    CHECK_THROWS_AS(Cliente::pesquisarPorCpf(""), std::invalid_argument);
    CHECK_THROWS_AS(Cliente::pesquisarPorCpf("abc"), std::invalid_argument);
    CHECK_THROWS_AS(Cliente::pesquisarPorCpf("123.456.789-00"), std::invalid_argument);
}

TEST_CASE("Cliente: excluirCliente tira o cliente da lista") {
    Cliente c("Carlos Silva", "12345678900", "11987654321", "Rua das Flores, 100");
    REQUIRE(existeCliente("12345678900"));

    c.excluirCliente();
    CHECK_FALSE(existeCliente("12345678900"));

    // chamar de novo (o destrutor tambem chama) nao pode dar problema
    CHECK_NOTHROW(c.excluirCliente());
}





TEST_CASE("Cliente: salvarNoArquivo grava o status do agendamento") {
    ArquivoTemporario arq("teste_clientes_status.txt");

    Cliente c("Joao Silva", "12345678900", "31999998888", "Rua A");
    Agendamento confirmado("11/12/2026", "09:00");
    Agendamento cancelado("12/12/2026", "16:00");
    confirmado.confirmar();
    cancelado.cancelar();
    c.setAgendamento(&confirmado);
    c.setAgendamento(&cancelado);

    CapturaSaida saida(std::cout);
    Cliente::salvarNoArquivo(arq.nome);
    saida.parar();

    const std::vector<std::string> linhas = lerLinhasDoArquivo(arq.nome);
    REQUIRE(linhas.size() == 3);
    CHECK(linhas[1] == "A;12345678900;11/12/2026;09:00;CONFIRMADO");
    CHECK(linhas[2] == "A;12345678900;12/12/2026;16:00;CANCELADO");
}

TEST_CASE("Cliente: salvarNoArquivo grava os dados de cada cliente junto do seu CPF") {
    ArquivoTemporario arq("teste_clientes_varios.txt");

    Cliente a("Ana Souza", "11122233344", "31988887777", "Rua A");
    Cliente b("Bia Costa", "55566677788", "31977776666", "Rua B");
    Veiculo v("FORD", "Ka", 2015, "XYZ9876", 50000);
    a.setVeiculos(&v);

    CapturaSaida saida(std::cout);
    Cliente::salvarNoArquivo(arq.nome);
    saida.parar();

    const std::vector<std::string> linhas = lerLinhasDoArquivo(arq.nome);
    REQUIRE(linhas.size() == 3);
    CHECK(linhas[0] == "C;Ana Souza;11122233344;31988887777;Rua A");
    CHECK(linhas[1] == "V;11122233344;FORD;Ka;2015;XYZ9876;50000");
    CHECK(linhas[2] == "C;Bia Costa;55566677788;31977776666;Rua B");
}


TEST_CASE("Cliente: salvarNoArquivo avisa quando nao consegue abrir o arquivo") {
    CapturaSaida saida(std::cout);
    Cliente::salvarNoArquivo("diretorio_que_nao_existe_xyz/clientes.txt");
    saida.parar();

    CHECK(contem(saida.texto(), "Erro ao abrir o arquivo"));
}




TEST_CASE("Cliente: carregarDoArquivo avisa quando o arquivo nao existe") {
    CapturaSaida saida(std::cout);
    Cliente::carregarDoArquivo("arquivo_que_nao_existe_xyz.txt");
    saida.parar();

    CHECK(contem(saida.texto(), "Nenhum cliente anterior encontrado"));
}
