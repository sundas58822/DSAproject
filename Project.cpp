#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <conio.h>

using namespace std;

// CARD CLASS

class Card {
private:
    string name;
    int power;

public:
    Card() : name("Unknown"), power(0) {}
    Card(const string &name, int power) : name(name), power(power) {}

    string getName() const { return name; }
    int getPower() const { return power; }

    void display() const {
        cout << name << " [Power: " << power << "]";
    }
};

// PLAYER CLASS

class Player {
private:
    string name;
    queue<Card> deck;
    stack<Card> discard;

public:
    Player(string name="Player") : name(name) {}

    void setName(string n) { name = n; }
    string getName() const { return name; }

    void addCardToDeck(Card c) { deck.push(c); }
    bool hasCards() const { return !deck.empty(); }
    int remainingCards() const { return deck.size(); }

    Card drawCard() {
        Card c = deck.front();
        deck.pop();
        return c;
    }

    void addWinCards(Card c1, Card c2) {
        discard.push(c1);
        discard.push(c2);
    }

    void keepOwnCard(Card c) {
        discard.push(c);
    }

    int getScore() const {
        int cnt = 0;
        stack<Card> temp = discard;
        while(!temp.empty()){
            temp.pop();
            cnt++;
        }
        return cnt;
    }
};


// GAME CLASS
class Game {
private:
    vector<Card> cardPool;
    Player p1, p2;
    bool vsComputer = true;

public:
    Game() {}

