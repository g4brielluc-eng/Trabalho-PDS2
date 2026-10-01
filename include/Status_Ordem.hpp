#ifndef STATUS_ORDEM_HPP
#define STATUS_ORDEM_HPP

/**
 * @brief Status possiveis de uma ordem de servico.
 *
 * Fluxo esperado:
 * @code
 * ABERTA -> EM_DIAGNOSTICO -> EM_EXECUCAO <-> AGUARDANDO_PECA
 *        -> AGUARDANDO_RETIRADA -> FINALIZADA
 * @endcode
 */
enum class StatusOrdem {
    ABERTA,               ///< Ordem criada, ainda sem diagnostico.
    EM_DIAGNOSTICO,       ///< Mecanico avaliando o veiculo.
    EM_EXECUCAO,          ///< Servicos em andamento.
    AGUARDANDO_PECA,      ///< Execucao parada esperando peca necessaria.
    AGUARDANDO_RETIRADA,  ///< Servico concluido; veiculo ainda na oficina.
    FINALIZADA            ///< Veiculo entregue ao cliente (so via OrdemDeServico::finalizar).
};

#endif