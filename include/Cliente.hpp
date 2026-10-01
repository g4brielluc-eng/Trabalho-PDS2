/**
 * @file Cliente.h
 * @brief Declaração da classe Cliente.
 */

#ifndef CLIENTE_H
#define CLIENTE_H

#include <iostream>
#include <string>
#include <vector>


class Veiculo;
class OrdemDeServico;
class Agendamento;

/**
 * @class Cliente
 * @brief Representa um cliente da oficina.
 *
 * Armazena os dados pessoais do cliente, além dos veículos,
 * ordens de serviço e agendamentos associados a ele.
 */
class Cliente{

    protected:
        /// Nome do cliente (não pode ser alterado após a construção).
        const std::string nome
        /// CPF do cliente.
        /// Contato do cliente (telefone, e-mail, etc.).
        /// Endereço do cliente.
        std::string cpf, contato, endereco;
        /// Lista de veículos pertencentes ao cliente.
        std::vector<Veiculo*> veiculos;        
        /// Lista de ordens de serviço associadas ao cliente.
        std::vector<OrdemDeServico*> servicos;
        /// Lista de agendamentos feitos pelo cliente.
        std::vector<Agendamento*> agendamentos;
        

    public:
        /**
         * @brief Construtor da classe Cliente.
         * @param nome Nome do cliente.
         * @param cpf CPF do cliente.
         * @param contato Informação de contato do cliente.
         * @param endereco Endereço do cliente.
         */
        Cliente(std::string nome,  std::string cpf, std::string contato, std::string endereco);

        /**
         * @brief Define o contato do cliente.
         * @param contato Novo contato do cliente.
         */
        void setContato(int contato);

        /**
         * @brief Obtém um veículo do cliente.
         * @return Veículo do cliente.
         */
        Veiculo getVeiculos();

        /**
         * @brief Associa um veículo ao cliente.
         * @param veiculo Veículo a ser associado.
         */
        void setVeiculos(Veiculo veiculo);

        /**
         * @brief Obtém uma ordem de serviço do cliente.
         * @return Ordem de serviço do cliente.
         */
        OrdemDeServico getServicos();

        /**
         * @brief Associa uma ordem de serviço ao cliente.
         * @param ordemdeservico Ordem de serviço a ser associada.
         */
        void setServicos(OrdemDeServico ordemdeservico);

        /**
         * @brief Associa um agendamento ao cliente.
         * @param agendamento Agendamento a ser associado.
         */
        void setAgendamento (Agendamento);

};



#endif