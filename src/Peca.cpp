#include "Peca.hpp"
#include <iostream>

#include <string>
#include <vector>

        Peca::Peca( std::string codigo,float preco,int quantidade)
        : codigo(codigo), preco(preco), quantidade(quantidade){
            this->descricao = "Peça sem descrição"; 
        }

        void Peca::setCodigo(std::string codigo){
            this -> codigo = codigo;
        }

       std::string Peca::getCodigo() const{
            return codigo;
        }

        void Peca::setDescricao(std::string descricao){
            this -> descricao = descricao;
        }

        std::string Peca::getDescricao() const{
            return descricao;
        }
       
        void Peca::setPreco(float preco){
            this -> preco = preco;
        }

        float Peca::getPreco() const{
            return preco;
        }

    
        int Peca::getQuantidade() const{
            return quantidade;
        }


    
        void Peca::setQuantidade(int newquantidade){
            this -> quantidade = newquantidade;
        }
        
        bool Peca::disponivel() const{
            if(quantidade > 0) return true;
            return false;
        }

        std::vector<OrdemDeServico*> Peca::getServicos() const{
            return this->servicos;
        }

    
        void Peca::setServicos(OrdemDeServico * ordemdeservico){
            servicos.push_back(ordemdeservico);
        }




