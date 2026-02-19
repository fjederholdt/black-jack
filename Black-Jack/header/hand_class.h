#pragma once

#include "card.h"
#include <vector>

class Hand
{
public:
	Hand(Card FirstCard, Card SecondCard);

	void NewCard(Card NewCard);
	void SplitHand(Card NewCard);
	std::vector<Card>& GetHand();
	bool IsHandSplitable();
	void SetHandIsBust();
	void SetHandValue(int Value);
	void SetHandDoubleDown();
	bool GetHandDoubleDown();
	bool IsHandBust();
	int GetNumberOfAcesInHand();
	int GetHandValue();

private:
	std::vector<Card> m_Hand;
	int m_HandValue = 0;
	bool m_HandIsBust = false;
	int m_NumberOfAcesInHand = 0;
	bool m_HandIsSplitable = false;
	bool m_HandDoubledDown = false;
};

