#pragma once

#include "deck.h"

class DeckController
{
public:
	DeckController();
	Card GetNewCard();
	void ShuffleIfNeeded();

private:
	Deck m_Deck;
};