#include "dialog.h"

#include <QFont>

Dialog::Dialog(const QString& WindowTitle, const QString& Text, QWidget* parent) : QDialog(parent)
{
    setWindowTitle(WindowTitle);
    setFixedSize(300, 150);

    m_pTextLabel = new QLabel(Text, this);
    QFont font("Arial");
    m_pTextLabel->setFont(font);
    m_pTextLabel->setAlignment(Qt::AlignCenter);
    m_pTextLabel->setWordWrap(true);

    m_pOkButton = new QPushButton("OK", this);

    m_pLayout = new QVBoxLayout(this);
    m_pLayout->addWidget(m_pTextLabel);
    m_pLayout->addWidget(m_pOkButton);
    
    connect(m_pOkButton, &QPushButton::clicked,
        this, &QDialog::accept);
}
