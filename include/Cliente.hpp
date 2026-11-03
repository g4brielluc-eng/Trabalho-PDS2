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
        const std::string nome;
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
        Cliente(std::string nome, std::string cpf, std::string contato, std::string endereco);

        std::string getCpf();
        std::string getEndereco();
        std::string getNome();


        std::string getContato();
        void setContato(std::string contato); 

        std::vector<Veiculo*>& getVeiculos();
        void setVeiculos(Veiculo* veiculo);

        std::vector<OrdemDeServico*>& getServicos();
        void setServicos(OrdemDeServico* ordemdeservico);

        void setAgendamento(Agendamento* agendamento);

        ~Cliente();        
};



#endif