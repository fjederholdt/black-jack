#include <gtest/gtest.h>
#include <string>
#include <set>
#include "deck.h"
#include "card.h"

TEST(Test_Deck, NoDuplicateCards)
{
	Deck deck = Deck();
	ASSERT_EQ(deck.DeckCount(), 52);

	std::set<std::pair<CardName, CardType>> seen;
	while (deck.Empty() == false)
	{
		Card card = deck.TakeCard();
		auto key = std::make_pair(card.GetCardName(), card.GetCardType());
		ASSERT_TRUE(seen.find(key) == seen.end()) << "Duplicate card found!";
		seen.insert(key);
	}

	EXPECT_EQ(seen.size(), 52);
}

TEST(Test_Deck, NoTwoDecksAreEqual)
{
	bool SameDeck = true;
	for (int i = 0; i < 100; i++)
	{
		SameDeck = true;
		Deck deckOne = Deck();
		Deck deckTwo = Deck();
		for (size_t j = 0; j < 52; j++)
		{
			if (deckOne.TakeCard().GetCardPictureName() != deckTwo.TakeCard().GetCardPictureName())
			{
				SameDeck = false;
				break;
			}
		}
	}
	ASSERT_FALSE(SameDeck);
}