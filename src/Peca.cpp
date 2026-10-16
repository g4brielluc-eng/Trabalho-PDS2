#include "Peca.hpp"
#include <iostream>

#include <string>
#include <vector>

        void Peca::setCodigo(std::string codigo){
            this -> nome = codigo;
        }

        void Peca::setDescricao(std::string descricao){
            this -> descricao = descricao;
        }

       
        void Peca::setPreco(float preco){
            this -> preco = preco;
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




