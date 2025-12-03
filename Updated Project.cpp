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
#include <sstream>

using namespace std;

// ===============================
// HELPER FUNCTIONS
// ===============================

// Clear console screen
void clearScreen() {
    system("cls");
}

// Wait for ENTER key
void waitForEnter() {
    cout << "\nPress ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Convert int to string (safe for older compilers)
string intToString(int x) {
    stringstream ss;
    ss << x;
    return ss.str();
}

// ===============================
// CARD CLASS
// ===============================
class Card {
private:
    string name;
    int power;
    string rarity;

public:
    Card() {
        name = "Unknown";
        power = 0;
        rarity = "Common";
    }

    Card(string n, int p, string r) {
        name = n;
        power = p;
        rarity = r;
    }

    string getName() const { return name; }
    int getPower() const { return power; }
    string getRarity() const { return rarity; }

    void display() const {
        cout << name << " (" << rarity << " | Power: " << power << ")";
    }
};

// ===============================
// PLAYER CLASS
// ===============================
class Player {
private:
    string name;
    queue<Card> deck;       // deck (queue – FIFO)
    stack<Card> discard;    // won cards (stack – LIFO)

public:
    Player(string n = "Player") {
        name = n;
    }

    void setName(string n) { name = n; }
    string getName() const { return name; }

    void addCardToDeck(const Card &c) { deck.push(c); }

    bool hasCards() const { return !deck.empty(); }

    int remainingCards() const { return (int)deck.size(); }

    Card drawCard() {
        Card c = deck.front();
        deck.pop();
        return c;
    }

    // Winner takes both cards
    void addWinCards(const Card &c1, const Card &c2) {
        discard.push(c1);
        discard.push(c2);
    }

    // In a draw each keeps their own
    void keepOwnCard(const Card &c) {
        discard.push(c);
    }

    int getScore() const {
        return (int)discard.size();
    }

    // Snapshot of current deck (for display only)
    queue<Card> getDeckSnapshot() const {
        return deck;
    }
};

// ===============================
// GAME CLASS
// ===============================
class Game {
private:
    vector<Card> cardPool;
    Player p1, p2;
    bool vsComputer;
    int roundNumber;

public:
    Game() {
        vsComputer = true;
        roundNumber = 1;
    }

    // ---------- Title Screen ----------
    void showTitleScreen() {
        clearScreen();
        cout << "=====================================\n";
        cout << "           CARD BATTLE GAME          \n";
        cout << "=====================================\n\n";
        
    }

    // ---------- Start Menu ----------
    int showStartMenu() {
        int choice = -1;
        do {
            cout << "1. Start New Game\n";
            cout << "0. Exit\n";
            cout << "Enter choice: ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = -1;
            }
        } while (choice != 0 && choice != 1);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return choice;
    }

    // ---------- Mode Selection ----------
    void chooseMode() {
        int mode = -1;
        do {
            clearScreen();
            cout << "Choose Game Mode:\n";
            cout << "1. Player vs Computer\n";
            cout << "2. Player vs Player (2 Humans)\n";
            cout << "Enter choice: ";
            cin >> mode;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                mode = -1;
            }
        } while (mode != 1 && mode != 2);

        vsComputer = (mode == 1);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // ---------- Setup Players ----------
    void setupPlayers() {
        clearScreen();
        string name;

        cout << "Enter Player 1 name: ";
        getline(cin, name);
        if (name.empty()) name = "Player1";
        p1.setName(name);

        if (vsComputer) {
            p2.setName("Computer");
        } else {
            cout << "Enter Player 2 name: ";
            getline(cin, name);
            if (name.empty()) name = "Player2";
            p2.setName(name);
        }

        clearScreen();
        cout << "Match Setup:\n";
        cout << p1.getName() << "  VS  " << p2.getName() << "\n";
        waitForEnter();
    }

    // ---------- Settings ----------
    void askGameSettings(int &totalCards, int &distMode) {
        clearScreen();
        // total cards
        do {
            cout << "Enter TOTAL number of cards (even number, min 4): ";
            cin >> totalCards;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                totalCards = -1;
            }
        } while (totalCards < 4);

        if (totalCards % 2 != 0) {
            totalCards--;
            cout << "Adjusted to even total: " << totalCards << "\n";
        }

        // distribution mode
        do {
            cout << "\nChoose card distribution style:\n";
            cout << "1. Alternate dealing (1 to each)\n";
            cout << "2. First half to " << p1.getName() << ", second half to " << p2.getName() << "\n";
            cout << "3. Random but equal number of cards\n";
            cout << "Enter choice: ";
            cin >> distMode;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                distMode = -1;
            }
        } while (distMode < 1 || distMode > 3);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        clearScreen();
        cout << "Game Settings:\n";
        cout << "Total cards: " << totalCards << "\n";
        cout << "Distribution mode: " << distMode << "\n";
        waitForEnter();
    }

    // ---------- Rarity From Power ----------
    string getRarity(int power) {
        if (power >= 90) return "Legendary";
        if (power >= 70) return "Epic";
        if (power >= 50) return "Rare";
        return "Common";
    }

    // ---------- Card Generation & Distribution ----------
    void generateAndDistributeCards(int totalCards, int distMode) {
        clearScreen();
        cout << "Generating cards...\n";

        vector<string> baseNames;
        baseNames.push_back("Knight");
        baseNames.push_back("Dragon");
        baseNames.push_back("Wizard");
        baseNames.push_back("Archer");
        baseNames.push_back("Assassin");
        baseNames.push_back("Golem");
        baseNames.push_back("Hunter");
        baseNames.push_back("Paladin");
        baseNames.push_back("Samurai");
        baseNames.push_back("Mage");

        cardPool.clear();

        for (int i = 0; i < totalCards; ++i) {
            string nm = baseNames[i % baseNames.size()] + " #" + intToString(i + 1);
            int power = (rand() % 91) + 10; // 10 - 100
            string rarity = getRarity(power);
            cardPool.push_back(Card(nm, power, rarity));
        }

        random_shuffle(cardPool.begin(), cardPool.end());

        // Reset players with same names
        p1 = Player(p1.getName());
        p2 = Player(p2.getName());

        if (distMode == 1) {
            // alternate dealing
            for (int i = 0; i < totalCards; ++i) {
                if (i % 2 == 0) p1.addCardToDeck(cardPool[i]);
                else            p2.addCardToDeck(cardPool[i]);
            }
        } else if (distMode == 2) {
            // first half to p1, second half to p2
            int half = totalCards / 2;
            for (int i = 0; i < totalCards; ++i) {
                if (i < half) p1.addCardToDeck(cardPool[i]);
                else          p2.addCardToDeck(cardPool[i]);
            }
        } else {
            // random but equal count distribution
            int half = totalCards / 2;
            int givenToP1 = 0;
            for (int i = 0; i < totalCards; ++i) {
                int r = rand() % 2;
                if (r == 0 && givenToP1 < half) {
                    p1.addCardToDeck(cardPool[i]);
                    givenToP1++;
                } else if ((i - givenToP1) < half) {
                    p2.addCardToDeck(cardPool[i]);
                } else {
                    p1.addCardToDeck(cardPool[i]);
                    givenToP1++;
                }
            }
        }

        clearScreen();
        cout << "Decks are ready!\n\n";
        cout << p1.getName() << " has " << p1.remainingCards() << " cards.\n";
        cout << p2.getName() << " has " << p2.remainingCards() << " cards.\n";
        waitForEnter();

        roundNumber = 1;
    }

    // ---------- Print Deck List (Name + Power + Rarity) ----------
    void printDeckList(const Player &pl) {
        cout << pl.getName() << "'s current deck:\n";

        queue<Card> temp = pl.getDeckSnapshot();
        if (temp.empty()) {
            cout << "  [No cards]\n\n";
            return;
        }

        int index = 1;
        while (!temp.empty()) {
            cout << "  " << index++ << ". ";
            temp.front().display();
            cout << "\n";
            temp.pop();
        }
        cout << "\n";
    }

    // ---------- Comparison ----------
    int compareCards(const Card &c1, const Card &c2) {
        if (c1.getPower() > c2.getPower()) return 1;
        if (c2.getPower() > c1.getPower()) return -1;
        return 0;
    }

    // ---------- One Round ----------
    void playRound() {
        if (!p1.hasCards() || !p2.hasCards()) {
            cout << "Cannot play round: one of the decks is empty.\n";
            waitForEnter();
            return;
        }

        clearScreen();
        cout << "========== ROUND " << roundNumber << " ==========\n\n";

        // Show both players' decks BEFORE playing round
        printDeckList(p1);
        printDeckList(p2);

        cout << "Press ENTER to draw cards...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Card c1 = p1.drawCard();
        Card c2 = p2.drawCard();

        cout << "\n";
        cout << p1.getName() << " plays: ";
        c1.display();
        cout << "\n";

        cout << p2.getName() << " plays: ";
        c2.display();
        cout << "\n\n";

        int res = compareCards(c1, c2);
        if (res == 1) {
            cout << p1.getName() << " wins this round!\n";
            p1.addWinCards(c1, c2);
        } else if (res == -1) {
            cout << p2.getName() << " wins this round!\n";
            p2.addWinCards(c1, c2);
        } else {
            cout << "It's a draw. Each keeps their own card.\n";
            p1.keepOwnCard(c1);
            p2.keepOwnCard(c2);
        }

        cout << "\nRemaining cards:\n";
        cout << "  " << p1.getName() << ": " << p1.remainingCards() << "\n";
        cout << "  " << p2.getName() << ": " << p2.remainingCards() << "\n";

        roundNumber++;
        waitForEnter();
    }

    // ---------- Score / Remaining ----------
    void showScores() {
        clearScreen();
        cout << "======== CURRENT SCORE ========\n";
        cout << p1.getName() << ": " << p1.getScore() << "\n";
        cout << p2.getName() << ": " << p2.getScore() << "\n";
        waitForEnter();
    }

    void showRemainingCards() {
        clearScreen();
        cout << "==== REMAINING CARDS IN DECK ====\n";
        cout << p1.getName() << ": " << p1.remainingCards() << "\n";
        cout << p2.getName() << ": " << p2.remainingCards() << "\n";
        waitForEnter();
    }

    // ---------- View Deck Menu (Optional) ----------
    void viewDeckMenu() {
        clearScreen();
        if (vsComputer) {
            printDeckList(p1);
        } else {
            int choice;
            cout << "Whose deck do you want to view?\n";
            cout << "1. " << p1.getName() << "\n";
            cout << "2. " << p2.getName() << "\n";
            cout << "Enter choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 1) printDeckList(p1);
            else             printDeckList(p2);
        }
        waitForEnter();
    }

    // ---------- Final Result ----------
    void showFinalResult() {
        clearScreen();
        int s1 = p1.getScore();
        int s2 = p2.getScore();

        cout << "========== FINAL RESULT ==========\n";
        cout << p1.getName() << " score: " << s1 << "\n";
        cout << p2.getName() << " score: " << s2 << "\n";

        if (s1 > s2) {
            cout << "\nWINNER: " << p1.getName() << " ??\n";
        } else if (s2 > s1) {
            cout << "\nWINNER: " << p2.getName() << " ??\n";
        } else {
            cout << "\nMATCH DRAW ??\n";
        }
        waitForEnter();
    }

    // ---------- Game Loop (Menu during game) ----------
    void gameLoop() {
        while (true) {
            if (!p1.hasCards() || !p2.hasCards()) {
                showFinalResult();
                break;
            }

            clearScreen();
            cout << "======== GAME MENU ========\n";
            cout << "1. Play Next Round\n";
            cout << "2. View Scores\n";
            cout << "3. View Remaining Cards\n";
            cout << "4. View Deck\n";
            cout << "0. End Game Now\n";
            cout << "===========================\n";
            cout << "Enter choice: ";

            int ch;
            cin >> ch;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (ch == 1)      playRound();
            else if (ch == 2) showScores();
            else if (ch == 3) showRemainingCards();
            else if (ch == 4) viewDeckMenu();
            else if (ch == 0) {
                showFinalResult();
                break;
            }
        }
    }

    // ---------- Run Single Game ----------
    void runSingleGame() {
        chooseMode();
        setupPlayers();

        int totalCards, distMode;
        askGameSettings(totalCards, distMode);
        generateAndDistributeCards(totalCards, distMode);

        gameLoop();
    }

    // ---------- Run Whole Program ----------
    void run() {
        while (true) {
            showTitleScreen();
            int choice = showStartMenu();
            if (choice == 0) {
                clearScreen();
                cout << "Thanks for playing Card Battle Game!\n";
                break;
            }
            runSingleGame();
        }
    }
};

// ===============================
// MAIN FUNCTION
// ===============================
int main() {
    srand((unsigned)time(0));

    Game game;
    game.run();

    cout << "\nPress any key to exit...";
    getch();
    return 0;
}

