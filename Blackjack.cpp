#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
using namespace std;

int getRandomNumber(int min, int max) {
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

class Card {
private:
    enum class Rank {
        ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
        JACK, QUEEN, KING
    };

    enum class Suit {
        CLUBS, DIAMONDS, HEARTS, SPADES
    };

    Rank m_rank;
    Suit m_suit;

public:
    Card(Rank rank = Rank::ACE, Suit suit = Suit::SPADES)
        : m_rank(rank), m_suit(suit) {}

    void print() const {
        switch (m_rank) {
            case Rank::ACE:    cout << 'A'; break;
            case Rank::TWO:    cout << '2'; break;
            case Rank::THREE:  cout << '3'; break;
            case Rank::FOUR:   cout << '4'; break;
            case Rank::FIVE:   cout << '5'; break;
            case Rank::SIX:    cout << '6'; break;
            case Rank::SEVEN:  cout << '7'; break;
            case Rank::EIGHT:  cout << '8'; break;
            case Rank::NINE:   cout << '9'; break;
            case Rank::TEN:    cout << 'T'; break;
            case Rank::JACK:   cout << 'J'; break;
            case Rank::QUEEN:  cout << 'Q'; break;
            case Rank::KING:   cout << 'K'; break;
        }

        switch (m_suit) {
            case Suit::CLUBS:    cout << 'C'; break;
            case Suit::DIAMONDS: cout << 'D'; break;
            case Suit::HEARTS:   cout << 'H'; break;
            case Suit::SPADES:   cout << 'S'; break;
        }
    }

    int getValue() const {
        switch (m_rank) {
            case Rank::TWO:    return 2;
            case Rank::THREE:  return 3;
            case Rank::FOUR:   return 4;
            case Rank::FIVE:   return 5;
            case Rank::SIX:    return 6;
            case Rank::SEVEN:  return 7;
            case Rank::EIGHT:  return 8;
            case Rank::NINE:   return 9;
            case Rank::TEN:    return 10;
            case Rank::JACK:   return 10;
            case Rank::QUEEN:  return 10;
            case Rank::KING:   return 10;
            case Rank::ACE:    return 11;
        }

        return 0; 
    }
};

class Deck {
private:
    vector<Card> m_deck;

public:
    Deck() {
        for (int suit = 0; suit < 4; ++suit) {
            for (int rank = 1; rank <= 13; ++rank) {
                m_deck.push_back(Card(static_cast<Card::Rank>(rank), static_cast<Card::Suit>(suit)));
            }
        }
    }

    void print() const {
        for (const auto &card : m_deck) {
            card.print();
            cout << ' ';
        }
        cout << '\n';
    }

    void shuffle() {
        std::random_shuffle(m_deck.begin(), m_deck.end());
    }

    const Card& dealCard() {
        Card& dealtCard = m_deck.back();
        m_deck.pop_back();
        return dealtCard;
    }
};

class Player {
private:
    vector<Card> m_hand;

public:
    void addCard(const Card& card) {
        m_hand.push_back(card);
    }

    int getHandValue() const {
        int totalValue = 0;
        int numAces = 0;

        for (const auto &card : m_hand) {
            totalValue += card.getValue();
            if (card.getValue() == 11) {
                ++numAces;
            }
        }

        while (totalValue > 21 && numAces > 0) {
            totalValue -= 10;
            --numAces;
        }

        return totalValue;
    }

    void print() const {
        for (const auto &card : m_hand) {
            card.print();
            cout << ' ';
        }
        cout << "(Value: " << getHandValue() << ")\n";
    }

    bool isBusted() const {
        return getHandValue() > 21;
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0)));
    rand();

    Deck deck;
    deck.shuffle();

    Player player;
    Player dealer;

    player.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());
    player.addCard(deck.dealCard());
    dealer.addCard(deck.dealCard());

    cout << "Dealer's hand: ";
    dealer.print();
    cout << "Your hand: ";
    player.print();

    while (!player.isBusted()) {
        char choice;
        cout << "Do you want to hit (h) or stand (s)? ";
        cin >> choice;

        if (choice == 'h') {
            player.addCard(deck.dealCard());
            cout << "Your hand: ";
            player.print();
        } else if (choice == 's') {
            break;
        }
    }

    if (player.isBusted()) {
        cout << "You busted! Dealer wins.\n";
        return 0;
    }

    cout << "Dealer's turn\n";
    cout << "Dealer's hand: ";
    dealer.print();
    while (dealer.getHandValue() < 17) {
        dealer.addCard(deck.dealCard());
        cout << "Dealer hits: ";
        dealer.print();
    }

    if (dealer.isBusted() || player.getHandValue() > dealer.getHandValue()) {
        cout << "You win!\n";
    } else if (dealer.getHandValue() > player.getHandValue()) {
        cout << "Dealer wins.\n";
    } else {
        cout << "It's a tie!\n";
    }

    return 0;
}
