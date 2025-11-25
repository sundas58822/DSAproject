#include <iostream>
#include <queue>
#include <stack>
using namespace std;

class Card {
public:
    string name;
    int power;

    Card(string n = "", int p = 0) {
        name = n;
        power = p;
    }
};

class Player {
public:
    string name;
    queue<Card> deck;
    stack<Card> discardPile;

    Player(string n = "") {
        name = n;
    }

    Card drawCard() {
        if (!deck.empty()) {
            Card top = deck.front();
            deck.pop();
            return top;
        } else {
            return Card("Empty", 0);
        }
    }

    void addToDiscard(Card c) {
        discardPile.push(c);
    }

    bool isDeckEmpty() {
        return deck.empty();
    }

    int getScore() {
        return discardPile.size();
    }
};

int main() {
    Player p("Arfa");

    // Add sample cards
    p.deck.push(Card("Dragon", 10));
    p.deck.push(Card("Elf", 6));

    cout << p.name << " draws a card...\n";
    Card c = p.drawCard();
    cout << "Card Drawn: " << c.name << " (Power " << c.power << ")\n";

    p.addToDiscard(c);
    cout << "Card added to discard pile.\n";

    cout << "Is deck empty? " << (p.isDeckEmpty() ? "Yes" : "No") << endl;
    cout << "Discard pile size: " << p.getScore() << endl;

    return 0;
}
