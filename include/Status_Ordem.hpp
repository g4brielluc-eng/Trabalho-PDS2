#ifndef STATUS_ORDEM_HPP
#define STATUS_ORDEM_HPP

/**
 * @brief Status possíveis de uma ordem de serviço.
 *
 * Fluxo esperado:
 * @code
 * ABERTA -> EM_DIAGNOSTICO -> EM_EXECUCAO <-> AGUARDANDO_PECA
 *        -> AGUARDANDO_RETIRADA -> FINALIZADA
 * @endcode
 */
enum class StatusOrdem {
    ABERTA,               ///< Ordem criada, ainda sem diagnóstico.
    EM_DIAGNOSTICO,       ///< Mecânico avaliando o veículo.
    EM_EXECUCAO,          ///< Serviços em andamento.
    AGUARDANDO_PECA,      ///< Execuçao parada esperando peça necessária.
    AGUARDANDO_RETIRADA,  ///< Serviço concluido; veículo ainda na oficina.
    FINALIZADA            ///< Veículo entregue ao cliente (so via OrdemDeServico::finalizar).
};

#endif