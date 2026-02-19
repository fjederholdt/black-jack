#pragma once

#include "deck.h"
#include <QObject>

class DeckController : public QObject
{
	Q_OBJECT

public:
	explicit DeckController(QObject* pParent = nullptr);
	Card GetNewCard();
	void ShuffleIfNeeded();

signals:
	void signal_DeckCount(int Count);

private:
	Deck m_Deck;
};