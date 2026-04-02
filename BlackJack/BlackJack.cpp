#include <cstdlib>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <string>
#include <random>

class Player {
private:
	int balance = 1500;
	int bet = 0;

public:

	void win() {
		balance += bet;
	}

	void lose() {
		balance -= bet;
	}

	int getBalance() {
		return balance;
	}

	int getBet() {
		return bet;
	}

	void setBet(int n) {
		bet = n;
	}
};

struct Card {
	std::string rank;
	int value;
};

std::vector<Card> fullDeck() {
	std::vector<Card> deck;
	for (int i = 0; i < 4; i++) {
	 deck.push_back({ "2", 2 });
	 deck.push_back({ "3", 3 });
	 deck.push_back({ "4", 4 });
	 deck.push_back({ "5", 5 });
	 deck.push_back({ "6", 6 });
	 deck.push_back({ "7", 7 });
	 deck.push_back({ "8", 8 });
	 deck.push_back({ "9", 9 });
	 deck.push_back({ "10", 10 });
	 deck.push_back({ "J", 10 });
	 deck.push_back({ "Q", 10 });
	 deck.push_back({ "K", 10 });
	 deck.push_back({ "A", 11 });
	}

	return deck;
}

void shuffleDeck(std::vector<Card>& deck) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::shuffle(deck.begin(), deck.end(), gen);
}

std::vector<Card> giveStartCards(std::vector<Card>& deck) {
	std::vector<Card> hand;

	hand.push_back(deck.back());
	deck.pop_back();

	hand.push_back(deck.back());
	deck.pop_back();

	return hand;
}

Card addCard(std::vector<Card>& deck) {
	Card card = deck.back();
	deck.pop_back();
	return card;
}

int calculateScore(const std::vector<Card>& hand) {
	int sum = 0;
	int aces = 0;

	for (const auto& card : hand) {
		sum += card.value;

		if (card.rank == "A") {
			aces++;
		}
	}

	while (sum > 21 && aces > 0) {
		sum -= 10;
		aces--;
	}

	return sum;
}

void showHand(const std::vector<Card>& hand) {
	for (const auto& card : hand) {
		std::cout << card.rank << " ";
	}

	std::cout << "\nScore: " << calculateScore(hand) << std::endl;
}

void determineWinner(const std::vector<Card>& playerHand,
	const std::vector<Card>& dealerHand) {
	int playerScore = calculateScore(playerHand);
	int dealerScore = calculateScore(dealerHand);

	if (dealerScore > 21 || playerScore > dealerScore) {
		std::cout << "You win!\n";
	}
	else if (playerScore < dealerScore) {
		std::cout << "Dealer wins!\n";
	}
	else {
		std::cout << "Draw!\n";
	}
}

void placeBet(Player& player) {
	int bet;

	while (true) {
		std::cout << "Your balance: " << player.getBalance() << std::endl;
		std::cout << "Enter your bet: ";
		std::cin >> bet;

		if (bet > 0 && bet <= player.getBalance()) {
			player.setBet(bet);
			break;
		}

		std::cout << "Invalid bet. Try again.\n";
	}
}

int main() {
	system("chcp 1251 > nul");

	Player player;

	placeBet(player);

	std::vector<Card> deck = fullDeck();
	shuffleDeck(deck);

	std::vector<Card> playerHand = giveStartCards(deck);
	std::vector<Card> dealerHand = giveStartCards(deck);

	std::cout << "\nYour cards:\n";
	showHand(playerHand);

	std::cout << "\nDealer shows: "
		<< dealerHand[0].rank << std::endl;

	char choice;

	while (true) {
		std::cout << "\nHit or stand? (h/s): ";
		std::cin >> choice;

		if (choice == 'h') {
			playerHand.push_back(addCard(deck));

			std::cout << "\nYour cards:\n";
			showHand(playerHand);

			if (calculateScore(playerHand) > 21) {
				std::cout << "Bust! You lose.\n";
				player.lose();

				std::cout << "Balance: "
					<< player.getBalance() << std::endl;
				return 0;
			}
		}
		else if (choice == 's') {
			break;
		}
	}

	while (calculateScore(dealerHand) < 17) {
		dealerHand.push_back(addCard(deck));
	}

	std::cout << "\nDealer cards:\n";
	showHand(dealerHand);

	int playerScore = calculateScore(playerHand);
	int dealerScore = calculateScore(dealerHand);

	if (dealerScore > 21 || playerScore > dealerScore) {
		std::cout << "You win!\n";
		player.win();
	}
	else if (playerScore < dealerScore) {
		std::cout << "Dealer wins!\n";
		player.lose();
	}
	else {
		std::cout << "Draw!\n";
	}

	std::cout << "Balance: "
		<< player.getBalance() << std::endl;

	return 0;
}