    // ---------- START MENU ----------
    int showStartMenu() {
        int choice;
        do {
            cout << "=========================\n";
            cout << "     CARD BATTLE GAME    \n";
            cout << "=========================\n";
            cout << "1. Start New Game\n";
            cout << "0. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
            }
        } while(choice != 1 && choice != 0);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return choice;
    }

    // ---------- PLAYER MODE MENU ----------
    void chooseMode() {
        int mode;
        do {
            cout << "\nChoose Game Mode:\n";
            cout << "1. Player vs Computer\n";
            cout << "2. Player vs Player (2 humans)\n";
            cout << "Enter choice: ";
            cin >> mode;

            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                mode = -1;
            }
        } while(mode != 1 && mode != 2);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vsComputer = (mode == 1);
    }

    // ---------- PLAYER NAME SETUP ----------
    void setupPlayers() {
        string name;

        cout << "\nEnter Player 1 name: ";
        getline(cin, name);
        if(name.empty()) name="Player1";
        p1.setName(name);

        if(vsComputer){
            p2.setName("Computer");
        } else {
            cout << "Enter Player 2 name: ";
            getline(cin, name);
            if(name.empty()) name="Player2";
            p2.setName(name);
        }

        cout << "\nMatch: " << p1.getName() << " VS " << p2.getName() << "\n\n";
    }

    // ---------- SETTINGS ----------
    void askCardSettings(int &totalCards, int &distMode) {
        do {
            cout << "Enter total number of cards (even number, min 4): ";
            cin >> totalCards;

            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                totalCards = -1;
            }
        } while(totalCards < 4);

        if(totalCards % 2 != 0){
            totalCards--;
            cout << "Adjusted to even: " << totalCards << "\n";
        }

        do {
            cout << "\nCard Distribution Style:\n";
            cout << "1. Alternate Dealing\n";
            cout << "2. First Half to Player1, Second Half to Player2\n";
            cout << "3. Random Distribution (Equal Count)\n";
            cout << "Enter choice: ";
            cin >> distMode;

            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                distMode = -1;
            }
        } while(distMode < 1 || distMode > 3);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // ---------- CARD GENERATION + DISTRIBUTION ----------
    void generateAndDealCards(int total, int mode) {
        vector<string> names={"Knight","Mage","Archer","Dragon","Rogue",
                              "Golem","Assassin","Paladin","Wizard","Hunter"};

        cardPool.clear();

        for(int i=0;i<total;i++){
            string nm = names[i % names.size()] + " #" + to_string(i+1);
            int power = (rand() % 91) + 10;
            cardPool.push_back(Card(nm, power));
        }

        random_shuffle(cardPool.begin(), cardPool.end());

        // clear decks
        p1 = Player(p1.getName());
        p2 = Player(p2.getName());

        if(mode==1){
            for(int i=0;i<total;i++){
                (i % 2 == 0 ? p1 : p2).addCardToDeck(cardPool[i]);
            }
        }
        else if(mode==2){
            int half = total/2;
            for(int i=0;i<total;i++){
                (i<half ? p1:p2).addCardToDeck(cardPool[i]);
            }
        }
        else{
            int half = total / 2;
            for(int i=0;i<total;i++){
                if(half>0){
                    p1.addCardToDeck(cardPool[i]);
                    half--;
                } else {
                    p2.addCardToDeck(cardPool[i]);
                }
            }
        }

        cout << "\nDecks Ready!\n";
        cout << p1.getName() << ": " << p1.remainingCards() << " cards\n";
        cout << p2.getName() << ": " << p2.remainingCards() << " cards\n\n";
    }

    // ---------- ROUND ----------
    void playRound() {
        if(!p1.hasCards() || !p2.hasCards()){
            cout << "No more rounds possible.\n";
            return;
        }

        cout << "\n--- ROUND ---\n";
        Card c1 = p1.drawCard();
        Card c2 = p2.drawCard();

        cout << p1.getName() << " plays: ";
        c1.display();
        cout << "\n";

        cout << p2.getName() << " plays: ";
        c2.display();
        cout << "\n";

        if(c1.getPower() > c2.getPower()){
            cout << p1.getName() << " wins the round!\n";
            p1.addWinCards(c1,c2);
        }
        else if(c2.getPower() > c1.getPower()){
            cout << p2.getName() << " wins!\n";
            p2.addWinCards(c1,c2);
        }
        else {
            cout << "Draw. Each keeps own card.\n";
            p1.keepOwnCard(c1);
            p2.keepOwnCard(c2);
        }

        cout << "Remaining -> " << p1.getName() << ": " << p1.remainingCards()
             << ", " << p2.getName() << ": " << p2.remainingCards() << "\n";
    }

    // ---------- RESULTS ----------
    void showScores() {
        cout << "\nScores:\n";
        cout << p1.getName() << ": " << p1.getScore() << "\n";
        cout << p2.getName() << ": " << p2.getScore() << "\n\n";
    }

    void finalResult(){
        cout << "\n===== FINAL RESULT =====\n";
        int s1=p1.getScore(), s2=p2.getScore();

        cout << p1.getName() << ": " << s1 << "\n";
        cout << p2.getName() << ": " << s2 << "\n";

        if(s1>s2) cout << "\nWinner: " << p1.getName() << " ??\n";
        else if(s2>s1) cout << "\nWinner: " << p2.getName() << " ??\n";
        else cout << "\nMatch Drawn ??\n";

        cout << "=========================\n\n";
    }

    // ---------- GAME MENU ----------
    void gameMenu() {
        while(true){
            if(!p1.hasCards() || !p2.hasCards()){
                finalResult();
                break;
            }

            cout << "1. Play Next Round\n";
            cout << "2. View Scores\n";
            cout << "3. View remaining cards\n";
            cout << "0. End Game\n";
            cout << "Enter choice: ";

            int ch;
            cin >> ch;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if(ch==1) playRound();
            else if(ch==2) showScores();
            else if(ch==3){
                cout << p1.getName() << ": " << p1.remainingCards() << "\n";
                cout << p2.getName() << ": " << p2.remainingCards() << "\n\n";
            }
            else if(ch==0){
                finalResult();
                break;
            }
        }
    }

    // ---------- RUN ----------
    void run(){
        while(true){
            if(showStartMenu()==0){
                cout << "Goodbye!\n";
                break;
            }

            chooseMode();
            setupPlayers();

            int totalCards, distMode;
            askCardSettings(totalCards, distMode);

            generateAndDealCards(totalCards, distMode);

            gameMenu();
        }
    }
};

// MAIN

int main() {
    srand(time(0));

    Game g;
    g.run();

    cout << "\nPress any key to exit...";
    getch();
    return 0;
}
