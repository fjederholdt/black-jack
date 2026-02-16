#pragma once

#include "card.h"
#include <random>
#include <functional>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>

class Deck
{
public:
	Deck();

	Card TakeCard();
	void ShuffleDeck();
	int DeckCount();
	bool Empty();

private:
	std::vector<Card> m_Deck;
};
