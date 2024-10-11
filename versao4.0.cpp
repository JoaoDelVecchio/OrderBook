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
#include <cstdlib>

// aqui criar dois objetos. o primeiro serao objeto limit e o segundo o objeto market
// ambos os objetos vao possuir uma classe anterior que vai ser, sei la

class Order
{
public:
    std::string type;
    std::string side;
    float price;
    float qty;
    float id;
    bool isPegg;
};

void AddToBuyBook(Order *ptr, std::vector<Order> &buyBook)
{

    bool END;
    int i, j;
    Order aux;
    int n = buyBook.size();

    if (n == 0)
    {
        buyBook.insert(buyBook.begin(), *ptr);
        return;
    }

    buyBook.insert(buyBook.begin(), *ptr);

    // Agora vamos colocar em ordem, priorizando o mais antigo na frente
    i = 0;
    END = FALSE;
    while (!END && (i + 1) != buyBook.size())
    {
        END = TRUE;

        if (buyBook[i].price < buyBook[i + 1].price || (buyBook[i].price == buyBook[i + 1].price && buyBook[i].id > buyBook[i + 1].id))
        {
            aux = buyBook[i];
            buyBook[i] = buyBook[i + 1];
            buyBook[i + 1] = aux;
            END = FALSE;
        }
        i++;
    }
}

void AddToSellBook(Order *ptr, std::vector<Order> &sellBook)
{
    bool END;
    int i, j;
    Order aux;
    int n = sellBook.size();
    /*---------------------------------------------------------------------------*/
    if (n == 0)
    {
        sellBook.insert(sellBook.begin(), *ptr);
        return;
    }
    sellBook.insert(sellBook.begin(), *ptr);

    /*---------------------------------------------------------------------------*/
    // Agora vamos colocar em ordem, priorizando o mais antigo na frente
    i = 0;
    END = FALSE;
    while (END == FALSE && (i + 1) != sellBook.size())
    {
        END = TRUE;
        if (sellBook[i].price > sellBook[i + 1].price || ( sellBook[i].price == sellBook[i + 1].price && sellBook[i].id > sellBook[i + 1].id) )
        {
            aux = sellBook[i];
            sellBook[i] = sellBook[i + 1];
            sellBook[i + 1] = aux;
            END = FALSE;
        }
        i++;
    }
}

void Trade(std::vector<Order> &buyBook, std::vector<Order> &sellBook)
{
    bool TRADING = TRUE;
    bool END;
    float quant;
    /*---------------------------------------------------------------------------*/
    while (sellBook.size() != 0 && buyBook.size() != 0 && TRADING)
    {
        if (buyBook[0].price >= sellBook[0].price) //  Se a maior oferta de compra for maior ou igual que a menor oferta de venda, realizamos um trade
        {
            quant = 0;   // quantidade que esta sendo trading no pre�o do topo do livro de vendas
            END = FALSE; // verifica se todos os caras com mesmo pre�o poss�vel j� foram transacionados

            while (!END)
            {
                if (buyBook[0].qty > sellBook[0].qty)
                {
                    quant += sellBook[0].qty;          // atualizo a quantidade sendo transacionada
                    buyBook[0].qty -= sellBook[0].qty; // atualizo a quantidade no meu buybook[0]

                    if (sellBook.size() == 1 || sellBook[1].price != sellBook[0].price)
                    {
                        END = TRUE;
                        std::cout << "\nTrade, price: " << sellBook[0].price << ", qty: " << quant << "\n";
                        sellBook.erase(sellBook.begin());
                    }
                    else
                    {
                        sellBook.erase(sellBook.begin()); // Apenas removo a ordem de venda do topo e analiso a segunda
                    }
                }
                else if (buyBook[0].qty < sellBook[0].qty)
                {
                    quant += buyBook[0].qty;
                    sellBook[0].qty -= buyBook[0].qty;

                    if (buyBook.size() == 1 || buyBook[1].price != buyBook[0].price)
                    {
                        END = TRUE;
                        std::cout << "\nTrade, price: " << sellBook[0].price << ", qty: " << quant << "\n";
                        buyBook.erase(buyBook.begin());
                    }
                    else
                    {
                        buyBook.erase(buyBook.begin());
                    }
                }
                else if (buyBook[0].qty == sellBook[0].qty)
                {
                    quant = buyBook[0].qty;
                    std::cout << "\nTrade, price: " << sellBook[0].price << ", qty: " << quant << "\n";
                    buyBook.erase(buyBook.begin());
                    sellBook.erase(sellBook.begin());
                }
            }
        }
        else
        {
            TRADING = FALSE;
        }
    }
}

