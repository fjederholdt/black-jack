#include "deck_controller.h"

DeckController::DeckController(QObject* pParent) : QObject(pParent)
{
}

Card DeckController::GetNewCard()
{
	emit signal_DeckCount(m_Deck.DeckCount() - 1);
	return m_Deck.TakeCard();
}

void DeckController::ShuffleIfNeeded()
{
	if (m_Deck.DeckCount() <= 26)
	{
		m_Deck.ShuffleDeck();
	}
	emit signal_DeckCount(m_Deck.DeckCount());
}
