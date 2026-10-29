// Blackjack - enhanced console edition (C++17)
// Build: g++ -std=c++17 -O2 blackjack.cpp -o blackjack
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <limits>
using namespace std;

class Card {
public:
    // Public so Deck can build cards (originally private, which wouldn't compile)
    enum class Rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };
    enum class Suit { CLUBS, DIAMONDS, HEARTS, SPADES };

    Card(Rank r, Suit s) : m_rank(r), m_suit(s) {}

    string str() const {
        static const char* ranks[] = {"", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
        static const char* suits[] = {"♣", "♦", "♥", "♠"};
        return string("[") + ranks[static_cast<int>(m_rank)] + suits[static_cast<int>(m_suit)] + "]";
    }

    int value() const {
        int r = static_cast<int>(m_rank);
        if (r == 1) return 11;
        return r >= 10 ? 10 : r;
    }
    bool isAce() const { return m_rank == Rank::ACE; }

private:
    Rank m_rank;
    Suit m_suit;
};

class Deck {
public:
    Deck() { reset(); }

    void reset() {
        m_cards.clear();
        for (int s = 0; s < 4; ++s)
            for (int r = 1; r <= 13; ++r)
                m_cards.emplace_back(static_cast<Card::Rank>(r), static_cast<Card::Suit>(s));
        shuffle(m_cards.begin(), m_cards.end(), m_rng);
    }

    // Returns by value: the original returned a reference to a popped element (dangling)
    Card deal() {
        if (m_cards.size() < 15) reset();  // reshuffle when the shoe runs low
        Card c = m_cards.back();
        m_cards.pop_back();
        return c;
    }

private:
    vector<Card> m_cards;
    mt19937 m_rng{random_device{}()};
};

class Hand {
public:
    void add(const Card& c) { m_cards.push_back(c); }
    void clear() { m_cards.clear(); }
    size_t size() const { return m_cards.size(); }

    int value() const {
        int total = 0, aces = 0;
        for (const auto& c : m_cards) {
            total += c.value();
            if (c.isAce()) ++aces;
        }
        while (total > 21 && aces-- > 0) total -= 10;
        return total;
    }
    bool busted() const { return value() > 21; }
    bool blackjack() const { return m_cards.size() == 2 && value() == 21; }

    void print(bool hideSecond = false) const {
        for (size_t i = 0; i < m_cards.size(); ++i)
            cout << (hideSecond && i == 1 ? "[??]" : m_cards[i].str()) << ' ';
        if (!hideSecond) cout << " = " << value();
        cout << '\n';
    }

private:
    vector<Card> m_cards;
};

struct Seat {
    string name; bool bot; int bank = 500, bet = 25; Hand hand;
};

int readInt(const string& prompt, int lo, int hi) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x && x >= lo && x <= hi) return x;
        if (cin.eof()) exit(0);
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Enter a number from " << lo << " to " << hi << ".\n";
    }
}
char readChoice(const string& prompt, const string& ok) {
    char c;
    while (cout << prompt, cin >> c) {
        c = static_cast<char>(tolower(c));
        if (ok.find(c) != string::npos) return c;
        cout << "  Choose one of: " << ok << "\n";
    }
    exit(0);
}
void show(const string& who, const Hand& h, bool hide = false) {
    cout << "  " << who << ": "; h.print(hide);
}

int main() {
    cout << "===== C++ BLACKJACK: MULTIPLAYER =====\n";
    int humans = readInt("Human players (0-4): ", 0, 4);
    int bots = readInt("Bot players (0-" + to_string(4 - humans) + "): ", 0, 4 - humans);
    if (humans + bots == 0) { cout << "Nobody at the table!\n"; return 0; }

    vector<Seat> seats;
    for (int i = 1; i <= humans; ++i) {
        string n; cout << "Name for player " << i << ": "; cin >> n;
        seats.push_back({n, false});
    }
    const char* botNames[] = {"Ava", "Max", "Zoe", "Leo"};
    for (int i = 0; i < bots; ++i) seats.push_back({string(botNames[i]) + " (bot)", true});

    Deck deck;
    Hand dealer;
    for (int round = 1;; ++round) {
        seats.erase(remove_if(seats.begin(), seats.end(), [](const Seat& s) {
            if (s.bank <= 0) cout << s.name << " is out of chips!\n";
            return s.bank <= 0; }), seats.end());
        if (seats.empty()) { cout << "\nEveryone is broke. Game over!\n"; break; }

        cout << "\n=== Round " << round << " ===\n";
        for (const auto& s : seats) cout << "  " << s.name << ": $" << s.bank << '\n';
        if (readChoice("Play round? (y/n) ", "yn") == 'n') break;

        for (auto& s : seats) {
            s.hand.clear();
            s.bet = s.bot ? min(25, s.bank)
                          : readInt(s.name + ", bet (1-" + to_string(s.bank) + "): ", 1, s.bank);
        }
        dealer.clear();
        for (int k = 0; k < 2; ++k) { for (auto& s : seats) s.hand.add(deck.deal()); dealer.add(deck.deal()); }
        show("Dealer", dealer, true);

        for (auto& s : seats) {
            cout << "\n--- " << s.name << "'s turn ---\n";
            show("Hand", s.hand);
            while (!s.hand.busted() && s.hand.value() < 21) {
                char c;
                if (s.bot) c = s.hand.value() < 17 ? 'h' : 's';
                else {
                    bool dbl = s.hand.size() == 2 && s.bank >= s.bet * 2;
                    c = readChoice(dbl ? "(H)it, (S)tand, (D)ouble? " : "(H)it or (S)tand? ", dbl ? "hsd" : "hs");
                }
                if (c == 's') break;
                if (c == 'd') s.bet *= 2;
                s.hand.add(deck.deal());
                show("Hand", s.hand);
                if (c == 'd') break;
            }
            if (s.hand.busted()) cout << "  Bust!\n";
        }

        cout << "\n--- Dealer reveals ---\n";
        while (dealer.value() < 17) dealer.add(deck.deal());
        show("Dealer", dealer);

        cout << "\n--- Results ---\n";
        for (auto& s : seats) {
            int p = s.hand.value(), d = dealer.value(), delta;
            if (p > 21) delta = -s.bet;
            else if (s.hand.blackjack() && !dealer.blackjack()) delta = s.bet * 3 / 2;
            else if (dealer.busted() || p > d) delta = s.bet;
            else if (p < d || dealer.blackjack()) delta = -s.bet;
            else delta = 0;
            s.bank += delta;
            cout << "  " << s.name << ": " << (delta > 0 ? "WIN +$" : delta < 0 ? "LOSE -$" : "PUSH $")
                 << abs(delta) << "  (bank $" << s.bank << ")\n";
        }
    }
    cout << "\nFinal standings:\n";
    sort(seats.begin(), seats.end(), [](auto& a, auto& b) { return a.bank > b.bank; });
    for (const auto& s : seats) cout << "  " << s.name << ": $" << s.bank << '\n';
}
