#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
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

    Player(string n = "") {
        name = n;
    }
};

class GameSetup {
public:
    vector<Card> allCards;
    Player player1, player2;

    GameSetup() {
        srand(time(0));
    }

    void createPlayers() {
        cout << "Enter Player 1 Name: ";
        cin >> player1.name;
        cout << "Enter Player 2 Name: ";
        cin >> player2.name;
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

    void storeCards() {
        cout << "Cards stored successfully!\n";
    }

    void shuffleAndDistribute() {
        random_shuffle(allCards.begin(), allCards.end());
        for (size_t i = 0; i < allCards.size(); i++) {
            if (i % 2 == 0)
                player1.deck.push(allCards[i]);
            else
                player2.deck.push(allCards[i]);
        }
        cout << "Cards shuffled and distributed equally!\n";
    }
};

int main() {
    GameSetup game;
    game.createPlayers();
    game.createCards();
    game.storeCards();
    game.shuffleAndDistribute();
    return 0;
}