void MarketBuy(Order *ptr, std::vector<Order> &sellBook)
{
    float quant;

    quant = 0; // quantidade que esta sendo trading no pre�o do topo do livro de vendas
    while (ptr->qty != 0 && sellBook.size() != 0)
    {

        if (ptr->qty > sellBook[0].qty)
        {
            quant += sellBook[0].qty;    // atualizo a quantidade sendo transacionada
            ptr->qty -= sellBook[0].qty; // atualizo a quantidade na minha ordem a mercado

            if (sellBook.size() == 1 || sellBook[1].price != sellBook[0].price)
            {

                std::cout << "\nTrade, price: " << sellBook[0].price << ", qty: " << quant << "\n";
                quant = 0;
                sellBook.erase(sellBook.begin());

            }
            else
            {
                sellBook.erase(sellBook.begin()); // Apenas removo a ordem de venda do topo e analiso a segunda
            }
        }
        else if (ptr->qty < sellBook[0].qty)
        {
            quant += ptr->qty;
            sellBook[0].qty -= ptr->qty;
            ptr->qty = 0;
            std::cout << "\nTrade, price: " << sellBook[0].price << ", qty: " << quant << "\n";
            quant = 0;
            free(ptr);
        }
        else if (ptr->qty == sellBook[0].qty)
        {
            quant += ptr->qty;
            std::cout << "\nTrade, price: " << sellBook[0].price << ", qty: " << quant << "\n";
            sellBook.erase(sellBook.begin());
            free(ptr);
        }
    }
}

void MarketSell(Order *ptr, std::vector<Order> &buyBook)
{
    float quant;

    quant = 0; // quantidade que esta sendo trading no pre�o do topo do livro de vendas
    while (ptr->qty != 0 && buyBook.size() != 0)
    {

        if (ptr->qty > buyBook[0].qty)
        {
            quant += buyBook[0].qty;    // atualizo a quantidade sendo transacionada
            ptr->qty -= buyBook[0].qty; // atualizo a quantidade na minha ordem a mercado
            if (buyBook.size() == 1 || buyBook[1].price != buyBook[0].price)
            {
                std::cout << "\nTrade, price: " << buyBook[0].price << ", qty: " << quant << "\n";
                quant = 0;
                buyBook.erase(buyBook.begin());
            }
            else
            {
                buyBook.erase(buyBook.begin()); // Apenas removo a ordem de venda do topo e analiso a segunda
            }
        }
        else if (ptr->qty < buyBook[0].qty)
        {
            quant += ptr->qty;
            buyBook[0].qty -= ptr->qty;
            ptr->qty = 0;
            std::cout << "\nTrade, price: " << buyBook[0].price << ", qty: " << quant << "\n";
            quant = 0;
            free(ptr);
        }
        else if (ptr->qty == buyBook[0].qty)
        {
            quant += ptr->qty;
            ptr->qty = 0;
            std::cout << "\nTrade, price: " << buyBook[0].price << ", qty: " << quant << "\n";
            buyBook.erase(buyBook.begin());
            free(ptr);
        }
    }
}

