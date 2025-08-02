#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;



struct Option {
    string name;
    double strike;
    bool isCall;
    
    Option(string n, double s, bool call) : name(n), strike(s), isCall(call) {}
};

struct Quote {
    double bid;
    double ask;
    
    Quote(double b, double a) : bid(b), ask(a) {}
};

struct Position {
    int contracts;
    double totalCost;
    
    Position() : contracts(0), totalCost(0) {}
};

class SimpleMarketMaker {
private:
    double stockPrice;
    double cash;
    vector<Option> options;
    vector<Position> positions;
    
public:
    SimpleMarketMaker() {
        stockPrice = 100.0;
        cash = 10000.0;
        
        options.push_back(Option("CALL_95", 95, true));
        options.push_back(Option("CALL_100", 100, true));
        options.push_back(Option("CALL_105", 105, true));
        options.push_back(Option("PUT_95", 95, false));
        options.push_back(Option("PUT_100", 100, false));
        options.push_back(Option("PUT_105", 105, false));
        
        positions.resize(options.size());
    }
    
    double getOptionValue(const Option& opt) {
        if (opt.isCall) {
            return max(0.0, stockPrice - opt.strike);
        } else {
            return max(0.0, opt.strike - stockPrice);
        }
    }
    
    Quote makeQuote(int optionIndex) {
        double fairValue = getOptionValue(options[optionIndex]);
        
        double spread = 2.0;
        double bid = max(0.1, fairValue - spread/2);
        double ask = fairValue + spread/2;
        
        if (positions[optionIndex].contracts > 5) {
            bid -= 1.0;
            ask -= 1.0;
        }
        else if (positions[optionIndex].contracts < -5) {
            bid += 1.0;
            ask += 1.0;
        }
        
        return Quote(bid, ask);
    }
    
    bool handleOrder(int optionIndex, bool customerBuying, int quantity, double customerPrice) {
        Quote ourQuote = makeQuote(optionIndex);
        
        cout << options[optionIndex].name << " - Our quote: $" << ourQuote.bid 
             << " / $" << ourQuote.ask << endl;
        
        if (customerBuying) {
            if (customerPrice >= ourQuote.ask) {
                cout << "SOLD " << quantity << " " << options[optionIndex].name 
                     << " at $" << ourQuote.ask << endl;
                
                positions[optionIndex].contracts -= quantity;
                positions[optionIndex].totalCost -= quantity * ourQuote.ask;
                cash += quantity * ourQuote.ask;
                return true;
            }
        } else {
            if (customerPrice <= ourQuote.bid) {
                cout << "BOUGHT " << quantity << " " << options[optionIndex].name 
                     << " at $" << ourQuote.bid << endl;
                
                positions[optionIndex].contracts += quantity;
                positions[optionIndex].totalCost += quantity * ourQuote.bid;
                cash -= quantity * ourQuote.bid;
                return true;
            }
        }
        
        cout << "No trade - customer price $" << customerPrice 
             << (customerBuying ? " too low" : " too high") << endl;
        return false;
    }
    
    void updateStockPrice() {
        double change = (rand() % 400 - 200) / 100.0;
        stockPrice = max(1.0, stockPrice + change);
    }
    
    void printStatus() {
        cout << "\n=== MARKET MAKER STATUS ===" << endl;
        cout << "Stock Price: $" << fixed << setprecision(2) << stockPrice << endl;
        cout << "Cash: $" << cash << endl;
        cout << "\nPositions:" << endl;
        
        double totalValue = 0;
        for (int i = 0; i < options.size(); i++) {
            if (positions[i].contracts != 0) {
                double currentValue = positions[i].contracts * getOptionValue(options[i]);
                double profit = currentValue + positions[i].totalCost;
                totalValue += currentValue;
                
                cout << options[i].name << ": " << positions[i].contracts 
                     << " contracts, Value: $" << currentValue 
                     << ", P&L: $" << profit << endl;
            }
        }
        
        cout << "Total Portfolio Value: $" << (cash + totalValue) << endl;
        cout << "================================\n" << endl;
    }
    
    double getStockPrice() { return stockPrice; }
    int getNumOptions() { return options.size(); }
    string getOptionName(int i) { return options[i].name; }
};

void generateCustomerOrder(SimpleMarketMaker& mm) {
    int optionIndex = rand() % mm.getNumOptions();
    bool customerBuying = (rand() % 2) == 0;
    int quantity = 1 + (rand() % 3);
    
    double basePrice = 5.0 + (rand() % 1000) / 100.0;
    
    cout << "\nCustomer wants to " << (customerBuying ? "BUY" : "SELL") 
         << " " << quantity << " " << mm.getOptionName(optionIndex) 
         << " at $" << basePrice << endl;
    
    mm.handleOrder(optionIndex, customerBuying, quantity, basePrice);
}

int main() {
    srand(time(0));
    
    SimpleMarketMaker marketMaker;
    
    cout << "=== SIMPLE OPTIONS MARKET MAKING SIMULATOR ===" << endl;
    cout << "We'll make markets in options and see how we do!" << endl;
    
    for (int day = 1; day <= 10; day++) {
        cout << "\n--- DAY " << day << " ---" << endl;
        
        marketMaker.updateStockPrice();
        cout << "Stock moved to: $" << marketMaker.getStockPrice() << endl;
        
        for (int order = 0; order < 3; order++) {
            generateCustomerOrder(marketMaker);
        }
        
        if (day % 3 == 0) {
            marketMaker.printStatus();
        }
    }
    
    cout << "\nFINAL RESULTS:" << endl;
    marketMaker.printStatus();
    

    cout << "Simulation complete! This shows the basics of how market makers:" << endl;
    cout << "1. Quote bid/ask prices with a spread to make profit" << endl;
    cout << "2. Adjust prices based on inventory (risk management)" << endl;
    cout << "3. Make money from the spread but take risk on price moves" << endl;
    
    return 0;
}