#ifndef AGENDAMENTO_H
#define AGENDAMENTO_H

#include <string>
#include <vector>

class Cliente;
class Veiculo;
class Servico;

/**
 * @brief Representa um agendamento de serviço da oficina.
 *
 * A classe Agendamento armazena as informações referentes
 * ao agendamento de um serviço, incluindo cliente, veículo,
 * serviço solicitado, data, horário e situação do agendamento.
 */
class Agendamento {
public:

    /**
     * @brief Representa a situação atual do agendamento.
     */
    enum class Status {
        PENDENTE,
        CONFIRMADO,
        CANCELADO
    };

private:
    std::string data;
    std::string horario;

    Cliente* cliente;
    Veiculo* veiculo;
    Servico* servico;

    Status status;

    /*
     * Armazena os agendamentos registrados no sistema.
     * É utilizada para consulta e verificação de disponibilidade.
     */
    static std::vector<Agendamento*> agendamentos;

public:

    /**
     * @brief Construtor da classe Agendamento.
     *
     * @param data Data do agendamento.
     * @param horario Horário do agendamento.
     */
    Agendamento(const std::string& data,
                const std::string& horario);

    /**
     * @brief Registra o agendamento no sistema.
     */
    void registrarAgendamento();

    /**
     * @brief Define a data e o horário do agendamento.
     *
     * @param data Data do agendamento.
     * @param horario Horário do agendamento.
     */
    void definirDataHorario(const std::string& data,
                            const std::string& horario);

    /**
     * @brief Associa um cliente ao agendamento.
     *
     * @param cliente Cliente responsável pelo agendamento.
     */
    void associarCliente(Cliente* cliente);

    /**
     * @brief Associa um veículo ao agendamento.
     *
     * @param veiculo Veículo que receberá o serviço.
     */
    void associarVeiculo(Veiculo* veiculo);

    /**
     * @brief Define o serviço solicitado no agendamento.
     *
     * @param servico Serviço solicitado pelo cliente.
     */
    void definirServico(Servico* servico);

    /**
     * @brief Altera a data e o horário do agendamento.
     *
     * @param novaData Nova data do agendamento.
     * @param novoHorario Novo horário do agendamento.
     */
    void alterarData(const std::string& novaData,
                     const std::string& novoHorario);

    /**
     * @brief Cancela o agendamento.
     */
    void cancelar();

    /**
     * @brief Confirma o agendamento.
     */
    void confirmar();

    /**
     * @brief Consulta os agendamentos registrados no sistema.
     *
     * @return Lista de agendamentos registrados.
     */
    static const std::vector<Agendamento*>& consultarAgendamentos();

    /**
     * @brief Verifica se uma data e horário estão disponíveis.
     *
     * @param data Data que será verificada.
     * @param horario Horário que será verificado.
     * @return true se o horário estiver disponível,
     *         false caso contrário.
     */
    static bool verificarDisponibilidade(const std::string& data,
                                         const std::string& horario);

    /**
     * @brief Retorna a data do agendamento.
     *
     * @return Data do agendamento.
     */
    const std::string& getData() const;

    /**
     * @brief Retorna o horário do agendamento.
     *
     * @return Horário do agendamento.
     */
    const std::string& getHorario() const;

    /**
     * @brief Retorna o cliente associado ao agendamento.
     *
     * @return Ponteiro para o cliente associado.
     */
    Cliente* getCliente() const;

    /**
     * @brief Retorna o veículo associado ao agendamento.
     *
     * @return Ponteiro para o veículo associado.
     */
    Veiculo* getVeiculo() const;

    /**
     * @brief Retorna o serviço solicitado.
     *
     * @return Ponteiro para o serviço associado.
     */
    Servico* getServico() const;

    /**
     * @brief Retorna o status atual do agendamento.
     *
     * @return Status atual do agendamento.
     */
    Status getStatus() const;
};

#endif // AGENDAMENTO_H