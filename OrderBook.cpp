#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#define FALSE 0
#define TRUE 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


// aqui criar dois objetos. o primeiro será o objeto limit e o segundo o objeto market
// ambos os objetos vão possuir uma classe anterior que vai ser, sei la


class Order
{
    public:
        std::string type;
        std::string side;
        float price;
        float qty;
};

void AddToBuyBook(Order *ptr, std::vector<Order>& buyBook)
{

    bool END;
    int i, j;
    Order aux;
    int n = buyBook.size();
    std::cout << "Entramos na funcao AddToBuyBook \n"<< "Vamos agora inserir o elemento: \n";

    if(n == 0){
        buyBook.insert(buyBook.begin(), *ptr);
        std::cout << "Elemento inserido \n";
        return;
    }

    buyBook.insert(buyBook.begin(), *ptr);

    std::cout << "Vamos agora colocar os elementos em ordem, de maior para menor, levando em conta ordem de chegada: \n\n";

    // Agora vamos colocar em ordem, priorizando o mais antigo na frente
    i = 0;
    END = FALSE;
    while(!END && (i+1)!= buyBook.size() )
    {
        std::cout << "Entrei no while \n";
        END = TRUE;
        std::cout << buyBook[i].price<< " " <<buyBook[i].qty << " e "<< buyBook[i+1].price<< " "<< buyBook[i+1].qty<< "\n";

        if(buyBook[i].price <= buyBook[i+1].price)
        {
            std::cout << "Inverterei " << buyBook[i].price<< " " <<buyBook[i].qty << " e "<< buyBook[i+1].price<< " "<< buyBook[i+1].qty<< "\n";
            aux = buyBook[i];
            buyBook[i] = buyBook[i+1];
            buyBook[i+1] = aux;
            std::cout << "Invertido. Nova ordem:" << buyBook[i].price<< " " <<buyBook[i].qty << " e "<< buyBook[i+1].price<< " "<< buyBook[i+1].qty<< "\n";
            END = FALSE;
        }
        i++;
    }

    std::cout << "\nAo fim da inversao, essa eh a nova configuracao do BuyBook: \n";
    for (j = 0; j < buyBook.size(); j++)
    {
        std::cout << buyBook[j].price << " " << buyBook[j].qty << "\n";
    }
}

void AddToSellBook(Order *ptr, std::vector<Order>& sellBook)
{
    bool END;
    int i, j;
    Order aux;
    int n = sellBook.size();
    /*---------------------------------------------------------------------------*/

    std::cout << "Entramos na funcao AddToSellBook \n"<< "Vamos agora inserir o elemento: \n";
    if(n == 0){
        sellBook.insert(sellBook.begin(), *ptr);
        std::cout << "Elemento inserido \n";
        return;
    }
    sellBook.insert(sellBook.begin(), *ptr);

    /*---------------------------------------------------------------------------*/
    std::cout << "Vamos agora colocar os elementos em ordem, de menor para maior, levando em conta ordem de chegada: \n\n";
    // Agora vamos colocar em ordem, priorizando o mais antigo na frente
    i = 0;
    END = FALSE;
    while(END == FALSE && (i+1)!= sellBook.size())
    {
        std::cout << "Entrei no while \n";
        END = TRUE;
        std::cout << sellBook[i].price<< " " <<sellBook[i].qty << " e "<< sellBook[i+1].price<< " "<< sellBook[i+1].qty<< "\n";
        if(sellBook[i].price > sellBook[i+1].price || sellBook[i].price == sellBook[i+1].price)
        {
            std::cout << "Inverterei " << sellBook[i].price<< " " <<sellBook[i].qty << " e "<< sellBook[i+1].price<< " "<< sellBook[i+1].qty<< "\n";
            aux = sellBook[i];
            sellBook[i] = sellBook[i+1];
            sellBook[i+1] = aux;
            std::cout << "Invertido. Nova ordem:" << sellBook[i].price<< " " <<sellBook[i].qty << " e "<< sellBook[i+1].price<< " "<< sellBook[i+1].qty<< "\n";
            END = FALSE;
        }
        i++;
    }

    /*---------------------------------------------------------------------------*/
    std::cout << "\nAo fim da inversao, essa eh a nova configuracao do SellBook: \n";
    for (j = 0; j < sellBook.size(); j++)
    {
        std::cout << sellBook[j].price << " " << sellBook[j].qty << "\n";
    }
}

