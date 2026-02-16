#include "deck.h"


Deck::Deck()
{
	ShuffleDeck();
}

Card Deck::TakeCard()
{
	Card NewCard = m_Deck.back();
	m_Deck.pop_back();
	return NewCard;
}

void Deck::ShuffleDeck()
{
	m_Deck.clear();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			m_Deck.push_back(Card(static_cast<CardName>(j), static_cast<CardType>(i)));
		}
	}

	static std::random_device rd;
	static std::mt19937 generator(rd());
	std::shuffle(m_Deck.begin(), m_Deck.end(), generator);
}

int Deck::DeckCount()
{
	return static_cast<int>(m_Deck.size());
}

bool Deck::Empty()
{
	return m_Deck.empty();
}
