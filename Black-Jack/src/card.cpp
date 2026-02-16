#include "card.h"

#include <iostream>
#include <string>

Card::Card(CardName Name, CardType CardType) : m_Name(Name), m_CardType(CardType), m_Value(0)
{
	switch (m_CardType)
	{
	case CardType::HEART:
	{
		m_CardPictureName = "H" + std::to_string(static_cast<int>(m_Name) + 1);
		break;
	}
	case CardType::CLUB:
	{
		m_CardPictureName = "K" + std::to_string(static_cast<int>(m_Name) + 1);
		break;
	}
	case CardType::DIAMOND:
	{
		m_CardPictureName = "D" + std::to_string(static_cast<int>(m_Name) + 1);
		break;
	}
	case CardType::SPADE:
	{
		m_CardPictureName = "S" + std::to_string(static_cast<int>(m_Name) + 1);
		break;
	}
	default:
		break;
	}

	switch (m_Name)
	{
	case CardName::ACE:
	{
		m_Value = 11;
		break;
	}
	case CardName::TWO:
	{
		m_Value = 2;
		break;
	}
	case CardName::THREE:
	{
		m_Value = 3;
		break;
	}
	case CardName::FOUR:
	{
		m_Value = 4;
		break;
	}
	case CardName::FIVE:
	{
		m_Value = 5;
		break;
	}
	case CardName::SIX:
	{
		m_Value = 6;
		break;
	}
	case CardName::SEVEN:
	{
		m_Value = 7;
		break;
	}
	case CardName::EIGHT:
	{
		m_Value = 8;
		break;
	}
	case CardName::NINE:
	{
		m_Value = 9;
		break;
	}
	case CardName::TEN:
	case CardName::JACK:
	case CardName::QUEEN:
	case CardName::KING:
	{
		m_Value = 10;
		break;
	}
	}
	FindCardPicturePath();
}

Card::Card() 
{
	m_Value = 0;
	m_CardPictureName = "Backside";
	FindCardPicturePath();
}

CardName Card::GetCardName()
{
	return m_Name;
}

CardType Card::GetCardType()
{
	return m_CardType;
}

int Card::GetCardValue()
{
	return m_Value;
}

std::filesystem::path Card::GetCardPicturePath()
{
	return m_CardPicturePath;
}

std::string Card::GetCardPictureName()
{
	return m_CardPictureName;
}

void Card::FindCardPicturePath()
{
	std::filesystem::path currentPath = std::filesystem::current_path();
	if (currentPath.filename() == "Debug")
	{
		currentPath = currentPath.parent_path();
	}
	try {
		bool foundFolder = false;
		for (const auto& entry : std::filesystem::recursive_directory_iterator(currentPath)) {
			if (entry.is_directory() && entry.path().filename() == "spillekort") {
				m_CardPicturePath = entry.path();
				m_CardPicturePath += std::filesystem::path("\\" + GetCardPictureName() + ".png");
				foundFolder = true;
				break;
			}
		}
		if (foundFolder == false)
		{
			std::cout << "Mappen 'spillekort' blev ikke fundet." << std::endl;
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Filesystem fejl: " << e.what() << std::endl;
	}
}
