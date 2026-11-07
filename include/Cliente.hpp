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
        // Nome do cliente (não pode ser alterado após a construção).
         std::string nome;
        // CPF do cliente.
        // Contato do cliente (telefone, e-mail, etc.).
        // Endereço do cliente.
        std::string cpf, contato, endereco;
        // Lista de veículos pertencentes ao cliente.
        std::vector<Veiculo*> veiculos;        
        // Lista de ordens de serviço associadas ao cliente.
        std::vector<OrdemDeServico*> servicos;
        // Lista de agendamentos feitos pelo cliente.
        std::vector<Agendamento*> agendamentos;
        static std::vector<Cliente*> listaClientes;

    public:
        /** @brief Construtor da classe Cliente.
         * @param nome Nome do cliente.
         * @param cpf CPF do cliente.
         * @param contato Contato do cliente.
         * @param endereco Endereço do cliente.
        **/
        Cliente(std::string nome, std::string cpf, std::string contato, std::string endereco);
        Cliente();

        /**
         * @brief Lista todos os clientes cadastrados.
         **/
        static void listarTodas();

        /** 
         * @brief Pesquisa um cliente pelo CPF.
         * @param cpfBuscado CPF do cliente a ser pesquisado. 
         */
        static Cliente* pesquisarPorCpf(const std::string& cpfBuscado);
     
        /** @brief Salva os clientes em um arquivo.
         * @param nomeArquivo Nome do arquivo onde os clientes serão salvos.
         */
        static void salvarNoArquivo(const std::string& nomeArquivo);

        /** @brief Carrega os clientes de um arquivo.
         * @param nomeArquivo Nome do arquivo de onde os clientes serão carregados.
         */
        static void carregarDoArquivo(const std::string& nomeArquivo);

        /// @brief Retorna o CPF do cliente.
        std::string getCpf();

        /// @brief Retorna o endereço do cliente.
        std::string getEndereco();
        
        /// @brief @brief Retorna o nome do cliente.
        std::string getNome();

        /// @brief @brief Retorna o contato do cliente.
        std::string getContato();

        /** @brief Define o contato do cliente.
         * @param contato Novo contato do cliente.
         */
        void setContato(std::string contato); 

        /// @brief Obtém a lista de veículos do cliente.
        std::vector<Veiculo*>& getVeiculos();

        /** @brief Adiciona um veículo à lista de veículos do cliente.
         * @param veiculo Veículo a ser adicionado.
         */
        void setVeiculos(Veiculo* veiculo);
        
        /// @brief Obtém a lista de ordens de serviço do cliente.
        std::vector<OrdemDeServico*>& getServicos();
        
        /** @brief Adiciona uma ordem de serviço à lista de ordens de serviço do cliente.
         * @param ordemdeservico Ordem de serviço a ser adicionada.
         */
        void setServicos(OrdemDeServico* ordemdeservico);
        
        /** @brief Adiciona um agendamento à lista de agendamentos do cliente.
         * @param agendamento Agendamento a ser adicionado.
         */
        void setAgendamento(Agendamento* agendamento);

        ~Cliente();        
};



#endif