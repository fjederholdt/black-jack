#pragma once

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(const QString& WindowTitle, const QString& Text, QWidget* parent = nullptr);

private:
    QLabel* m_pTextLabel;
    QVBoxLayout* m_pLayout;
    QPushButton* m_pOkButton;
};