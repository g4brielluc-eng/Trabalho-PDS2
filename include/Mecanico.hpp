#ifndef MECANICO_HPP
#define MECANICO_HPP

#include <string>
#include <vector>
#include "status_ordem.hpp"

class OrdemDeServico;
class Servico;

/**
 * @brief Representa um mecanico da oficina.
 *
 * Os metodos que recebem uma OrdemDeServico exigem que ela esteja
 * atribuida a este mecanico; caso contrario, lancam std::logic_error.
 */
class Mecanico {

private:
    int id_; //ID unico do mecanico.
    std::string nome_; //Nome completo do mecanico.
    std::string especialidade_; //Especialidade do mecanico; pode ser alterada posteriormente.
    std::vector<OrdemDeServico*> ordens_; //Lista de ordens de servico atribuidas ao mecanico.

public:
    /**
     * @brief Cadastra um mecanico.
     * @param id ID unico do mecanico.
     * @param nome Nome do mecanico.
     * @param especialidade Area de atuacao do mecanico.
     */
    Mecanico(int id, const std::string& nome, const std::string& especialidade);

    /// @brief Retorna o ID do mecanico.
    int getId() const;

    /// @brief Retorna o nome do mecanico.
    std::string getNome() const;

    /// @brief Retorna a especialidade do mecanico.
    std::string getEspecialidade() const;

    /**
     * @brief Altera a especialidade.
     * @param especialidade Nova especialidade do mecanico.
     */
    void alterarEspecialidade(const std::string& especialidade);

    /// @brief Retorna as ordens de servico atribuidas ao mecanico.
    std::vector<OrdemDeServico*> consultarOrdensAtribuidas() const;

    /**
     * @brief Assume uma ordem de servico.
     * @param ordem Ordem a assumir (nao pode ser nula).
     * @throws std::invalid_argument se a ordem for nula.
     */
    void assumirOrdem(OrdemDeServico* ordem);

    /**
     * @brief Registra o diagnostico em uma ordem atribuida ao mecanico.
     * @param ordem Ordem atribuida ao mecanico.
     * @param diagnostico Problema encontrado no veiculo.
     */
    void registrarDiagnostico(OrdemDeServico* ordem, const std::string& diagnostico);

    /**
     * @brief Registra um servico realizado em uma ordem.
     * @param ordem Ordem atribuida ao mecanico.
     * @param servico Servico executado pelo mecanico.
     */
    void registrarServicoRealizado(OrdemDeServico* ordem, Servico* servico);

    /**
     * @brief Atualiza o andamento (status) da ordem.
     *
     * O mecanico so atualiza o status da ordem ate "AGUARDANDO_RETIRADA".
     * Ele nao pode alterar o status para FINALIZADA; a finalizacao
     * (entrega do veiculo) e feita por OrdemDeServico::finalizar.
     *
     * @param ordem Ordem atribuida a este mecanico.
     * @param novoStatus Novo status.
     * @throws std::invalid_argument se novoStatus for FINALIZADA.
     */
    void atualizarAndamento(OrdemDeServico* ordem, StatusOrdem novoStatus);
};

#endif