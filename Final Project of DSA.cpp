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
// COLOR CODES (for console text colors)
// ===============================
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// ===============================
// HELPER FUNCTIONS
// ===============================

// Clear screen using system command
void clearScreen() {
    system("cls");
}

// Wait for user to press enter
void waitForEnter() {
    cout << YELLOW << "\nPress ENTER to continue..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Convert integer to string
string intToString(int x) {
    stringstream ss;
    ss << x;
    return ss.str();
}

// ===============================
// CARD CLASS
// ===============================

// Represents a single card with name, power, and rarity
class Card {
private:
    string name;
    int power;
    string rarity;

public:
    // Default constructor
    Card() {
        name = "Unknown";
        power = 0;
        rarity = "Common";
    }

    // Parameterized constructor
    Card(string n, int p, string r) {
        name = n;
        power = p;
        rarity = r;
    }

    // Getters
    string getName() const { return name; }
    int getPower() const { return power; }
    string getRarity() const { return rarity; }

    // Rarity colors for display
    string rarityColor() const {
        if (rarity == "Legendary") return RED;
        if (rarity == "Epic") return MAGENTA;
        if (rarity == "Rare") return BLUE;
        return WHITE;
    }

    // Display card info
    void display() const {
        cout << rarityColor() << name
             << " (" << rarity << " | Power: " << power << ")"
             << RESET;
    }
};

// ===============================
// PLAYER CLASS
// ===============================

class Player {
private:
    string name;
    queue<Card> deck;       // Cards to draw
    stack<Card> discard;    // Cards won

public:
    Player(string n = "Player") {
        name = n;
    }

    // Set / get name
    void setName(string n) { name = n; }
    string getName() const { return name; }

    // Add card to player's deck
    void addCardToDeck(const Card &c) { deck.push(c); }

    // Check if player still has cards
    bool hasCards() const { return !deck.empty(); }

    // Remaining cards count
    int remainingCards() const { return (int)deck.size(); }

    // Draw top card
    Card drawCard() {
        Card c = deck.front();
        deck.pop();
        return c;
    }

    // Player wins round and takes both cards
    void addWinCards(const Card &c1, const Card &c2) {
        discard.push(c1);
        discard.push(c2);
    }

    // Draw result is tie
    void keepOwnCard(const Card &c) {
        discard.push(c);
    }

    // Score is total cards in discard
    int getScore() const { return (int)discard.size(); }

    // Copy deck for viewing
    queue<Card> getDeckSnapshot() const {
        return deck;
    }
};

// ===============================
// GAME CLASS
// ===============================

class Game {
private:
    vector<Card> cardPool;  // All generated cards
    Player p1, p2;
    bool vsComputer;        // Mode flag
    int roundNumber;

public:
    Game() {
        vsComputer = true;
        roundNumber = 1;
    }

    // Show title screen
    void showTitleScreen() {
        clearScreen();
        cout << CYAN;
        cout << "=====================================\n";
        cout << "           CARD BATTLE GAME          \n";
        cout << "=====================================\n";
        cout << "       A DSA-based Console Game      \n";
        cout << "-------------------------------------\n\n";
        cout << RESET;
    }

    // Main menu before game
    int showStartMenu() {
        int choice = -1;
        do {
            cout << YELLOW << "1. Start New Game\n0. Exit\nEnter choice: " << RESET;
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

    // Choose PvP or PvC mode
    void chooseMode() {
        int mode = -1;
        do {
            clearScreen();
            cout << CYAN << "Choose Game Mode:\n" << RESET;
            cout << YELLOW << "1. Player vs Computer\n2. Player vs Player\nEnter choice: " << RESET;
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

    // Player names setup
    void setupPlayers() {
        clearScreen();
        string name;

        cout << CYAN << "Enter Player 1 name: " << RESET;
        getline(cin, name);
        if (name.empty()) name = "Player1";
        p1.setName(name);

        if (vsComputer) {
            p2.setName("Computer");
        } else {
            cout << CYAN << "Enter Player 2 name: " << RESET;
            getline(cin, name);
            if (name.empty()) name = "Player2";
            p2.setName(name);
        }

        clearScreen();
        cout << GREEN << "Match Setup:\n" << RESET;
        cout << GREEN << p1.getName() << RESET << "  VS  "
             << GREEN << p2.getName() << RESET << "\n";
        waitForEnter();
    }

    // Card count + distribution settings
    void askGameSettings(int &totalCards, int &distMode) {
        clearScreen();
        do {
            cout << YELLOW << "Enter TOTAL number of cards (even number, min 4): " << RESET;
            cin >> totalCards;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                totalCards = -1;
            }
        } while (totalCards < 4);

        if (totalCards % 2 != 0) {
            totalCards--;
            cout << MAGENTA << "Adjusted to even total: " << totalCards << RESET << "\n";
        }

        do {
            cout << CYAN << "\nChoose card distribution style:\n" << RESET;
            cout << YELLOW << "1. Alternate dealing\n2. First half to P1\n3. Random equal\nEnter choice: " << RESET;
            cin >> distMode;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                distMode = -1;
            }
        } while (distMode < 1 || distMode > 3);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        clearScreen();
        cout << CYAN << "Game Settings:\n" << RESET;
        cout << YELLOW << "Total cards: " << totalCards << "\nDistribution mode: " << distMode << RESET << "\n";
        waitForEnter();
    }

    // Convert power to rarity
    string getRarity(int power) {
        if (power >= 90) return "Legendary";
        if (power >= 70) return "Epic";
        if (power >= 50) return "Rare";
        return "Common";
    }

    // Create card pool and distribute to players
    void generateAndDistributeCards(int totalCards, int distMode) {
        clearScreen();
        cout << CYAN << "Generating cards...\n" << RESET;

        vector<string> baseNames = {
            "Knight", "Dragon", "Wizard", "Archer", "Assassin",
            "Golem", "Hunter", "Paladin", "Samurai", "Mage"
        };

        cardPool.clear();

        // Generate random cards
        for (int i = 0; i < totalCards; ++i) {
            string nm = baseNames[i % baseNames.size()] + " #" + intToString(i + 1);
            int power = (rand() % 91) + 10;
            string rarity = getRarity(power);
            cardPool.push_back(Card(nm, power, rarity));
        }

        random_shuffle(cardPool.begin(), cardPool.end());

        // Reset players
        p1 = Player(p1.getName());
        p2 = Player(p2.getName());

        int half = totalCards / 2;

        // THREE distribution modes
        if (distMode == 1) {
            for (int i = 0; i < totalCards; i++)
                (i % 2 == 0 ? p1 : p2).addCardToDeck(cardPool[i]);
        } else if (distMode == 2) {
            for (int i = 0; i < totalCards; i++)
                (i < half ? p1 : p2).addCardToDeck(cardPool[i]);
        } else {
            int p1count = 0;
            for (int i = 0; i < totalCards; i++) {
                if ((rand() % 2 == 0 && p1count < half) || (i - p1count) >= half) {
                    p1.addCardToDeck(cardPool[i]);
                    p1count++;
                } else {
                    p2.addCardToDeck(cardPool[i]);
                }
            }
        }

        clearScreen();
        cout << GREEN << "Decks are ready!\n\n" << RESET;
        cout << p1.getName() << " has " << p1.remainingCards() << " cards.\n";
        cout << p2.getName() << " has " << p2.remainingCards() << " cards.\n";
        waitForEnter();

        roundNumber = 1;
    }

    // Show a player's deck
    void printDeckList(const Player &pl) {
        cout << CYAN << pl.getName() << "'s current deck:\n" << RESET;

        queue<Card> temp = pl.getDeckSnapshot();
        if (temp.empty()) {
            cout << WHITE << "  [No cards]\n\n" << RESET;
            return;
        }

        int index = 1;
        while (!temp.empty()) {
            cout << YELLOW << "  " << index++ << ". " << RESET;
            temp.front().display();
            cout << "\n";
            temp.pop();
        }
        cout << "\n";
    }

    // Compare card powers
    int compareCards(const Card &c1, const Card &c2) {
        if (c1.getPower() > c2.getPower()) return 1;
        if (c2.getPower() > c1.getPower()) return -1;
        return 0;
    }

    // Play one round of the game
    void playRound() {
        if (!p1.hasCards() || !p2.hasCards()) {
            cout << RED << "Cannot play round: one deck empty.\n" << RESET;
            waitForEnter();
            return;
        }

        clearScreen();
        cout << CYAN << "========== ROUND " << roundNumber << " ==========\n\n" << RESET;

        printDeckList(p1);
        printDeckList(p2);

        cout << YELLOW << "Press ENTER to draw cards..." << RESET;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        Card c1 = p1.drawCard();
        Card c2 = p2.drawCard();

        cout << "\n" << GREEN << p1.getName() << " plays: " << RESET;
        c1.display(); cout << "\n";

        cout << GREEN << p2.getName() << " plays: " << RESET;
        c2.display(); cout << "\n\n";

        int res = compareCards(c1, c2);

        if (res == 1) {
            cout << GREEN << p1.getName() << " wins this round!\n" << RESET;
            p1.addWinCards(c1, c2);
        } else if (res == -1) {
            cout << GREEN << p2.getName() << " wins this round!\n" << RESET;
            p2.addWinCards(c1, c2);
        } else {
            cout << MAGENTA << "It's a draw. Each keeps their card.\n" << RESET;
            p1.keepOwnCard(c1);
            p2.keepOwnCard(c2);
        }

        cout << CYAN << "\nRemaining cards:\n" << RESET;
        cout << p1.getName() << ": " << p1.remainingCards() << "\n";
        cout << p2.getName() << ": " << p2.remainingCards() << "\n";

        roundNumber++;
        waitForEnter();
    }

    // Show current scores
    void showScores() {
        clearScreen();
        cout << CYAN << "======== CURRENT SCORE ========\n" << RESET;
        cout << p1.getName() << ": " << p1.getScore() << "\n";
        cout << p2.getName() << ": " << p2.getScore() << "\n";
        waitForEnter();
    }

    // Show remaining cards count
    void showRemainingCards() {
        clearScreen();
        cout << CYAN << "==== REMAINING CARDS ====\n" << RESET;
        cout << p1.getName() << ": " << p1.remainingCards() << "\n";
        cout << p2.getName() << ": " << p2.remainingCards() << "\n";
        waitForEnter();
    }

    // View player's deck menu
    void viewDeckMenu() {
        clearScreen();
        if (vsComputer) {
            printDeckList(p1);
        } else {
            int choice;
            cout << CYAN << "Whose deck do you want to view?\n" << RESET;
            cout << "1. " << p1.getName() << "\n";
            cout << "2. " << p2.getName() << "\n";
            cout << "Enter choice: ";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 1) printDeckList(p1);
            else printDeckList(p2);
        }
        waitForEnter();
    }

    // Show the final winner
    void showFinalResult() {
        clearScreen();
        int s1 = p1.getScore();
        int s2 = p2.getScore();

        cout << CYAN << "========== FINAL RESULT ==========" << RESET << "\n";
        cout << p1.getName() << " score: " << s1 << "\n";
        cout << p2.getName() << " score: " << s2 << "\n";

        if (s1 > s2)
            cout << GREEN << "\nWINNER: " << p1.getName() << " \n" << RESET;
        else if (s2 > s1)
            cout << GREEN << "\nWINNER: " << p2.getName() << " \n" << RESET;
        else
            cout << MAGENTA << "\nMATCH DRAW \n" << RESET;

        waitForEnter();
    }

    // In-game menu loop
    void gameLoop() {
        while (true) {
            if (!p1.hasCards() || !p2.hasCards()) {
                showFinalResult();
                break;
            }

            clearScreen();
            cout << YELLOW << "======== GAME MENU ========\n";
            cout << "1. Play Next Round\n";
            cout << "2. View Scores\n";
            cout << "3. View Remaining Cards\n";
            cout << "4. View Deck\n";
            cout << "0. End Game Now\n";
            cout << "===========================\n";
            cout << "Enter choice: " << RESET;

            int ch;
            cin >> ch;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (ch == 1) playRound();
            else if (ch == 2) showScores();
            else if (ch == 3) showRemainingCards();
            else if (ch == 4) viewDeckMenu();
            else if (ch == 0) {
                showFinalResult();
                break;
            }
        }
    }

    // Run a full game
    void runSingleGame() {
        chooseMode();
        setupPlayers();

        int totalCards, distMode;
        askGameSettings(totalCards, distMode);

        generateAndDistributeCards(totalCards, distMode);
        gameLoop();
    }

    // Outer loop
    void run() {
        while (true) {
            showTitleScreen();
            int choice = showStartMenu();
            if (choice == 0) {
                clearScreen();
                cout << GREEN << "Thanks for playing Card Battle Game!\n" << RESET;
                break;
            }
            runSingleGame();
        }
    }
};

// ===============================
// MAIN ENTRY POINT
// ===============================

int main() {
    srand((unsigned)time(0)); // Seed RNG
    Game game;
    game.run();

    cout << YELLOW << "\nPress any key to exit..." << RESET;
    getch();
    return 0;
}

