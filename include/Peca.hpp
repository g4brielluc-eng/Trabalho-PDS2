/**
 * @file Peca.hpp
 * @brief Declaração da classe Peca.
 */

#ifndef PECA_H
#define PECA_H
#include <iostream>

#include <string>
#include <vector>

class Veiculo;
class OrdemDeServico;


/**
 * @class Peca
 * @brief Representa uma peça do estoque da oficina.
 *
 * Armazena código, descrição, preço e quantidade em estoque da peça,
 * além dos veículos e ordens de serviço associados a ela.
 */
class Peca{
   
        /// Código identificador da peça.
        std::string codigo;
        /// Descrição da peça.
        std::string descricao;
        /// Preço unitário da peça.
        float preco;
        /// Quantidade da peça em estoque.
        int quantidade;
        /// Lista de veículos associados à peça.
        std::vector<Veiculo*> veiculos;
        /// Lista de ordens de serviço que utilizam a peça.
        std::vector<OrdemDeServico*> servicos;
        /// Lista de todas as peças cadastradas.
        static std::vector<Peca*> listaPecas;

    public:
        /** @brief Construtor da classe Peca.
         * @param codigo Código identificador da peça.
         * @param preco Preço unitário da peça.
         * @param quantidade Quantidade da peça em estoque.  
         */
        Peca(std::string codigo,float preco,int quantidade);

        /** @brief Lista todas as peças cadastradas.
         * @param apenasEmFalta Se @c true, lista apenas as peças em falta.
         */
        static void listarTodas(bool apenasEmFalta = false);
        
        /** @brief Pesquisa uma peça pelo código.
         * @param codigoBuscado Código da peça a ser pesquisada.
         */
        static Peca* pesquisarPorCodigo(const std::string& codigoBuscado);

        /** @brief Salva as peças em um arquivo.
         * @param nomeArquivo Nome do arquivo onde as peças serão salvas.
         */
        static void salvarNoArquivo(const std::string& nomeArquivo);
        
        /** @brief Carrega as peças de um arquivo.
         * @param nomeArquivo Nome do arquivo de onde as peças serão carregadas.
         */
        static void carregarDoArquivo(const std::string& nomeArquivo);
        

        /**
         * @brief Define o código da peça.
         * @param codigo Novo código da peça.
         */
        void setCodigo(std::string codigo);

        /// @brief Retorna o código da peça.
        std::string getCodigo() const;
        
        /**
         * @brief Define a descrição da peça.
         * @param descricao Nova descrição da peça.
         */
        void setDescricao(std::string descricao);

        /// @brief Retorna a descrição da peça.
        std::string getDescricao() const;

        /**
         * @brief Define o preço da peça.
         * @param preco Novo preço unitário da peça.
         */
        void setPreco(float preco);

        /// @brief Retorna o preço unitário da peça.
        float getPreco() const;

        /**
         * @brief Obtém a quantidade da peça em estoque.
         * @return Quantidade em estoque.
         */
        int getQuantidade() const;

        /**
         * @brief Define a quantidade da peça em estoque.
         * @param newquantidade Nova quantidade em estoque.
         */
        void setQuantidade(int newquantidade);

        /**
         * @brief Verifica se a peça está disponível em estoque.
         * @return @c true se houver peça disponível, @c false caso contrário.
         */
        bool disponivel() const;

        /**
         * @brief Obtém as ordens de serviço associadas à peça.
         * @return Vetor de ponteiros para as ordens de serviço.
         */
        std::vector<OrdemDeServico*> getServicos() const;

        /**
         * @brief Associa uma ordem de serviço à peça.
         * @param ordemdeservico Ponteiro para a ordem de serviço a ser associada.
         */
        void setServicos(OrdemDeServico * ordemdeservico);

};



#endif