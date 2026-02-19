#include "hand_controller.h"

HandController::HandController(QObject* pParent) : QObject(pParent)
{
}

void HandController::AddHand(Card FirstCard, Card SecondCard, bool IsPlayersHand)
{
	Hand NewHand = Hand(FirstCard, SecondCard);
	if (IsPlayersHand == true)
	{
		emit signal_EnableSplitButton(NewHand.IsHandSplitable());
	}
	else
	{
		emit signal_EnableSplitButton(false);
	}
	m_Hands.push_back(NewHand);
	CalculateHandAndEmitValue(m_Hands.back());
}

void HandController::NewCard(Card NewCard, bool PlayersHand)
{
	int HandNumber = (PlayersHand == true) ? m_PlayerHandCounter : 0;
	Hand& CurrentHand = m_Hands.at(HandNumber);
	CurrentHand.NewCard(NewCard);
	CalculateHandAndEmitValue(CurrentHand);
}

void HandController::SplitHand(Card FirstNewCard, Card SecondNewCard)
{
	m_PlayerHandCounter++;
	Card SplitCard = m_Hands.back().GetHand().at(1);
	m_Hands.back().SplitHand(FirstNewCard);
	CalculateHandAndEmitValue(m_Hands.back());
	AddHand(SplitCard, SecondNewCard, true);
}

void HandController::Stand()
{
	if (m_PlayerHandCounter > 0)
	{
		m_PlayerHandCounter--;
	}
	if (m_PlayerHandCounter > 0)
	{
		emit signal_EnableSplitButton(m_Hands.at(m_PlayerHandCounter).IsHandSplitable());
	}
}

void HandController::SurrenderHand()
{
	m_Hands.clear();
	m_PlayerHandCounter = 1;
}

void HandController::SetHandIsBust(int Hand)
{
	m_Hands.at(Hand).SetHandIsBust();
}

void HandController::SetHandDoubleDown(int Hand)
{
	m_Hands.at(Hand).SetHandDoubleDown();
}

void HandController::CalculateHandAndEmitValue(Hand& hand)
{
	std::vector<Card>& CalculateHand = hand.GetHand();
	int sum = 0;
	for (int i = 0; i < CalculateHand.size(); i++)
	{
		sum += CalculateHand.at(i).GetCardValue();
	}
	for (int i = 0; i < hand.GetNumberOfAcesInHand(); i++)
	{
		if (sum > 21) 
		{
			sum -= 10;
		}
	}
	if ((sum < 21) && (CalculateHand.size() == 5))
	{
		sum = 21;
	}
	hand.SetHandValue(sum);
	emit signal_HandValue(sum);
}

int HandController::GetNumberOfCardsInHand()
{
	return static_cast<int>(m_Hands.back().GetHand().size());
}

int HandController::GetNumberOfHands()
{
	return m_Hands.size();
}

int HandController::GetHandValue(int Hand)
{
	return m_Hands.at(Hand).GetHandValue();
}

bool HandController::GetHandDoubleDown(int Hand)
{
	return m_Hands.at(Hand).GetHandDoubleDown();
}

bool HandController::GetIsHandBust(int Hand)
{
	return m_Hands.at(Hand).IsHandBust();
}

Hand& HandController::GetHand(int Hand)
{
	return m_Hands.at(Hand);
}

Card HandController::GetDealersFirstCard()
{
	return m_Hands.at(0).GetHand().at(1);
}

bool HandController::NoMorePlayerHands()
{
	return m_PlayerHandCounter == 0;
}