void Trade(std::vector<Order>& buyBook, std::vector<Order>& sellBook)
{

    bool TRADING = TRUE;
    bool END;
    float quant;
    /*---------------------------------------------------------------------------*/
    std::cout << "\nEntramos na funcao de Trade. \n";
    while( sellBook.size() != 0 && buyBook.size() != 0 && TRADING)
    {

        std::cout << "Entrei no while que garante: nSell != 0 && nBuy != 0 && TRADING \n";
        std::cout << "buyBook[0]: "<< buyBook[0].price << "sellBook[0]: "<< sellBook[0].price<< "\n";

        if(buyBook[0].price >= sellBook[0].price)   //  Se a maior oferta de compra for maior ou igual que a menor oferta de venda, realizamos um trade
        {
            quant = 0; // quantidade que esta sendo trading no preço do topo do livro de vendas
            END = FALSE; // verifica se todos os caras com mesmo preço possível já foram transacionados

            while(!END){
                std::cout << "buyBook[0].qty: "<< buyBook[0].qty << " sellBook[0].qty: "<< sellBook[0].qty<< "\n";
                std::cout << "buyBook[0].price: "<< buyBook[0].price << " sellBook[0].price: "<< sellBook[0].price<< "\n";

                if(buyBook[0].qty > sellBook[0].qty)
                {
                    std::cout << "Estou no caso que temos mais quantidade de compra do que venda nas ofertas 0\n";

                    quant += sellBook[0].qty; // atualizo a quantidade sendo transacionada
                    buyBook[0].qty -= sellBook[0].qty; // atualizo a quantidade no meu buybook[0]


                    std::cout << "quant: "<< quant << "  buyBook[0].qty "<< buyBook[0].qty<< "\n";
                    if(sellBook.size() == 1 || sellBook[1].price != sellBook[0].price){
                        std::cout << "Estou no caso que nao tem mais outro cara no livro de vendas com o mesmo preço que o que eu acabei de esgotar.\n";

                        END = TRUE;
                        std::cout<< "\n--------------------------------------------- \n";
                        std::cout<< "Trade, price: "<< sellBook[0].price<< ", qty: "<< quant<< "\n";
                        std::cout<< "\n--------------------------------------------- \n";

                        sellBook.erase(sellBook.begin());

                        std::cout << "Dei Erase no meu sellBook. \n";
                    }
                    else{
                        std::cout << "Estou no caso que possui mais um cara com mesmo valor de preço.\n";
                        sellBook.erase(sellBook.begin()); // Apenas removo a ordem de venda do topo e analiso a segunda
                        std::cout << "Dei Erase no meu sellBook. meu novo maior valor eh: "<< "sellBook[0].price: "<< sellBook[0].price << " sellBook[0].qty: "<< sellBook[0].qty << "\n";
                    }
                }
                else if (buyBook[0].qty < sellBook[0].qty){
                    std::cout << "Estou no caso que temos mais quantidade de venda do que compra nas ofertas do topo\n";

                    quant += buyBook[0].qty;
                    sellBook[0].qty -= buyBook[0].qty;

                    std::cout << "quant: "<< quant << "  sellBook[0].qty "<< sellBook[0].qty<< "\n";

                    if(buyBook.size() == 1 || buyBook[1].price != buyBook[0].price){
                        std::cout << "Estou no caso que nao tem mais outro cara no livro de compras com o mesmo preço que o que eu acabei de esgotar.\n";

                        END = TRUE;
                        std::cout<< "\n--------------------------------------------- \n";
                        std::cout<< "Trade, price: "<< sellBook[0].price<< ", qty: "<< quant<< "\n";
                        std::cout<< "\n--------------------------------------------- \n";

                        buyBook.erase(buyBook.begin());

                        std::cout << "Dei Erase no meu buyBook. \n";
                    }
                    else{
                        std::cout << "Estou no caso que possui mais um cara com mesmo valor de preço.\n";
                        buyBook.erase(buyBook.begin());
                        std::cout << "Dei Erase no meu sellBook. meu novo maior valor eh: "<< "sellBook[0].price: "<< sellBook[0].price << " sellBook[0].qty: "<< sellBook[0].qty << "\n";
                    }
                }
                else if (buyBook[0].qty == sellBook[0].qty){
                    std::cout<< "Ambas as ordens do topo possuem quantidades iguais\n";

                    quant = buyBook[0].qty;
                    std::cout<< "\n--------------------------------------------- \n";
                    std::cout<< "Trade, price: "<< sellBook[0].price<< ", qty: "<< quant<< "\n";
                    std::cout<< "\n--------------------------------------------- \n";

                    buyBook.erase(buyBook.begin());
                    sellBook.erase(sellBook.begin());

                    std::cout << "Dei Erase no meu sellBook. meu novo maior valor eh: "<< "sellBook[0].price: "<< sellBook[0].price << " sellBook[0].qty: "<< sellBook[0].qty << "\n";
                    std::cout << "Dei Erase no meu sellBook. meu novo maior valor eh: "<< "sellBook[0].price: "<< sellBook[0].price << " sellBook[0].qty: "<< sellBook[0].qty << "\n";
                }
            }
        }
        else
        {
            TRADING = FALSE;
        }

    }
    std::cout << "Saindo da Funcao de Trade \n";

}

