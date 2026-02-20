#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

#include <iostream>
#include <QPalette>
#include <QLabel>
#include <QDialog>


MainWindow::MainWindow(QWidget* pParent) : QMainWindow(pParent), pUi(new Ui::MainWindow)
{
	pUi->setupUi(this);

	m_CardSize = QSize(125, 200);
	m_pCardBacksidePicture = new QLabel("Deck", this);
	m_pCardBacksidePicture->setFixedSize(m_CardSize);
	m_pCardBacksidePicture->setScaledContents(true);
	m_pDeckController = new DeckController(this);
	m_pHandController = new HandController(this);

	QPalette pal = pUi->centralwidget->palette();

	// set black background
	pal.setColor(QPalette::Window, Qt::darkGreen);
	pUi->centralwidget->setAutoFillBackground(true);
	pUi->centralwidget->setPalette(pal);
	pUi->centralwidget->show();

	pUi->DoubleDownButton->setStyleSheet("background-color:black;");
	pUi->DoubleDownButton->show();
	pUi->NewHandButton->setStyleSheet("background-color:black;");
	pUi->NewHandButton->show();
	pUi->HitButton->setStyleSheet("background-color:black;");
	pUi->HitButton->show();
	pUi->SplitButton->setStyleSheet("background-color:black;");
	pUi->SplitButton->show();
	pUi->SurrenderButton->setStyleSheet("background-color:black;");
	pUi->SurrenderButton->show();
	pUi->StandButton->setStyleSheet("background-color:black;");
	pUi->StandButton->show();
	DisableButtons(true);
	pUi->SplitButton->setDisabled(true);
	pUi->Saldo->setText(QString::number(m_Saldo));
	pUi->BetSpinBox->setMaximum(m_Saldo);
	pUi->BetSpinBox->setMinimum(1);
	SetActiveHandLabel(0);

	Connect();
	m_pDeckController->ShuffleIfNeeded();
	std::filesystem::path currentPath = std::filesystem::current_path();
	try {
		bool foundFolder = false;
		for (const auto& entry : std::filesystem::recursive_directory_iterator(currentPath)) {
			if (entry.is_directory() && entry.path().filename() == "spillekort") {
				std::filesystem::path backPicturePath = entry.path();
				backPicturePath += std::filesystem::path("\\Backside.png");
				QPixmap pixmap(QString::fromStdString(backPicturePath.string()));

				if (!pixmap.isNull()) {
					m_pCardBacksidePicture->setPixmap(pixmap);
					pUi->DeckLayout->addWidget(m_pCardBacksidePicture);
				}
				else
				{
					qDebug() << "Kunne ikke loade billede:" << QString::fromStdString(backPicturePath.string());
				}
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

void MainWindow::Connect()
{
	connect(pUi->HitButton, &QPushButton::clicked, this, &MainWindow::HitButtonClicked);
	connect(pUi->SplitButton, &QPushButton::clicked, this, &MainWindow::SplitButtonClicked);
	connect(pUi->StandButton, &QPushButton::clicked, this, &MainWindow::StandButtonClicked);
	connect(pUi->DoubleDownButton, &QPushButton::clicked, this, &MainWindow::DoubleDownButtonClicked);
	connect(pUi->SurrenderButton, &QPushButton::clicked, this, &MainWindow::SurrenderButtonClicked);
	connect(pUi->NewHandButton, &QPushButton::clicked, this, &MainWindow::NewHandButtonClicked);
	connect(m_pDeckController, &DeckController::signal_DeckCount, this, &MainWindow::OnDeckCountChanged);
	connect(m_pHandController, &HandController::signal_HandValue, this, &MainWindow::OnNewHandValue);
	connect(m_pHandController, &HandController::signal_EnableSplitButton, this, &MainWindow::OnSplitEnabled);
}

void MainWindow::CreateAndShowCardPictureLabel(std::string CardPicturePath, std::string CardPictureName)
{
	QLabel* pCardPictureLabel = new QLabel(this);
	pCardPictureLabel->setObjectName(QString::fromStdString(CardPictureName));
	pCardPictureLabel->setFixedSize(m_CardSize);
	pCardPictureLabel->setScaledContents(true);
	QPixmap pixmap(QString::fromStdString(CardPicturePath));

	if (!pixmap.isNull()) {
		pCardPictureLabel->setPixmap(pixmap);
	}
	else
	{
		qDebug() << "Kunne ikke loade billede:" << QString::fromStdString(CardPicturePath);
	}
	if (m_IsPlayerHand == true)
	{
		SetActiveHandLabel(m_CurrentPlayerHand);
		switch (m_CurrentPlayerHand)
		{
		case 0:
		{
			break;
		}
		case 1:
		{
			pUi->HandOneLayout->addWidget(pCardPictureLabel);
			break;
		}
		case 2:
		{
			pUi->HandTwoLayout->addWidget(pCardPictureLabel);
			break;
		}
		case 3:
		{
			pUi->HandThreeLayout->addWidget(pCardPictureLabel);
			break;
		}
		case 4:
		{
			pUi->HandFourLayout->addWidget(pCardPictureLabel);
			break;
		}
		default:
			break;
		}
		m_PlayerCardPicture.push_back(pCardPictureLabel);
	}
	else
	{
		pUi->DealerLayout->addWidget(pCardPictureLabel);
		m_DealerCardPicture.push_back(pCardPictureLabel);
	}
}


void MainWindow::HitButtonClicked()
{
	m_pHandController->NewCard(GetAndShowNewCard(), m_IsPlayerHand);
}

void MainWindow::SplitButtonClicked()
{
	m_Saldo -= m_BetValue;
	pUi->Saldo->setText(QString::number(m_Saldo));
	m_BetWinnings += m_BetValue;
	Card FirstHandCard = m_pDeckController->GetNewCard();
	Card SecondHandCard = m_pDeckController->GetNewCard();
	PerformSplitOfPlayerHand(FirstHandCard, SecondHandCard);
	m_pHandController->SplitHand(FirstHandCard, SecondHandCard);
}

void MainWindow::StandButtonClicked()
{
	SetActiveHandLabel(--m_CurrentPlayerHand);
	m_pHandController->Stand();
	if (m_pHandController->NoMorePlayerHands() == true)
	{
		m_IsPlayerHand = false;
		DisableButtons(true);
		pUi->SplitButton->setDisabled(true);
		FlipDealersFirstCard();
		OnNewHandValue(m_DealerHandValue);
	}
	else
	{
		m_pHandController->CalculateHandAndEmitValue(m_pHandController->GetHand(m_CurrentPlayerHand));
	}
}

void MainWindow::DoubleDownButtonClicked()
{
	m_Saldo -= m_BetValue;
	pUi->Saldo->setText(QString::number(m_Saldo));
	m_BetWinnings += m_BetValue;
	int HandToDoubleDownAndCheckBust = m_CurrentPlayerHand;
	m_pHandController->SetHandDoubleDown(HandToDoubleDownAndCheckBust);
	HitButtonClicked();
	if ((m_pHandController->GetNumberOfHands() > 1) && (m_pHandController->GetIsHandBust(HandToDoubleDownAndCheckBust) == false) && (m_CurrentPlayerHandValue != 21))
	{
		StandButtonClicked();
	}
}

void MainWindow::NewHandButtonClicked()
{
	m_pDeckController->ShuffleIfNeeded();
	pUi->NewHandButton->setDisabled(true);
	m_CurrentPlayerHand = 1;
	m_CurrentPlayerHandValue = 0;
	m_DealerHandValue = 0;
	m_BetValue = pUi->BetSpinBox->value();
	m_Saldo -= m_BetValue;
	pUi->Saldo->setText(QString::number(m_Saldo));
	pUi->BetSpinBox->setDisabled(true);
	m_BetWinnings = m_BetValue;
	DisableButtons(false);
	m_FirstDealerCard = true;
	m_IsPlayerHand = false;
	m_pHandController->AddHand(GetAndShowNewCard(), GetAndShowNewCard(), m_IsPlayerHand);

	m_IsPlayerHand = true;
	m_pHandController->AddHand(GetAndShowNewCard(), GetAndShowNewCard(), m_IsPlayerHand);
}

void MainWindow::SurrenderButtonClicked()
{
	Surrender(true);
}

void MainWindow::Surrender(bool FromButton)
{
	if (FromButton == false)
	{
		m_Saldo += m_BetWinnings * 2;
		pUi->Saldo->setText(QString::number(m_Saldo));
	}
	m_BetValue = 0;
	m_BetWinnings = 0;
	pUi->BetSpinBox->setDisabled(false);
	pUi->BetSpinBox->setMaximum(m_Saldo);
	pUi->BetSpinBox->setValue(1);
	m_pHandController->SurrenderHand();
	for (size_t i = 0; i < m_PlayerCardPicture.size(); i++)
	{
		if (m_PlayerCardPicture[i] != nullptr)
		{
			pUi->PlayerLayout->removeWidget(m_PlayerCardPicture[i]);
			m_PlayerCardPicture[i]->deleteLater();
			m_PlayerCardPicture[i] = nullptr;
		}
	}
	m_PlayerCardPicture.clear();
	for (size_t i = 0; i < m_DealerCardPicture.size(); i++)
	{
		if (m_DealerCardPicture[i] != nullptr)
		{
			pUi->DealerLayout->removeWidget(m_DealerCardPicture[i]);
			m_DealerCardPicture[i]->deleteLater();
			m_DealerCardPicture[i] = nullptr;
		}
	}
	m_DealerCardPicture.clear();
	m_IsPlayerHand = true;
	DisableButtons(true);
	pUi->NewHandButton->setDisabled(false);
	m_CurrentPlayerHandValue = 0;
	m_MaxPlayerHandValue = 0;
	m_DealerHandValue = 0;
	pUi->HandOneLabel->setVisible(false);
	pUi->HandTwoLabel->setVisible(false);
	pUi->HandThreeLabel->setVisible(false);
	pUi->HandFourLabel->setVisible(false);
}

void MainWindow::OnDeckCountChanged(int Count)
{
	pUi->DeckCount->setText(QString::number(Count));
}

void MainWindow::OnBust(int Value)
{
	if (m_IsPlayerHand == true)
	{
		m_pHandController->SetHandIsBust(m_CurrentPlayerHand);
		StandButtonClicked();
	}
	else
	{
		const QString Text = "Dealer bust, You won: " + QString::number(m_BetWinnings * 2);
		Dialog PlayerWonDialog("Player Won!", Text, this);
		PlayerWonDialog.exec();
	}
}

void MainWindow::On21()
{
	const QString Text = QString("Player got 21 on hand: ") + QString::number(m_CurrentPlayerHand);
	Dialog Player21Dialog("Player 21", Text, this);
	Player21Dialog.exec();
	StandButtonClicked();
}

void MainWindow::OnNewHandValue(int Value)
{
	int NumberOfHands = m_pHandController->GetNumberOfHands();
	if (m_IsPlayerHand == true)
	{
		m_CurrentPlayerHandValue = Value;
		if (m_CurrentPlayerHandValue > 21)
		{
			OnBust(m_CurrentPlayerHandValue);
		}
		else if (m_CurrentPlayerHandValue == 21)
		{
			On21();
		}
	}
	else
	{
		m_DealerHandValue = Value;
		if (NumberOfHands > 1)
		{

			// First check if player went bust on first and only hand
			if ((NumberOfHands == 2) && (m_pHandController->GetIsHandBust(1) == true))
			{
				const QString Text = QString("Player went bust with value: ") + QString::number(m_CurrentPlayerHandValue);
				Dialog PlayerBustDialog("Player bust", Text, this);
				PlayerBustDialog.exec();
			}
			else
			{
				// Check Dealer 21
				if (m_DealerHandValue == 21)
				{
					const QString Text = "Dealer got 21";
					Dialog DealerWonDialog("Dealer won", Text, this);
					DealerWonDialog.exec();
					m_BetWinnings = 0;
				}
				// Check if Dealer hand is less than 17
				else if (m_DealerHandValue < 17)
				{
					// We gamble! Dealer gets a new card
					m_pHandController->NewCard(GetAndShowNewCard(), m_IsPlayerHand);
				}
				else
				{
					bool DealerBust = (m_DealerHandValue > 21);
					bool AtleastOneHandWon = false;
					for (int i = 1; i < NumberOfHands; i++)
					{
						int DoubleDown = m_pHandController->GetHandDoubleDown(i) == true ? 2 : 1;
						int HandValue = m_pHandController->GetHandValue(i);
						if (m_pHandController->GetIsHandBust(i) == true)
						{
							const QString Text = QString("Player hand ") + QString::number(i) + " went bust with value: " + QString::number(HandValue);
							Dialog PlayerBustDialog("Player bust", Text, this);
							PlayerBustDialog.exec();
							m_BetWinnings -= m_BetValue * DoubleDown;
							continue;
						}
						else if ((HandValue <= m_DealerHandValue) && (DealerBust == false))
						{
							const QString Text = "Dealer won with: " + QString::number(m_DealerHandValue);
							const QString Text2 = " against players hand " + QString::number(i) + " with value: " + QString::number(HandValue);
							Dialog DealerWonDialog("Dealer won", Text + Text2, this);
							DealerWonDialog.exec();
							m_BetWinnings -= m_BetValue * DoubleDown;
						}
						else
						{
							AtleastOneHandWon = true;
						}
					}
					if (AtleastOneHandWon == true)
					{
						if (DealerBust == true)
						{
							OnBust(m_DealerHandValue);
						}
						else
						{
							const QString Text = "Congratulation you won: " + QString::number(m_BetWinnings * 2);
							Dialog PlayerWonDialog("Player won!", Text, this);
							PlayerWonDialog.exec();
						}
					}
				}
			}
			Surrender();
		}
	}
}

void MainWindow::OnSplitEnabled(bool Enable)
{
	pUi->SplitButton->setEnabled(Enable);
}

Card MainWindow::GetAndShowNewCard()
{
	Card NewCard = m_pDeckController->GetNewCard();
	if ((m_IsPlayerHand == false) && (m_pHandController->GetNumberOfHands() == 0) && (m_FirstDealerCard == true))
	{
		Card BackSide = Card();
		CreateAndShowCardPictureLabel(BackSide.GetCardPicturePath().string(), BackSide.GetCardPictureName());
		m_FirstDealerCard = false;
	}
	else
	{
		CreateAndShowCardPictureLabel(NewCard.GetCardPicturePath().string(), NewCard.GetCardPictureName());
	}
	return NewCard;
}

void MainWindow::DisableButtons(bool Disable)
{
	pUi->HitButton->setDisabled(Disable);
	pUi->DoubleDownButton->setDisabled(Disable);
	pUi->StandButton->setDisabled(Disable);
	pUi->SurrenderButton->setDisabled(Disable);
}

void MainWindow::SetActiveHandLabel(int HandNumber)
{
	switch (HandNumber)
	{
	case 0:
	{
		pUi->HandOneLabel->setVisible(false);
		pUi->HandTwoLabel->setVisible(false);
		pUi->HandThreeLabel->setVisible(false);
		pUi->HandFourLabel->setVisible(false);
		break;
	}
	case 1:
	{
		pUi->HandOneLabel->setVisible(true);
		pUi->HandTwoLabel->setVisible(false);
		pUi->HandThreeLabel->setVisible(false);
		pUi->HandFourLabel->setVisible(false);
		break;
	}
	case 2:
	{
		pUi->HandOneLabel->setVisible(false);
		pUi->HandTwoLabel->setVisible(true);
		pUi->HandThreeLabel->setVisible(false);
		pUi->HandFourLabel->setVisible(false);
		break;
	}
	case 3:
	{
		pUi->HandOneLabel->setVisible(false);
		pUi->HandTwoLabel->setVisible(false);
		pUi->HandThreeLabel->setVisible(true);
		pUi->HandFourLabel->setVisible(false);
		break;
	}
	case 4:
	{
		pUi->HandOneLabel->setVisible(false);
		pUi->HandTwoLabel->setVisible(false);
		pUi->HandThreeLabel->setVisible(false);
		pUi->HandFourLabel->setVisible(true);
		break;
	}
	default:
		break;
	}
}

void MainWindow::FlipDealersFirstCard()
{
	Card DealersFirstCard = m_pHandController->GetDealersFirstCard();
	ReplaceCardPictureInLayout(pUi->DealerLayout, "Backside", DealersFirstCard.GetCardPictureName(), DealersFirstCard.GetCardPicturePath().string());
}

void MainWindow::PerformSplitOfPlayerHand(Card FirstHandSecondCard, Card SecondHandSecondCard)
{
	Hand PlayerHand = m_pHandController->GetHand(true);
	Card SecondHandFirstCard = PlayerHand.GetHand().at(1);
	ReplaceCardPictureInActiveHandLayout(SecondHandFirstCard.GetCardPictureName(), FirstHandSecondCard.GetCardPictureName(), FirstHandSecondCard.GetCardPicturePath().string());
	m_CurrentPlayerHand++;
	QSpacerItem* pHSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	pUi->PlayerLayout->addItem(pHSpacer);
	CreateAndShowCardPictureLabel(SecondHandFirstCard.GetCardPicturePath().string(), SecondHandFirstCard.GetCardPictureName());
	CreateAndShowCardPictureLabel(SecondHandSecondCard.GetCardPicturePath().string(), SecondHandSecondCard.GetCardPictureName());
}

void MainWindow::ReplaceCardPictureInActiveHandLayout(std::string OldCardLabelName, std::string NewCardLabelName, std::string NewCardPicturePath)
{
	switch (m_CurrentPlayerHand)
	{
	case 0:
	{
		break;
	}
	case 1:
	{
		ReplaceCardPictureInLayout(pUi->HandOneLayout, OldCardLabelName, NewCardLabelName, NewCardPicturePath);
		break;
	}
	case 2:
	{
		ReplaceCardPictureInLayout(pUi->HandTwoLayout, OldCardLabelName, NewCardLabelName, NewCardPicturePath);
		break;
	}
	case 3:
	{
		ReplaceCardPictureInLayout(pUi->HandThreeLayout, OldCardLabelName, NewCardLabelName, NewCardPicturePath);
		break;
	}
	case 4:
	{
		ReplaceCardPictureInLayout(pUi->HandFourLayout, OldCardLabelName, NewCardLabelName, NewCardPicturePath);
		break;
	}
	default:
		break;
	}

}

void MainWindow::ReplaceCardPictureInLayout(QLayout* pLayout, std::string OldCardLabelName, std::string NewCardLabelName, std::string NewCardPicturePath)
{
	for (int i = 0; i < pLayout->count(); ++i)
	{
		QLayoutItem* item = pLayout->itemAt(i);
		if (!item)
			continue;

		QWidget* widget = item->widget();
		if (!widget)
			continue;

		QLabel* label = qobject_cast<QLabel*>(widget);
		if (label && label->objectName() == OldCardLabelName)
		{
			label->setObjectName(NewCardLabelName);
			QPixmap pixmap(QString::fromStdString(NewCardPicturePath));

			if (!pixmap.isNull()) {
				label->setPixmap(pixmap);
			}
			else
			{
				qDebug() << "Kunne ikke loade billede:" << QString::fromStdString(NewCardPicturePath);
			}
			break;
		}
	}
}
