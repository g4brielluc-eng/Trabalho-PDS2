/**
 * @file Servico.hpp
 * @brief Declaração da classe Servico.
 */

#ifndef SERVICO_HPP
#define SERVICO_HPP

#include <string>
#include <vector>

class OrdemDeServico;

/**
 * @brief Representa um tipo de serviço oferecido pela oficina.
 *
 * A classe Servico armazena o nome, a descrição, o preço e a duração
 * estimada de um serviço (por exemplo, "Troca de óleo"). Um mesmo serviço
 * pode ser solicitado em várias ordens de serviço, que ficam registradas
 * nele por meio de associarOrdem().
 *
 * Os métodos que recebem valores inválidos (preço negativo, duração
 * não positiva, texto vazio ou ponteiro nulo) lançam std::invalid_argument.
 *
 * Colabora com:
 * - OrdemDeServico: utiliza o serviço e o preço dele no cálculo do total;
 * - Agendamento e Mecanico: referenciam o serviço solicitado/executado.
 */
class Servico {
private:
    /// Nome do serviço (não pode ser vazio).
    std::string nome;

    /// Descrição detalhada do serviço.
    std::string descricao;

    /// Preço base do serviço, em reais (>= 0).
    float preco;

    /// Duração estimada do serviço, em minutos (> 0).
    int duracaoEstimada;

    /// Ordens de serviço às quais este serviço está associado.
    std::vector<OrdemDeServico*> ordens;

public:
    /**
     * @brief Cadastra um novo tipo de serviço.
     * @param nomeServico Nome do serviço (não pode ser vazio).
     * @param descricaoServico Descrição do serviço.
     * @param precoServico Preço base do serviço (deve ser >= 0).
     * @param duracaoMinutos Duração estimada em minutos (deve ser > 0).
     * @throws std::invalid_argument se o nome for vazio, o preço for
     *         negativo ou a duração for menor ou igual a zero.
     */
    Servico(const std::string& nomeServico,
            const std::string& descricaoServico,
            float precoServico,
            int duracaoMinutos);

    /**
     * @brief Retorna o nome do serviço.
     * @return Nome do serviço.
     */
    const std::string& getNome() const;

    /**
     * @brief Retorna a descrição do serviço.
     * @return Descrição do serviço.
     */
    const std::string& getDescricao() const;

    /**
     * @brief Retorna o preço base do serviço.
     * @return Preço do serviço, em reais.
     */
    float getPreco() const;

    /**
     * @brief Retorna a duração estimada do serviço.
     * @return Duração estimada, em minutos.
     */
    int getDuracaoEstimada() const;

    /**
     * @brief Retorna as ordens de serviço associadas a este serviço.
     * @return Lista de ponteiros para as ordens associadas.
     */
    const std::vector<OrdemDeServico*>& getOrdens() const;

    /**
     * @brief Atualiza a descrição do serviço.
     * @param novaDescricao Nova descrição do serviço.
     */
    void atualizarDescricao(const std::string& novaDescricao);

    /**
     * @brief Atualiza o preço do serviço.
     *
     * A alteração vale apenas para novos cálculos; não há histórico
     * de preços anteriores.
     *
     * @param novoPreco Novo preço do serviço (deve ser >= 0).
     * @throws std::invalid_argument se novoPreco for negativo.
     */
    void atualizarPreco(float novoPreco);

    /**
     * @brief Registra a duração estimada do serviço.
     * @param minutos Nova duração estimada, em minutos (deve ser > 0).
     * @throws std::invalid_argument se minutos for menor ou igual a zero.
     */
    void registrarDuracaoEstimada(int minutos);

    /**
     * @brief Consulta as informações do serviço.
     * @return Texto com nome, descrição, preço e duração estimada.
     */
    std::string consultarInformacoes() const;

    /**
     * @brief Associa o serviço a uma ordem de serviço.
     *
     * Se a ordem já estiver associada, a chamada não tem efeito
     * (a mesma ordem não é registrada duas vezes).
     *
     * @param ordem Ordem de serviço que utiliza este serviço (não pode ser nula).
     * @throws std::invalid_argument se a ordem for nula.
     */
    void associarOrdem(OrdemDeServico* ordem);

    /**
     * @brief Calcula o custo do serviço.
     * @param quantidade Quantas vezes o serviço é executado (deve ser >= 1).
     * @return Preço do serviço multiplicado pela quantidade.
     * @throws std::invalid_argument se quantidade for menor que 1.
     */
    float calcularCusto(int quantidade = 1) const;
};

#endif // SERVICO_HPP
