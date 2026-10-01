#ifndef VEICULO_H
#define VEICULO_H

#include <string>
#include <vector>

class Cliente;
class OrdemDeServico;

/**
 * @brief Representa um veículo cadastrado no sistema da oficina.
 *
 * A classe Veiculo armazena os dados básicos do veículo,
 * permite sua associação a um cliente e mantém as ordens
 * de serviço relacionadas a ele.
 */
class Veiculo {
private:
    std::string marca;
    std::string modelo;
    int ano;
    std::string placa;
    int quilometragem;

    Cliente* cliente;
    std::vector<OrdemDeServico*> ordensServico;

public:

    /**
     * @brief Construtor da classe Veiculo.
     *
     * @param marca Marca do veículo.
     * @param modelo Modelo do veículo.
     * @param ano Ano do veículo.
     * @param placa Placa do veículo.
     * @param quilometragem Quilometragem atual do veículo.
     */
    Veiculo(const std::string& marca,
            const std::string& modelo,
            int ano,
            const std::string& placa,
            int quilometragem);

    /**
     * @brief Associa o veículo a um cliente.
     *
     * @param cliente Cliente associado ao veículo.
     */
    void associarCliente(Cliente* cliente);

    /**
     * @brief Atualiza as informações básicas do veículo.
     *
     * @param placa Nova placa do veículo.
     */
    void atualizarInformacoes(const std::string& placa);

    /**
     * @brief Registra a quilometragem atual do veículo.
     *
     * @param quilometragem Nova quilometragem do veículo.
     */
    void registrarQuilometragem(int quilometragem);

    /**
     * @brief Consulta as ordens de serviço relacionadas ao veículo.
     *
     * @return Lista de ordens de serviço relacionadas ao veículo.
     */
    const std::vector<OrdemDeServico*>& consultarOrdensServico() const;

    /**
     * @brief Consulta o histórico de manutenções do veículo.
     *
     * O histórico é obtido a partir das ordens de serviço
     * relacionadas ao veículo.
     */
    void consultarHistoricoManutencoes() const;

    /**
     * @brief Retorna a marca do veículo.
     *
     * @return Marca do veículo.
     */
    const std::string& getMarca() const;

    /**
     * @brief Retorna o modelo do veículo.
     *
     * @return Modelo do veículo.
     */
    const std::string& getModelo() const;

    /**
     * @brief Retorna o ano do veículo.
     *
     * @return Ano do veículo.
     */
    int getAno() const;

    /**
     * @brief Retorna a placa do veículo.
     *
     * @return Placa do veículo.
     */
    const std::string& getPlaca() const;

    /**
     * @brief Retorna a quilometragem atual do veículo.
     *
     * @return Quilometragem atual do veículo.
     */
    int getQuilometragem() const;

    /**
     * @brief Retorna o cliente associado ao veículo.
     *
     * @return Ponteiro para o cliente associado.
     */
    Cliente* getCliente() const;
};

#endif // VEICULO_H