void MarketBuy(Order *ptr, std::vector<Order>& sellBook)
{
    bool END;
    float quant;

    std::cout << "Entrei na funcao de MarketBuy \n";

    quant = 0; // quantidade que esta sendo trading no preço do topo do livro de vendas
    END = FALSE; // verifica se todos os caras com mesmo preço possível já foram transacionados
    while(!END){
        std::cout << "sellBook[0].price: "<< sellBook[0].price << " sellBook[0].qty: "<< sellBook[0].qty<< "\n";
        std::cout << "Ordens de compra a mercado: "<< ptr->qty <<  "\n";

         if(ptr->qty > sellBook[0].qty)
         {
             std::cout << "Estou no caso que temos mais quantidade de pedidos de compra a mercado do que venda nas ofertas do topo\n";
             quant += sellBook[0].qty; // atualizo a quantidade sendo transacionada
             ptr->qty -= sellBook[0].qty; // atualizo a quantidade na minha ordem a mercado

            std::cout << "quantidade atual: "<< quant << "  market order qty: "<< ptr->qty<< "\n";
            if(sellBook.size() == 1 || sellBook[1].price != sellBook[0].price){
                std::cout << "Estou no caso que nao tem mais outro cara no livro de vendas com o mesmo preco que o que eu acabei de esgotar.\n";

                END = TRUE;

                std::cout<< "\n--------------------------------------------- \n";
                std::cout<< "Trade, price: "<< sellBook[0].price<< ", qty: "<< quant<< "\n";
                std::cout<< "\n--------------------------------------------- \n";

                sellBook.erase(sellBook.begin());

                std::cout << "Dei Erase no meu sellBook. \n";
            }
            else
            {
                std::cout << "Estou no caso que possui mais um cara com mesmo valor de preco.\n";
                sellBook.erase(sellBook.begin()); // Apenas removo a ordem de venda do topo e analiso a segunda
                std::cout << "Dei Erase no meu sellBook. meu novo maior valor eh: "<< "sellBook[0].price: "<< sellBook[0].price << "sellBook[0].qty: "<< sellBook[0].qty << "\n";
            }
         }
         else if (ptr->qty < sellBook[0].qty){
            std::cout << "Estou no caso que temos mais quantidade de venda do que compra a mercado nas ofertas do topo\n";

            quant += ptr->qty;
            sellBook[0].qty -= ptr->qty;

            std::cout << "quant: "<< quant << " sellBook[0].qty "<< sellBook[0].qty<< "\n";
            std::cout<< "\n--------------------------------------------- \n";
            std::cout<< "Trade, price: "<< sellBook[0].price<< ", qty: "<< quant<< "\n";
            std::cout<< "\n--------------------------------------------- \n";

            free(ptr);
            END = TRUE;
         }
         else if (ptr->qty == sellBook[0].qty)
         {
            std::cout<< "Ordem do topo do livro de vendar igual a ordem de compra a mercado \n";

            quant += ptr->qty;

            std::cout<< "\n--------------------------------------------- \n";
            std::cout<< "Trade, price: "<< sellBook[0].price<< ", qty: "<< quant<< "\n";
            std::cout<< "\n--------------------------------------------- \n";

            sellBook.erase(sellBook.begin());
            std::cout << "Dei Erase no meu sellBook. meu novo maior valor eh: "<< " sellBook[0].price: "<< sellBook[0].price << " sellBook[0].qty: "<< sellBook[0].qty << "\n";
            free(ptr);
            END = TRUE;
         }
    }
}

