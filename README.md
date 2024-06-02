Blackjack Game
This is a simple command-line Blackjack game implemented in C++.

Overview
This project implements a text-based version of the classic casino card game, Blackjack. It allows a player to play against a computer-controlled dealer. The game follows standard Blackjack rules, where the player aims to beat the dealer's hand without exceeding a total value of 21.

Features:
-> Simple and intuitive text-based interface.
-> Randomized deck shuffling.
-> Basic Blackjack gameplay mechanics, including hitting and standing.
-> Automatic dealer play based on predefined rules.
-> Win, lose, or tie outcome determination.

Usage:
To use this Blackjack game, follow these steps:
-> Clone the repository to your local machine:
           git clone https://github.com/your-username/blackjack-game.git
-> Navigate to the project directory:
           cd blackjack-game
-> Compile the source code using a C++ compiler. For example, using g++:
           g++ main.cpp -o blackjack
-> Run the compiled executable:
           ./blackjack


How to Play
-> Upon starting the game, you will be dealt two cards, as will the dealer.
-> Your cards will be displayed, along with their total value.
-> You will be prompted to either hit (request another card) or stand (end your turn).
-> If you choose to hit, another card will be dealt to you.
-> If your total card value exceeds 21, you bust, and the dealer wins automatically.
-> Once you stand, it's the dealer's turn.
-> The dealer will continue to hit until their total card value is 17 or higher.
-> After the dealer's turn, the winner will be determined based on the total card values.
-> If you have a higher total value than the dealer without busting, you win.
-> If the dealer has a higher total value than you without busting, the dealer wins.
-> If both you and the dealer have the same total value, it's a tie.
