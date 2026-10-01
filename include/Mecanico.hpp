#ifndef MECANICO_HPP
#define MECANICO_HPP

#include <string>
#include <vector>
#include "status_ordem.hpp"

class OrdemDeServico;
class Servico;

/**
 * @brief Representa um mecânico da oficina.
 *
 * Os métodos que recebem uma OrdemDeServico exigem que ela esteja
 * atribuída a este mecânico; caso contrário, lançam std::logic_error.
 */
class Mecanico {

private:
    int id_; //ID único do mecânico.
    std::string nome_; //Nome completo do mecânico.
    std::string especialidade_; //Especialidade do mecânico; pode ser alterada posteriormente.
    std::vector<OrdemDeServico*> ordens_; //Lista de ordens de serviço atribuídas ao mecânico.

public:
    /**
     * @brief Cadastra um mecânico.
     * @param id ID único do mecânico.
     * @param nome Nome do mecânico.
     * @param especialidade Área de atuação do mecânico.
     */
    Mecanico(int id, const std::string& nome, const std::string& especialidade);

    /// @brief Retorna o ID do mecânico.
    int getId() const;

    /// @brief Retorna o nome do mecânico.
    std::string getNome() const;

    /// @brief Retorna a especialidade do mecânico.
    std::string getEspecialidade() const;

    /**
     * @brief Altera a especialidade.
     * @param especialidade Nova especialidade do mecânico.
     */
    void alterarEspecialidade(const std::string& especialidade);

    /// @brief Retorna as ordens de serviço atribuídas ao mecânico.
    std::vector<OrdemDeServico*> consultarOrdensAtribuidas() const;

    /**
     * @brief Assume uma ordem de serviço.
     * @param ordem Ordem a assumir (não pode ser nula).
     * @throws std::invalid_argument se a ordem for nula.
     */
    void assumirOrdem(OrdemDeServico* ordem);

    /**
     * @brief Registra o diagnóstico em uma ordem atribuída ao mecânico.
     * @param ordem Ordem atribuída ao mecânico.
     * @param diagnostico Problema encontrado no veículo.
     */
    void registrarDiagnostico(OrdemDeServico* ordem, const std::string& diagnostico);

    /**
     * @brief Registra um serviço realizado em uma ordem.
     * @param ordem Ordem atribuída ao mecânico.
     * @param servico Serviço executado pelo mecânico.
     */
    void registrarServicoRealizado(OrdemDeServico* ordem, Servico* servico);

    /**
     * @brief Atualiza o andamento (status) da ordem.
     *
     * O mecânico só atualiza o status da ordem até "AGUARDANDO_RETIRADA".
     * Ele não pode alterar o status para FINALIZADA; a finalização
     * (entrega do veículo) é feita por OrdemDeServico::finalizar.
     *
     * @param ordem Ordem atribuída a este mecânico.
     * @param novoStatus Novo status.
     * @throws std::invalid_argument se novoStatus for FINALIZADA.
     */
    void atualizarAndamento(OrdemDeServico* ordem, StatusOrdem novoStatus);
};

#endif