void MarketSell(Order *ptr, std::vector<Order>& buyBook)
{
    bool END;
    float quant;

    std::cout << "Entrei na funcao de MarketSell \n";

    quant = 0; // quantidade que esta sendo trading no preço do topo do livro de vendas
    END = FALSE; // verifica se todos os caras com mesmo preço possível já foram transacionados
    while(!END){
        std::cout << "buyBook[0].price: "<< buyBook[0].price << " buyBook[0].qty: "<< buyBook[0].qty<< "\n";
        std::cout << "Ordens de venda a mercado: "<< ptr->qty <<  "\n";

         if(ptr->qty > buyBook[0].qty)
         {
             std::cout << "Estou no caso que temos mais quantidade de pedidos de compra a mercado do que venda nas ofertas do topo\n";
             quant += buyBook[0].qty; // atualizo a quantidade sendo transacionada
             ptr->qty -= buyBook[0].qty; // atualizo a quantidade na minha ordem a mercado

            std::cout << "quantidade atual: "<< quant << "  market sell order qty restante: "<< ptr->qty<< "\n";
            if(buyBook.size() == 1 || buyBook[1].price != buyBook[0].price){
                std::cout << "Estou no caso que nao tem mais outro cara no livro de compras com o mesmo preço que o que eu acabei de esgotar.\n";

                END = TRUE;

                std::cout<< "\n--------------------------------------------- \n";
                std::cout<< "Trade, price: "<< buyBook[0].price<< ", qty: "<< quant<< "\n";
                std::cout<< "\n--------------------------------------------- \n";

                buyBook.erase(buyBook.begin());

                std::cout << "Dei Erase no meu buyBook. \n";
            }
            else
            {
                std::cout << "Estou no caso que possui mais um cara com mesmo valor de preço.\n";
                buyBook.erase(buyBook.begin()); // Apenas removo a ordem de venda do topo e analiso a segunda
                std::cout << "Dei Erase no meu buyBook. meu novo maior valor eh: "<< "buyBook[0].price: "<< buyBook[0].price << "buyBook[0].qty: "<< buyBook[0].qty << "\n";
            }
         }
         else if (ptr->qty < buyBook[0].qty){
            std::cout << "Estou no caso que temos mais quantidade de compra do que venda a mercado nas ofertas do topo\n";

            quant = ptr->qty;
            buyBook[0].qty -= ptr->qty;

            std::cout << "quant: "<< quant << " buyBook[0].qty "<< buyBook[0].qty<< "\n";
            std::cout<< "\n--------------------------------------------- \n";
            std::cout<< "Trade, price: "<< buyBook[0].price<< ", qty: "<< quant<< "\n";
            std::cout<< "\n--------------------------------------------- \n";

            free(ptr);
            END = TRUE;
         }
         else if (ptr->qty == buyBook[0].qty)
         {
            std::cout<< "Ordem do topo do livro de vendar igual a ordem de compra a mercado \n";

            quant = ptr->qty;

            std::cout<< "\n--------------------------------------------- \n";
            std::cout<< "Trade, price: "<< buyBook[0].price<< ", qty: "<< quant<< "\n";
            std::cout<< "\n--------------------------------------------- \n";

            buyBook.erase(buyBook.begin());
            std::cout << "Dei Erase no meu buyBook. meu novo maior valor eh: "<< " buyBook[0].price: "<< buyBook[0].price << " buyBook[0].qty: "<< buyBook[0].qty << "\n";
            free(ptr);
            END = TRUE;
         }
    }
}

