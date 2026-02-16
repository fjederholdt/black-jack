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
	m_Hands.push_back(Hand(FirstCard, SecondCard));
	HandleNewHandValueAndEmitSignal(CalculateHandAndReturnValue(m_Hands.back()), false /*Hand size is 5*/);
}

void HandController::NewCard(Card NewCard, bool PlayersHand)
{
	if (PlayersHand == true)
	{
		Hand& CurrentHand = m_Hands.at(m_SplitCounter);
		CurrentHand.NewCard(NewCard);
		HandleNewHandValueAndEmitSignal(CalculateHandAndReturnValue(CurrentHand), (CurrentHand.GetHand().size() == 5));
	}
	else
	{
		Hand& CurrentHand = m_Hands.at(0);
		CurrentHand.NewCard(NewCard);
		HandleNewHandValueAndEmitSignal(CalculateHandAndReturnValue(CurrentHand), (CurrentHand.GetHand().size() == 5));
	}
}

void HandController::SplitHand(Card FirstNewCard, Card SecondNewCard)
{
	m_SplitCounter++;
	Card SplitCard = m_Hands.back().GetHand().at(1);
	m_Hands.back().SplitHand(FirstNewCard);
	AddHand(SplitCard, SecondNewCard, true);
}

void HandController::Stand()
{
	m_SplitCounter--;
}

void HandController::SurrenderHand()
{
	m_Hands.clear();
	m_SplitCounter = 1;
}

void HandController::HandleNewHandValueAndEmitSignal(int NewHandValue, bool HandSizeIs5)
{
	if (NewHandValue > 21)
	{
		emit signal_Bust(NewHandValue);
	}
	else if ((HandSizeIs5 == true) || (NewHandValue == 21))
	{
		emit signal_21();
	}
	else
	{
		emit signal_HandValue(NewHandValue);
	}
}

int HandController::CalculateHandAndReturnValue(Hand hand)
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
	return sum;
}

int HandController::GetNumberOfCardsInHand()
{
	return static_cast<int>(m_Hands.back().GetHand().size());
}

int HandController::GetNumberOfHands()
{
	return m_Hands.size();
}

Hand HandController::GetHand(bool IsPlayersHand)
{
	if (IsPlayersHand == true)
	{
		return m_Hands.at(m_SplitCounter);
	}
	else
	{
		return m_Hands.at(0);
	}
}

Card HandController::GetDealersFirstCard()
{
	return m_Hands.at(0).GetHand().at(1);
}

bool HandController::NoMorePlayerHands()
{
	return m_SplitCounter == 0;
}
