#pragma once

#include "hand_class.h"
#include <QObject>

class HandController : public QObject
{
	Q_OBJECT

public:
	explicit HandController(QObject* pParent = nullptr);
	void AddHand(Card FirstCard, Card SecondCard, bool IsPlayersHand);
	void NewCard(Card NewCard, bool PlayersHand);
	void SplitHand(Card FirstNewCard, Card SecondNewCard);
	void Stand();
	void SurrenderHand();
	void HandleNewHandValueAndEmitSignal(int NewHandValue, bool HandSizeIs5);
	int CalculateHandAndReturnValue(Hand Hand);
	int GetNumberOfCardsInHand();
	int GetNumberOfHands();
	Hand GetHand(bool IsPlayersHand);
	Card GetDealersFirstCard();
	bool NoMorePlayerHands();

signals:
	void signal_NewHand();
	void signal_Bust(int Value);
	void signal_21();
	void signal_HandValue(int Value);
	void signal_EnableSplitButton(bool Enable);

private:
	std::vector<Hand> m_Hands;
	int m_SplitCounter = 1;
};

