/**
 * @file OrdemDeServico.hpp
 * @brief Declaração da classe OrdemDeServico.
 */

#ifndef ORDEM_DE_SERVICO_HPP
#define ORDEM_DE_SERVICO_HPP

#include <string>
#include <vector>

class Veiculo;
class Servico;
class Peca;
class Mecanico;

/**
 * @brief Representa uma ordem de serviço da oficina mecânica.
 *
 * A classe OrdemDeServico armazena as informações relacionadas
 * à execução de um serviço, incluindo o veículo, os serviços
 * solicitados, as peças utilizadas, o mecânico responsável,
 * o diagnóstico e o status da ordem.
 */
class OrdemDeServico {
private:

    /// Diagnóstico realizado no veículo.
    std::string diagnostico;

    /// Status atual da ordem de serviço.
    std::string status;

    /// Data de entrada do veículo na oficina.
    std::string dataEntrada;

    /// Data de saída do veículo da oficina.
    std::string dataSaida;

    /// Veículo relacionado à ordem de serviço.
    Veiculo* veiculo;

    /// Mecânico responsável pela ordem de serviço.
    Mecanico* mecanico;

    /// Lista de serviços solicitados na ordem.
    std::vector<Servico*> servicos;

    /// Lista de peças utilizadas na ordem.
    std::vector<Peca*> pecas;

public:

    /**
     * @brief Construtor da classe OrdemDeServico.
     * @param veiculo Veículo relacionado à ordem de serviço.
     * @param dataEntrada Data de entrada do veículo na oficina.
     */
    OrdemDeServico(Veiculo* veiculo,
                   const std::string& dataEntrada);

    /**
     * @brief Associa um veículo à ordem de serviço.
     * @param veiculo Veículo a ser associado.
     */
    void associarVeiculo(Veiculo* veiculo);

    /**
     * @brief Adiciona um serviço à ordem de serviço.
     * @param servico Serviço a ser adicionado.
     */
    void adicionarServico(Servico* servico);

    /**
     * @brief Adiciona uma peça utilizada na ordem de serviço.
     * @param peca Peça a ser adicionada.
     */
    void adicionarPeca(Peca* peca);

    /**
     * @brief Altera o status da ordem de serviço.
     * @param status Novo status da ordem.
     */
    void alterarStatus(const std::string& status);

    /**
     * @brief Registra o diagnóstico do veículo.
     * @param diagnostico Diagnóstico realizado.
     */
    void registrarDiagnostico(const std::string& diagnostico);

    /**
     * @brief Calcula o valor total da ordem de serviço.
     * @return Valor total dos serviços e peças utilizados.
     */
    float calcularValorTotal() const;

    /**
     * @brief Define a data de entrada do veículo.
     * @param data Data de entrada.
     */
    void definirDataEntrada(const std::string& data);

    /**
     * @brief Define a data de saída do veículo.
     * @param data Data de saída.
     */
    void definirDataSaida(const std::string& data);

    /**
     * @brief Associa um mecânico responsável à ordem de serviço.
     * @param mecanico Mecânico responsável.
     */
    void associarMecanico(Mecanico* mecanico);

    /**
     * @brief Finaliza a ordem de serviço.
     */
    void finalizar();

};

#endif // ORDEM_DE_SERVICO_HPP