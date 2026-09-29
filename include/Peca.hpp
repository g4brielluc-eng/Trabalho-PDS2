#ifndef PECA_H
#define PECA_H
#include <iostream>

#include <string>
#include <vector>

class Veiculo;
class OrdemDeServico;


class Peca{
   
        std::string codigo;
        std::string descricao;
        float preco;
        int quantidade;
        std::vector<Veiculo*> veiculos;
        std::vector<OrdemDeServico*> servicos;

    public:
        void setCodigo(std::string codigo);
        void setDescricao(std::string descricao);
        void setPreco(float preco);


        int getQuantidade() const;
        void setQuantidade(int newquantidade);

        bool disponivel() const;

        std::vector<OrdemDeServico*> getServicos() const;
        void setServicos(OrdemDeServico * ordemdeservico);

};



#endif
