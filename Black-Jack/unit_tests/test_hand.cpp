#include <gtest/gtest.h>
#include "hand_controller.h"

class HandControllerTest : public ::testing::Test
{
protected:
	HandController* pHandController = nullptr;
	void SetUp() override
	{
		pHandController = new HandController();
	}

	void TearDown() override
	{
		delete pHandController;
		pHandController = nullptr;
	}
};
TEST_F(HandControllerTest, TestHandValues)
{
	int HandValue = 0;
	bool Bust = false;
	bool BlackJack = false;
	QObject::connect(pHandController, &HandController::signal_HandValue, [&](int NewValue)
		{
			HandValue = NewValue;
			Bust = false;
			BlackJack = false;
		});

	QObject::connect(pHandController, &HandController::signal_Bust, [&](int Value)
		{
			Bust = true;
			BlackJack = false;
			HandValue = Value;
		});

	QObject::connect(pHandController, &HandController::signal_21, [&]()
		{
			Bust = false;
			BlackJack = true;
			HandValue = 21;
		});

	bool IsPlayerHand = true;
	Card AceHearts = Card(ACE, HEART);
	Card KingHearts = Card(KING, HEART);
	Card FiveHearts = Card(FIVE, HEART);
	Card NineHeart = Card(NINE, HEART);

	pHandController->AddHand(AceHearts, KingHearts, false);// dealers hand
	pHandController->AddHand(AceHearts, KingHearts, IsPlayerHand);
	ASSERT_TRUE(BlackJack);
	ASSERT_FALSE(Bust);
	ASSERT_EQ(HandValue, 21);
	pHandController->NewCard(FiveHearts, IsPlayerHand);
	ASSERT_FALSE(BlackJack);
	ASSERT_FALSE(Bust);
	ASSERT_EQ(HandValue, 16);
	pHandController->NewCard(NineHeart, IsPlayerHand);
	ASSERT_FALSE(BlackJack);
	ASSERT_TRUE(Bust) << HandValue;
	ASSERT_EQ(HandValue, 25);
	pHandController->SurrenderHand();

	pHandController->AddHand(AceHearts, KingHearts, false);// dealers hand
	pHandController->AddHand(AceHearts, AceHearts, IsPlayerHand);
	ASSERT_FALSE(BlackJack);
	ASSERT_FALSE(Bust);
	ASSERT_EQ(HandValue, 12);
	pHandController->NewCard(NineHeart, IsPlayerHand);
	ASSERT_EQ(HandValue, 21);
	ASSERT_TRUE(BlackJack);
	ASSERT_FALSE(Bust);
	pHandController->SurrenderHand();

	pHandController->AddHand(AceHearts, KingHearts, false);// dealers hand
	pHandController->AddHand(FiveHearts, NineHeart, IsPlayerHand);
	ASSERT_FALSE(BlackJack);
	ASSERT_FALSE(Bust);
	ASSERT_EQ(HandValue, 14);
	pHandController->NewCard(AceHearts, IsPlayerHand);
	ASSERT_FALSE(BlackJack);
	ASSERT_FALSE(Bust);
	ASSERT_EQ(HandValue, 15);
	pHandController->NewCard(KingHearts, IsPlayerHand);
	ASSERT_FALSE(BlackJack);
	ASSERT_TRUE(Bust);
	ASSERT_EQ(HandValue, 25);
}