void Livro(std::vector<Order> sellBook, std::vector<Order> buyBook)
{
    std::cout << "\n_________________________\n";
    std::cout << "ORDENS DE COMPRA: \n";
    for (int j = 0; j < buyBook.size(); j++)
    {
        std::cout << buyBook[j].qty << " @ "<< buyBook[j].price<< "  Identificador: "<< buyBook[j].id << "\n";
    }
    std::cout << "_________________________\n";


    std::cout << "\n_________________________\n";
    std::cout << "ORDENS DE VENDA: \n";
    for (int j = 0; j < sellBook.size(); j++)
    {
        std::cout << sellBook[j].qty << " @ "<< sellBook[j].price<< "  Identificador: "<< sellBook[j].id << "\n";
    }
    std::cout << "_________________________\n";
}

bool CancelOrder(float identificador, std::vector<Order> &sellBook, std::vector<Order> &buyBook, std::string& sideNew, bool& isPegg)
{
    bool REMOVEU = FALSE;
    int i;
    // Procura no sellBook
    for (i=0; i<sellBook.size() && !REMOVEU ; i++)
    {
        if (sellBook[i].id == identificador)
        {
            sellBook.erase(sellBook.begin() + i);
            REMOVEU = TRUE;
            sideNew = "sell";
            isPegg = sellBook[i].isPegg;
        }
    }
    // Procura no buyBook
    for (i=0; i<buyBook.size() && !REMOVEU ; i++)
    {
        if (buyBook[i].id == identificador)
        {
            buyBook.erase(buyBook.begin() + i);
            REMOVEU = TRUE;
            sideNew = "buy";
            isPegg = buyBook[i].isPegg;
        }
    }
    // Avisa caso essa ordem nao exista
    return REMOVEU;
}

void ChangeOrder(float& id, std::vector<Order> &sellBook, std::vector<Order> &buyBook, std::string& sideNew, float price, float qty)
{
    bool isPegg = FALSE;
    Order *ptr;

    CancelOrder(id, sellBook, buyBook, sideNew, isPegg);
    ptr = new Order;
    ptr->side = sideNew;
    ptr->price = price;
    ptr->qty = qty;
    ptr->id = id;
    ptr->isPegg = isPegg;

    if (ptr->side == "buy")
    {
        AddToBuyBook(ptr, buyBook);
    }
    else
    {
        AddToSellBook(ptr, sellBook);
    }
}

void updatePegg(std::vector<Order> &sellBook, std::vector<Order> &buyBook)
{
    int i;
    std::string sideNew;
    // Procura no sellBook
    for (i=0; i<sellBook.size(); i++)
    {
        if (sellBook[i].isPegg == TRUE)
        {
            ChangeOrder(sellBook[i].id, sellBook, buyBook, sideNew , sellBook[0].price, sellBook[i].qty);
        }
    }
    // Procura no buyBook
    for (i=0; i<buyBook.size(); i++)
    {
        if (buyBook[i].isPegg == TRUE)
        {
            ChangeOrder(buyBook[i].id, sellBook, buyBook, sideNew , buyBook[0].price, buyBook[i].qty);
        }
    }
}

