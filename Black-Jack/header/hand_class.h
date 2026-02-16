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
	int GetNumberOfAcesInHand();

private:
	std::vector<Card> m_Hand;
	int m_NumberOfAcesInHand = 0;
	bool m_HandIsSplitable;
};

