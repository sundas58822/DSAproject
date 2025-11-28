#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <ctime>
#include <string>
#include <cstdlib>

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
            Card c = deck.front();
            deck.pop();
            return c;
        }
        return Card("Empty", 0);
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

class Game {
public:
    vector<Card> allCards;
    Player player1, player2;

    void setup() {
        cout << "Enter Player 1 Name: ";
        cin >> player1.name;

        cout << "Enter Player 2 Name: ";
        cin >> player2.name;

        createCards();
        shuffleAndDistribute();
    }

    void createCards() {
        allCards.push_back(Card("Dragon", 10));
        allCards.push_back(Card("Wizard", 8));
        allCards.push_back(Card("Knight", 7));
        allCards.push_back(Card("Elf", 6));
        allCards.push_back(Card("Goblin", 4));
        allCards.push_back(Card("Giant", 9));
        allCards.push_back(Card("Vampire", 5));
        allCards.push_back(Card("Wolf", 3));
        allCards.push_back(Card("Zombie", 2));
        allCards.push_back(Card("Fairy", 1));
    }

    void shuffleAndDistribute() {
        srand(time(0));

        for (int i = 0; i < allCards.size(); i++) {
            int j = rand() % allCards.size();
            swap(allCards[i], allCards[j]);
        }

        for (int i = 0; i < allCards.size(); i++) {
            if (i % 2 == 0)
                player1.deck.push(allCards[i]);
            else
                player2.deck.push(allCards[i]);
        }
    }

    void startGame() {
        cout << "\n--- GAME STARTS ---\n";

        while (!player1.isDeckEmpty() && !player2.isDeckEmpty()) {
            Card c1 = player1.drawCard();
            Card c2 = player2.drawCard();

            cout << player1.name << " drew " << c1.name
                 << " (Power: " << c1.power << ")\n";

            cout << player2.name << " drew " << c2.name
                 << " (Power: " << c2.power << ")\n";

            if (c1.power > c2.power) {
                cout << "Round Winner: " << player1.name << endl;
                player1.addToDiscard(c1);
                player1.addToDiscard(c2);
            }
            else if (c2.power > c1.power) {
                cout << "Round Winner: " << player2.name << endl;
                player2.addToDiscard(c1);
                player2.addToDiscard(c2);
            }
            else {
                cout << "It's a Tie!\n";
            }

            cout << "---------------------------------\n";
        }

        displayWinner();
    }

    void displayWinner() {
        int score1 = player1.getScore();
        int score2 = player2.getScore();

        cout << "\n--- GAME OVER ---\n";
        cout << player1.name << " Score: " << score1 << endl;
        cout << player2.name << " Score: " << score2 << endl;

        if (score1 > score2)
            cout << player1.name << " is the WINNER!\n";
        else if (score2 > score1)
            cout << player2.name << " is the WINNER!\n";
        else
            cout << "It's a DRAW!\n";
    }
};

int main() {
    Game g;
    g.setup();
    g.startGame();
    return 0;
}