int main()
{
    // Declara��o de Vari�veis a serem utilizadas
    std::string type, side, sideNew;
    float price, qty, identificador, id;
    Order *ptr;                           // ponteiro para um objeto da classe order
    std::vector<Order> buyBook, sellBook; //   Nossos livros de compra e venda
    bool REMOVEU, isPegg;
    /*---------------------------------------------------------------------*/
    identificador = 1;
    std::cout << "Para encerrar o programa a qualquer momento, digite: exit \n\n";
    std::cout << "Para visualizar o livro de ordens a qualquer momento, digite: print book \n\n";
    std::cout << "Para alterar algum valor, digite: change identificador new_Price new_Quantity \n\n";
    std::cout << "Matching Engine - Digite algum comando ou as entradas de compra e venda: \n\n";

    std::cin >> type; // Acabei de ler a primeira entrada
    while (type != "exit")
    {
        if (type == "print") // print book
        {
            std::cin >> side;
            Livro(sellBook, buyBook);
        }

        if (type == "limit") // limit side price qty OU limit bid buy price qty OU limit offer sell price qty
        {
            std::cin >> side;
            if (side == "bid" || side == "offer"){
                std::cin >> side;
            }

            std::cin >> price >> qty;
            //  Proximo passo sera criar um objeto da classe ordem e adicionar esses valores ao objeto
            identificador += 1;
            ptr = new Order;
            ptr->type = type;
            ptr->side = side;
            ptr->price = price;
            ptr->qty = qty;
            ptr->id = identificador;
            ptr->isPegg = FALSE;

            //  Agora vamos adicionar essa nova ordem ou no livro de compras ou no de vendas
            if (ptr->side == "buy")
            {
                AddToBuyBook(ptr, buyBook);
                std::cout << "Order Created: buy "<< ptr->qty<< " @ "<< ptr->price<< "  identificador: "<< ptr->id<< "\n";

            }
            else
            {
                AddToSellBook(ptr, sellBook);
                std::cout << "Order Created: sell "<< ptr->qty<< " @ "<< ptr->price<< "  identificador: "<< ptr->id<< "\n";
            }



            // Atualizar os livros para ver se � poss�vel realizar alguma venda
            updatePegg(sellBook, buyBook);
            Trade(buyBook, sellBook);
        }
        /*--------------------------------------*/
        if (type == "market")
        {
            std::cin >> side;
            std::cin >> qty;
            //  Proximo passo sera criar um objeto da classe ordem e adicionar esses valores ao objeto
            ptr = new Order;
            ptr->type = type;
            ptr->side = side;
            ptr->qty = qty;

            if (ptr->side == "buy")
            {
                MarketBuy(ptr, sellBook);
            }
            else
            {
                MarketSell(ptr, buyBook);
            }
        }
        /*--------------------------------------*/
        if (type == "cancel") // cancel order identificador
        {
            std::cin >> side; // vai receber a string order
            std::cin >> qty; // nesse caso qty sera o ID da ordem.
            REMOVEU = CancelOrder(qty ,sellBook, buyBook, sideNew, isPegg);
            if (!REMOVEU)
            {
                std::cout << "Order don't exist.\n";
            }
            else
            {
                std::cout << "Order cancelled.\n";
            }
        }
        /*--------------------------------------*/
        // Para alterar a ordem: Change identificador newPrice newQuantity
        if (type == "change")
        {
            std::cin >> id;
            std::cin >> price >> qty;
            ChangeOrder(id, sellBook, buyBook, sideNew, price, qty);
            updatePegg(sellBook, buyBook);

            Trade(buyBook, sellBook);
        }
        /*--------------------------------------*/
        if (type == "peg") // peg bid buy qty OU peg offer sell qty
        {
            std::cin >> side;
            std::cin >> side;
            std::cin >> qty;

            if (side == "buy")
            {
                price = buyBook[0].price;
                identificador += 1;

                ptr = new Order;
                ptr->side = side;
                ptr->price = price;
                ptr->qty = qty;
                ptr->id = identificador;
                ptr->isPegg = TRUE;
                AddToBuyBook(ptr, buyBook);
            }
            if (side == "sell")
            {
                price = sellBook[0].price;
                identificador += 1;

                ptr = new Order;
                ptr->side = side;
                ptr->price = price;
                ptr->qty = qty;
                ptr->id = identificador;
                ptr->isPegg = TRUE;
                AddToSellBook(ptr, sellBook);
            }
        }
        /*--------------------------------------*/
        std::cout << "______________________________________________________________________________________________\n\n";
        std::cout << "Matching Engine - Type the next order: \n";
        std::cin >> type;
    }

    return 0;
}
