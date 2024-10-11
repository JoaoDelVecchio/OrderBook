#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>

class Order {
public:
    std::string type;
    std::string side;
    float price;
    float qty;
    float id;

    // Construtor da ordem
    Order(const std::string& t, const std::string& s, float p, float q, float i)
        : type(t), side(s), price(p), qty(q), id(i) {}

    // Comparação para organizar as ordens (para priority_queue)
    bool operator<(const Order& o) const {
        if (side == "buy") {
            return price < o.price;  // Compra, maior preço primeiro
        } else {
            return price > o.price;  // Venda, menor preço primeiro
        }
    }
};

// Função para criar ordens de compra e venda
void addOrder(const std::shared_ptr<Order>& order,
              std::priority_queue<Order>& book,
              std::unordered_map<float, std::shared_ptr<Order>>& orderMap) {
    orderMap[order->id] = order;
    book.push(*order); // Insere na priority queue
}

void processTrades(std::priority_queue<Order>& buyBook, std::priority_queue<Order>& sellBook) {
    while (!buyBook.empty() && !sellBook.empty() && buyBook.top().price >= sellBook.top().price) {
        auto buyOrder = buyBook.top();
        auto sellOrder = sellBook.top();

        float tradeQty = std::min(buyOrder.qty, sellOrder.qty);
        std::cout << "Trade, price: " << sellOrder.price << ", qty: " << tradeQty << std::endl;

        // Ajusta as quantidades
        buyBook.pop();
        sellBook.pop();

        if (buyOrder.qty > tradeQty) {
            buyOrder.qty -= tradeQty;
            buyBook.push(buyOrder); // Reinsere com quantidade ajustada
        }

        if (sellOrder.qty > tradeQty) {
            sellOrder.qty -= tradeQty;
            sellBook.push(sellOrder); // Reinsere com quantidade ajustada
        }
    }
}

void cancelOrder(float id, std::priority_queue<Order>& book, std::unordered_map<float, std::shared_ptr<Order>>& orderMap) {
    if (orderMap.find(id) != orderMap.end()) {
        std::cout << "Order " << id << " cancelled.\n";
        orderMap.erase(id);
    } else {
        std::cout << "Order " << id << " not found.\n";
    }
}

int main() {
    std::priority_queue<Order> buyBook, sellBook;
    std::unordered_map<float, std::shared_ptr<Order>> orderMap;

    float idCounter = 1;
    std::string type, side;

    std::cout << "Para encerrar o programa a qualquer momento, digite: exit \n";
    std::cout << "Para visualizar o livro de ordens, digite: ver livro \n";
    std::cin >> type >> side;

    while (type != "exit") {
        if (type == "limit") {
            float price, qty;
            std::cin >> price >> qty;
            auto order = std::make_shared<Order>(type, side, price, qty, idCounter++);
            if (side == "buy") {
                addOrder(order, buyBook, orderMap);
            } else {
                addOrder(order, sellBook, orderMap);
            }
            processTrades(buyBook, sellBook);
        }
        else if (type == "market") {
            float qty;
            std::cin >> qty;
            if (side == "buy" && !sellBook.empty()) {
                while (qty > 0 && !sellBook.empty()) {
                    auto sellOrder = sellBook.top();
                    sellBook.pop();
                    float tradeQty = std::min(qty, sellOrder.qty);
                    std::cout << "Trade, price: " << sellOrder.price << ", qty: " << tradeQty << std::endl;
                    qty -= tradeQty;
                    if (sellOrder.qty > tradeQty) {
                        sellOrder.qty -= tradeQty;
                        sellBook.push(sellOrder);
                    }
                }
            } else if (side == "sell" && !buyBook.empty()) {
                while (qty > 0 && !buyBook.empty()) {
                    auto buyOrder = buyBook.top();
                    buyBook.pop();
                    float tradeQty = std::min(qty, buyOrder.qty);
                    std::cout << "Trade, price: " << buyOrder.price << ", qty: " << tradeQty << std::endl;
                    qty -= tradeQty;
                    if (buyOrder.qty > tradeQty) {
                        buyOrder.qty -= tradeQty;
                        buyBook.push(buyOrder);
                    }
                }
            }
        }
        else if (type == "cancel") {
            float orderId;
            std::cin >> orderId;
            cancelOrder(orderId, (side == "buy" ? buyBook : sellBook), orderMap);
        }

        std::cout << "Matching Engine - Digite próximo comando: \n";
        std::cin >> type >> side;
    }
    return 0;
}
