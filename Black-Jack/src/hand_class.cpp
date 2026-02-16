#include "hand_class.h"

Hand::Hand(Card FirstCard, Card SecondCard)
{
	if (FirstCard.GetCardName() == ACE)
	{
		m_NumberOfAcesInHand++;
	}
	if (SecondCard.GetCardName() == ACE)
	{
		m_NumberOfAcesInHand++;
	}
	m_HandIsSplitable = (FirstCard.GetCardName() == SecondCard.GetCardName()) ? true : false;
	
	m_Hand.push_back(FirstCard);
	m_Hand.push_back(SecondCard);
}

void Hand::NewCard(Card NewCard)
{
	if (NewCard.GetCardName() == ACE)
	{
		m_NumberOfAcesInHand++;
	}
	m_Hand.push_back(NewCard);
}

void Hand::SplitHand(Card NewCard)
{
	if (m_Hand.back().GetCardName() == ACE)
	{
		m_NumberOfAcesInHand--;
	}
	if (NewCard.GetCardName() == ACE)
	{
		m_NumberOfAcesInHand++;
	}
	m_Hand.pop_back();
	m_HandIsSplitable = (m_Hand.back().GetCardName() == NewCard.GetCardName()) ? true : false;
	m_Hand.push_back(NewCard);
}

std::vector<Card>& Hand::GetHand()
{
	return m_Hand;
}

bool Hand::IsHandSplitable()
{
	return m_HandIsSplitable;
}

int Hand::GetNumberOfAcesInHand()
{
	return m_NumberOfAcesInHand;
}
