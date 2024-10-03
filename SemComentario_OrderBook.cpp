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

    if(n == 0){
        buyBook.insert(buyBook.begin(), *ptr);
        return;
    }

    buyBook.insert(buyBook.begin(), *ptr);

    // Agora vamos colocar em ordem, priorizando o mais antigo na frente
    i = 0;
    END = FALSE;
    while(!END && (i+1)!=n )
    {
        END = TRUE;

        if(buyBook[i].price <= buyBook[i+1].price)
        {
            aux = buyBook[i];
            buyBook[i] = buyBook[i+1];
            buyBook[i+1] = aux;
            END = FALSE;
        }
        i++;
    }
}

void AddToSellBook(Order *ptr, std::vector<Order>& sellBook)
{
    bool END;
    int i, j;
    Order aux;
    int n = sellBook.size();
    /*---------------------------------------------------------------------------*/
    if(n == 0){
        sellBook.insert(sellBook.begin(), *ptr);
        return;
    }
    sellBook.insert(sellBook.begin(), *ptr);

    /*---------------------------------------------------------------------------*/
    // Agora vamos colocar em ordem, priorizando o mais antigo na frente
    i = 0;
    END = FALSE;
    while(!END && (i+1)!= n)
    {
        END = TRUE;
        if(sellBook[i].price >= sellBook[i+1].price)
        {
            aux = sellBook[i];
            sellBook[i] = sellBook[i+1];
            sellBook[i+1] = aux;
            END = FALSE;
        }
        i++;
    }
}

void Trade(std::vector<Order>& buyBook, std::vector<Order>& sellBook)
{
    int nBuy = buyBook.size();
    int nSell = sellBook.size();
    bool TRADING = TRUE;
    bool END;
    float quant;
    /*---------------------------------------------------------------------------*/
    while( nSell != 0 && nBuy != 0 && TRADING)
    {
        if(buyBook[0].price >= sellBook[0].price)   //  Se a maior oferta de compra for maior ou igual que a menor oferta de venda, realizamos um trade
        {
            quant = 0; // quantidade que esta sendo trading no preço do topo do livro de vendas
            END = FALSE; // verifica se todos os caras com mesmo preço possível já foram transacionados

            while(!END){

                if(buyBook[0].qty > sellBook[0].qty)
                {

                    quant += sellBook[0].qty; // atualizo a quantidade sendo transacionada
                    buyBook[0].qty -= sellBook[0].qty; // atualizo a quantidade no meu buybook[0]

                    if(sellBook.size() == 1 || sellBook[1].price != sellBook[0].price){

                        END = TRUE;
                        std::cout<< "Trade, price: "<< sellBook[0].price<< ", qty: "<< quant<< "\n";

                        sellBook.erase(sellBook.begin());
                    }
                    else{
                        sellBook.erase(sellBook.begin()); // Apenas removo a ordem de venda do topo e analiso a segunda
                    }
                }
                else if (buyBook[0].qty < sellBook[0].qty){
                    quant += buyBook[0].qty;
                    sellBook[0].qty -= buyBook[0].qty;

                    if(buyBook.size() == 1 || buyBook[1].price != buyBook[0].price){

                        END = TRUE;
                        std::cout<< "Trade, price: "<< sellBook[0].price<< ", qty: "<< quant<< "\n";

                        buyBook.erase(buyBook.begin());;
                    }
                    else{
                        buyBook.erase(buyBook.begin());
                    }
                }
                /*else if (buyBook[0].qty == sellBook[0].qty){

                }*/
            }
        }
        else
        {
            TRADING = FALSE;
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
            std::cin >> price >> qty;

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
            std::cin >> qty;
            //std::cout << type << side << qty << "\n";
        }
        /*--------------------------------------*/
        std::cin >> type >> side;
    }

    return 0;
}
