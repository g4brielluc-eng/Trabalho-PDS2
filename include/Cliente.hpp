#ifndef CLIENTE_H
#define CLIENTE_H

#include <iostream>
#include <string>
#include <vector>


class Veiculo;
class OrdemDeServico;
class Agendamento;

class Cliente{

    protected:
        const std::string nome
        std::string cpf, contato, endereco;
        std::vector<Veiculo*> veiculos;        
        std::vector<OrdemDeServico*> servicos;
        std::vector<Agendamento*> agendamentos;
        

    public:
        Cliente(std::string nome,  std::string cpf, std::string contato, std::string endereco);
        void setContato(int contato);
        Veiculo getVeiculos();
        void setVeiculos(Veiculo veiculo);
        OrdemDeServico getServicos();
        void setServicos(OrdemDeServico ordemdeservico);
        void setAgendamento (Agendamento);

};



#endif
