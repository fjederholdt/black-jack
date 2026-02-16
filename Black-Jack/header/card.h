#pragma once

#include <filesystem>

enum CardType
{
	HEART,
	CLUB,
	DIAMOND,
	SPADE
};

enum CardName
{
	ACE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING
};

class Card
{
public:
	Card(CardName Name, CardType CardType);
	Card();

	CardName GetCardName();
	CardType GetCardType();
	int GetCardValue();
	std::string GetCardPictureName();
	std::filesystem::path GetCardPicturePath();

private:
	void FindCardPicturePath();

private:
	CardName m_Name;
	int m_Value;
	CardType m_CardType;
	std::string m_CardPictureName;
	std::filesystem::path m_CardPicturePath;
};

