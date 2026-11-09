#include "Peca.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>    
#include <sstream>     

std::vector<Peca*> Peca::listaPecas;

        Peca::Peca(std::string codigo, float preco, int quantidade) {
    
        setCodigo(codigo);
        setPreco(preco);
        setQuantidade(quantidade);
        
        this->descricao = "Peça sem descrição";
        
        listaPecas.push_back(this);
        }


        void Peca::listarTodas(bool apenasEmFalta) {
            if (listaPecas.empty()) {
                std::cout << "Nenhuma peça cadastrada no sistema.\n";
                return;
            }
            if(!apenasEmFalta){
            std::cout << "\n-------------- LISTA DE PEÇAS --------------\n";
            for (const auto& p : listaPecas) {
                std::cout << "Código: " << p->getCodigo() 
                        << " | Descrição: " << p->getDescricao() 
                        << " | Preço: R$" << p->getPreco() 
                        << " | Qtd: " << p->getQuantidade() << "\n";
            }
            std::cout << "--------------------------------\n";
            }
            else{
            std::cout << "\n-------------- LISTA DE PEÇAS EM FALTA --------------\n";
            for (const auto& p : listaPecas) {
                if(!p->disponivel()){
                std::cout << "Código: " << p->getCodigo() 
                        << " | Descrição: " << p->getDescricao() 
                        << " | Preço: R$" << p->getPreco() 
                        << " | Qtd: " << p->getQuantidade() << "\n";
                }
            }
            std::cout << "--------------------------------\n";
            }
        }
        Peca* Peca::pesquisarPorCodigo(const std::string& codigoBuscado) {
            for (const auto& p : listaPecas) {
                if (p->getCodigo() == codigoBuscado) {
                    return p; 
                }
            }
            return nullptr;
        }
     
        void Peca::salvarNoArquivo(const std::string& nomeArquivo) {
            std::ofstream arquivo(nomeArquivo);
            if (arquivo.is_open()) {
                for (const auto& p : listaPecas) {
                    arquivo << p->codigo << ";" 
                            << p->descricao << ";" 
                            << p->preco << ";" 
                            << p->quantidade << "\n";
                }
                arquivo.close();
                std::cout << "Estoque salvo com sucesso em '" << nomeArquivo << "'!\n";
            } else {
                std::cout << "Erro ao abrir o arquivo para escrita!\n";
            }
        }






        void Peca::carregarDoArquivo(const std::string& nomeArquivo) {
            std::ifstream arquivo(nomeArquivo);
            std::string linha;

            if (arquivo.is_open()) {
                int carregados = 0;
                while (std::getline(arquivo, linha)) {
                    std::stringstream ss(linha);
                    std::string cod, desc, precoStr, qtdStr;

                    std::getline(ss, cod, ';');
                    std::getline(ss, desc, ';');
                    std::getline(ss, precoStr, ';');
                    std::getline(ss, qtdStr);

                    if (!cod.empty() && !precoStr.empty() && !qtdStr.empty()) {
                        float pr = std::stof(precoStr);
                        int qtd = std::stoi(qtdStr);
                        bool jaExiste = false;
                        for (const auto& p : listaPecas) {
                            if (p->getCodigo() == cod) {
                                jaExiste = true;
                                break;
                            }
                        }

                        if (!jaExiste) {
                            Peca* novaPeca = new Peca(cod, pr, qtd);
                            novaPeca->setDescricao(desc);
                            carregados++;
                        }
                    }
                }
                arquivo.close();
                std::cout << carregados << " peças carregadas do arquivo com sucesso!\n";
            } else {
                std::cout << "Nenhum arquivo anterior encontrado. Iniciando com estoque vazio.\n";
            }
        }





        void Peca::setCodigo(std::string codigo){
                Peca* teste = Peca::pesquisarPorCodigo(codigo);
                if (teste != nullptr && teste != this)
                        throw std::invalid_argument("Código já utilizado");            
                else
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
            if (preco > 0) {
                this -> preco = preco;
            } else
                throw std::invalid_argument("Preço inválido");
        }

        float Peca::getPreco() const{
            return preco;
        }

    
        int Peca::getQuantidade() const{
            return quantidade;
        }


    
        void Peca::setQuantidade(int quantidade){
             if (quantidade >= 0) {
                this -> quantidade = quantidade;
            } else
                throw std::invalid_argument("Quantidade inválida");
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




