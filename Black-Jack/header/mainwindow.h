#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <qmainwindow.h>
#include <QLabel>
#include <atomic>

#include "hand_controller.h"
#include "deck_controller.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* pParent = nullptr);
    void Connect();

signals:
    void signal_Hit();
    void signal_Split();
    void signal_DoubleDown();
    void signal_NewHand();
    void signal_Stand();
    void signal_Surrender();
    void signal_CreateAndShowCard(std::string CardPictureName);

private slots:
    void CreateAndShowCardPictureLabel(std::string CardPicturePath, std::string CardPictureName);
    void HitButtonClicked();
    void SplitButtonClicked();
    void StandButtonClicked();
    void DoubleDownButtonClicked();
    void NewHandButtonClicked();
    void SurrenderButtonClicked();
    void OnDeckCountChanged(int Count);
    void OnNewHandValue(int Value);
    void OnSplitEnabled(bool Enable);

private:
    void OnBust(int Value);
    void On21();
    Card GetAndShowNewCard();
    void Surrender(bool FromButton = false);
    void DisableButtons(bool Disable);
    void SetActiveHandLabel(int HandNumber);
    void FlipDealersFirstCard();
    void PerformSplitOfPlayerHand(Card FirstHandSecondCard, Card SecondHandSecondCard);
    void ReplaceCardPictureInLayout(QLayout* pLayout, std::string CardLabelName, std::string NewCardLabelName, std::string CardPicturePath);
    void ReplaceCardPictureInActiveHandLayout(std::string OldCardLabelName, std::string NewCardLabelName, std::string NewCardPicturePath);

private:
    Ui::MainWindow* pUi;
    HandController* m_pHandController;
    DeckController* m_pDeckController;
    std::vector<QLabel*> m_PlayerCardPicture;
    std::vector<QLabel*> m_DealerCardPicture;
    int m_BetValue = 0;
    int m_BetWinnings = 0;
    int m_CurrentPlayerHandValue = 0;
    int m_MaxPlayerHandValue = 0;
    std::vector<int> m_PlayerHandValues;
    int m_DealerHandValue = 0;
    std::atomic<bool> m_IsPlayerHand = true;
    bool m_FirstDealerCard = true;
    QLabel* m_pCardBacksidePicture;
    QSize m_CardSize;
    int m_CurrentPlayerHand = 0;
    int m_Saldo = 100;
};

#endif // MAINWINDOW_H