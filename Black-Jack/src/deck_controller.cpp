#include "deck_controller.h"

DeckController::DeckController()
{
}

Card DeckController::GetNewCard()
{
	return m_Deck.TakeCard();
}

void DeckController::ShuffleIfNeeded()
{
	if (m_Deck.DeckCount() < 26)
	{
		m_Deck.ShuffleDeck();
	}
}
