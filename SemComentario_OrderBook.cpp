#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // Adicionar esta linha para resolver o erro de sort
// Definindo constantes TRUE e FALSE para facilitar leitura
#define FALSE 0
#define TRUE 1

// Classe de pedido (Order)
class Order {
public:
    std::string type;
    std::string side;
    float price;
    float qty;
    float id;
    bool isPegg;
};

// Função para adicionar ordem ao livro de compra
void AddToBuyBook(Order* ptr, std::vector<Order>& buyBook) {
    buyBook.push_back(*ptr);
    std::sort(buyBook.begin(), buyBook.end(), [](const Order& a, const Order& b) {
        return a.price > b.price || (a.price == b.price && a.id < b.id);
    });
}

// Função para adicionar ordem ao livro de venda
void AddToSellBook(Order* ptr, std::vector<Order>& sellBook) {
    sellBook.push_back(*ptr);
    std::sort(sellBook.begin(), sellBook.end(), [](const Order& a, const Order& b) {
        return a.price < b.price || (a.price == b.price && a.id < b.id);
    });
}

// Função para executar transações (trade) entre livros de compra e venda
void Trade(std::vector<Order>& buyBook, std::vector<Order>& sellBook) {
    while (!sellBook.empty() && !buyBook.empty()) {
        if (buyBook[0].price >= sellBook[0].price) {
            float quant = std::min(buyBook[0].qty, sellBook[0].qty);
            std::cout << "Trade, price: " << sellBook[0].price << ", qty: " << quant << "\n";

            buyBook[0].qty -= quant;
            sellBook[0].qty -= quant;

            if (buyBook[0].qty == 0) buyBook.erase(buyBook.begin());
            if (sellBook[0].qty == 0) sellBook.erase(sellBook.begin());
        } else {
            break;
        }
    }
}

// Função para uma ordem de compra a mercado
void MarketBuy(Order* ptr, std::vector<Order>& sellBook) {
    while (ptr->qty != 0 && !sellBook.empty()) {
        float quant = std::min(ptr->qty, sellBook[0].qty);
        std::cout << "Trade, price: " << sellBook[0].price << ", qty: " << quant << "\n";
        ptr->qty -= quant;
        sellBook[0].qty -= quant;

        if (sellBook[0].qty == 0) sellBook.erase(sellBook.begin());
    }
}

// Função para uma ordem de venda a mercado
void MarketSell(Order* ptr, std::vector<Order>& buyBook) {
    while (ptr->qty != 0 && !buyBook.empty()) {
        float quant = std::min(ptr->qty, buyBook[0].qty);
        std::cout << "Trade, price: " << buyBook[0].price << ", qty: " << quant << "\n";
        ptr->qty -= quant;
        buyBook[0].qty -= quant;

        if (buyBook[0].qty == 0) buyBook.erase(buyBook.begin());
    }
}

// Função para exibir o livro de ordens
void ShowBooks(const std::vector<Order>& buyBook, const std::vector<Order>& sellBook) {
    std::cout << "\nORDENS DE COMPRA: \n";
    for (const auto& order : buyBook) {
        std::cout << order.qty << " @ " << order.price << "  Identificador: " << order.id << "\n";
    }

    std::cout << "\nORDENS DE VENDA: \n";
    for (const auto& order : sellBook) {
        std::cout << order.qty << " @ " << order.price << "  Identificador: " << order.id << "\n";
    }
}

// Função para cancelar uma ordem
bool CancelOrder(float identificador, std::vector<Order>& sellBook, std::vector<Order>& buyBook, std::string& sideNew, bool& isPegg) {
    for (auto it = sellBook.begin(); it != sellBook.end(); ++it) {
        if (it->id == identificador) {
            isPegg = it->isPegg;
            sideNew = "sell";
            sellBook.erase(it);
            return TRUE;
        }
    }
    for (auto it = buyBook.begin(); it != buyBook.end(); ++it) {
        if (it->id == identificador) {
            isPegg = it->isPegg;
            sideNew = "buy";
            buyBook.erase(it);
            return TRUE;
        }
    }
    return FALSE;
}

// Função para alterar uma ordem
void ChangeOrder(float id, std::vector<Order>& sellBook, std::vector<Order>& buyBook, std::string& sideNew, float price, float qty) {
    bool isPegg = FALSE;
    CancelOrder(id, sellBook, buyBook, sideNew, isPegg);
    Order newOrder = {sideNew, sideNew, price, qty, id, isPegg};
    if (sideNew == "buy") AddToBuyBook(&newOrder, buyBook);
    else AddToSellBook(&newOrder, sellBook);
}

// Função para atualizar ordens peggadas
void updatePegg(std::vector<Order>& sellBook, std::vector<Order>& buyBook) {
    for (auto& order : sellBook) {
        if (order.isPegg) {
            ChangeOrder(order.id, sellBook, buyBook, order.side, sellBook[0].price, order.qty);
        }
    }
    for (auto& order : buyBook) {
        if (order.isPegg) {
            ChangeOrder(order.id, sellBook, buyBook, order.side, buyBook[0].price, order.qty);
        }
    }
}

// Função para mostrar a ajuda
void ShowHelp() {
    std::cout << "Comandos disponíveis:\n";
    std::cout << "1. add order [tipo] [lado] [preço] [quantidade] - Adicionar ordem\n";
    std::cout << "2. market buy [quantidade] - Comprar a mercado\n";
    std::cout << "3. market sell [quantidade] - Vender a mercado\n";
    std::cout << "4. cancel [identificador] - Cancelar ordem\n";
    std::cout << "5. show books - Mostrar o livro de ordens\n";
    std::cout << "6. exit - Sair do programa\n";
}

// Função principal
int main() {
    std::string type, side, sideNew;
    float price, qty, id;
    std::vector<Order> buyBook, sellBook;
    bool running = TRUE;

    std::cout << "Digite 'help' para ver os comandos disponíveis.\n";

    while (running) {
        std::string command;
        std::cout << "\nDigite o comando: ";
        std::cin >> command;

        if (command == "help") {
            ShowHelp();
        } else if (command == "add") {
            std::cin >> type >> side >> price >> qty;
            id = buyBook.size() + sellBook.size() + 1; // Gera um ID simples
            Order newOrder = {type, side, price, qty, id, FALSE};
            if (side == "buy") AddToBuyBook(&newOrder, buyBook);
            else AddToSellBook(&newOrder, sellBook);
        } else if (command == "market") {
            std::cin >> side >> qty;
            Order marketOrder = {"market", side, 0, qty, 0, FALSE};
            if (side == "buy") MarketBuy(&marketOrder, sellBook);
            else MarketSell(&marketOrder, buyBook);
        } else if (command == "cancel") {
            std::cin >> id;
            CancelOrder(id, sellBook, buyBook, sideNew, running);
        } else if (command == "show") {
            ShowBooks(buyBook, sellBook);
        } else if (command == "exit") {
            running = FALSE;
        } else {
            std::cout << "Comando não reconhecido. Digite 'help' para ajuda.\n";
        }
    }

    return 0;
}
