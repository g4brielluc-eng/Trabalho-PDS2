#ifndef MECANICO_HPP
#define MECANICO_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include "Status_Ordem.hpp"

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
    int id; ///ID único do mecânico.
    std::string nome; ///Nome completo do mecânico.
    std::string especialidade; ///Especialidade do mecânico; pode ser alterada posteriormente.
    std::vector<OrdemDeServico*> ordens; ///Lista de ordens de serviço atribuídas ao mecânico.

public:
    /**
     * @brief Cadastra um mecânico.
     * @param id ID único do mecânico.
     * @param nome Nome do mecânico.
     * @param especialidade Área de atuação do mecânico.
     * @throws std::invalid_argument se o id for menor ou igual a zero.
     * @throws std::invalid_argument se o nome ou a especialidade estiverem vazios.
     */
    Mecanico(int id, const std::string& nome, const std::string& especialidade);

    /// @brief Retorna o ID do mecânico.
    int getId() const;

    /// @brief Retorna o nome do mecânico.
    const std::string& getNome() const;

    /// @brief Retorna a especialidade do mecânico.
    const std::string& getEspecialidade() const;

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
     * @throws std::logic_error se a ordem não estiver atribuída ao mecânico.
     */
    void registrarDiagnostico(OrdemDeServico* ordem, const std::string& diagnostico);

    /**
     * @brief Registra um serviço realizado em uma ordem.
     * @param ordem Ordem atribuída ao mecânico.
     * @param servico Serviço executado pelo mecânico (nao pode ser nulo).
     * @throws std::invalid_argument se o servico for nulo. 
     * @throws std::logic_error se a ordem não estiver atribuída ao mecânico.
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
     * @throws std::logic_error se a ordem não estiver atribuída ao mecânico.
     */
    void atualizarAndamento(OrdemDeServico* ordem, StatusOrdem novoStatus);

    /**
     * @brief Carrega os mecânicos de um arquivo.
     * @param lista Vetor para armazenar os mecânicos carregados.
     * @param nomeArquivo Nome do arquivo a ser lido.
     */

    static void carregarMecanicosDoArquivo(std::vector<std::shared_ptr<Mecanico>>& lista, const std::string& nomeArquivo);

    /**
     * @brief Salva os mecânicos em um arquivo.
     * @param lista Vetor contendo os mecânicos a serem salvos.
     * @param nomeArquivo Nome do arquivo a ser escrito.
     * @throws std::runtime_error se houver falha na gravação do arquivo.
     */

    static void salvarMecanicosNoArquivo(const std::vector<std::shared_ptr<Mecanico>>& lista, const std::string& nomeArquivo);

};

#endif