int main()
{
    // Declaração de Variáveis a serem utilizadas
    std::string type, side;
    float price, qty;
    Order *ptr; // ponteiro para um objeto da classe order
    std::vector<Order> buyBook, sellBook; //   Nossos livros de compra e venda
    /*---------------------------------------------------------------------*/

    std::cout << "Para encerrar o programa a qualquer momento, digite: exit \n" ;
    std::cout << "Matching Engine - Digite as entradas de compra ou venda: \n" ;

    std::cin >> type >> side; // Acabei de ler a primeira entrada
    while (type != "exit")
    {
        //  Separar em dois casos: Ordem Limit ou Ordem Market
        if (type == "limit")
        {
            std::cout << "\nRealizou uma ordem limit: \n";
            std::cin >> price >> qty;
            std::cout << type <<" "<< side <<" " << price <<" "<< qty << "\n\n";

            //  Proximo passo sera criar um objeto da classe ordem e adicionar esses valores ao objeto
            ptr = new Order;
            ptr->type = type;
            ptr->side = side;
            ptr->price = price;
            ptr->qty = qty;

            //  Agora vamos adicionar essa nova ordem ou no livro de compras ou no de vendas
            if (ptr->side == "buy")
            {
                AddToBuyBook(ptr, buyBook);
            }
            else
            {
                AddToSellBook(ptr, sellBook);
            }

            // Atualizar os livros para ver se é possível realizar alguma venda
            std::cout << "Vamos tentar realizar um limit Trade \n";
            Trade(buyBook, sellBook);
            std::cout << "\nEssa eh a nova configuracao do BuyBook: \n";
            for (int j = 0; j < buyBook.size(); j++)
            {
                std::cout << buyBook[j].price << " " << buyBook[j].qty << "\n";
            }
            std::cout << "\nEssa eh a nova configuracao do SellBook: \n";
            for (int j = 0; j < sellBook.size(); j++)
            {
                std::cout << sellBook[j].price << " " << sellBook[j].qty << "\n";
            }
        }
        /*--------------------------------------*/
        if (type == "market")
        {
            std::cout << "Realizou uma ordem market \n";
            std::cin >> qty;
            std::cout << type << " "<< side <<" "<< qty << "\n";


             //  Proximo passo sera criar um objeto da classe ordem e adicionar esses valores ao objeto
            ptr = new Order;
            ptr->type = type;
            ptr->side = side;
            ptr->qty = qty;

            if (ptr->side == "buy"){
                MarketBuy(ptr, sellBook);
            }
            else{
                MarketSell(ptr, buyBook);
            }
            std::cout << "\nEssa eh a nova configuracao do BuyBook: \n";
            for (int j = 0; j < buyBook.size(); j++)
            {
                std::cout << buyBook[j].price << " " << buyBook[j].qty << "\n";
            }
            std::cout << "\nEssa eh a nova configuracao do SellBook: \n";
            for (int j = 0; j < sellBook.size(); j++)
            {
                std::cout << sellBook[j].price << " " << sellBook[j].qty << "\n";
            }
        }
        /*--------------------------------------*/
        std::cout<< "\n--------------------------------------------- \n";
        std::cin >> type >> side;
    }

    return 0;
}
