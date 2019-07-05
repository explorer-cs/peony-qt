#include "peony-status-bar.h"

#include <QHBoxLayout>
#include <QLabel>

PeonyStatusBar::PeonyStatusBar(QWidget *parent) : QWidget(parent)
{

}

void PeonyStatusBar::createStatusBar()
{
    if (is_status_bar_created)
        return;

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setMargin(0);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setAlignment(Qt::AlignTop);

    QLabel *statusLabel = new QLabel(tr("status bar"), this);
    m_label = statusLabel;
    statusLabel->setMargin(0);
    statusLabel->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(statusLabel, 0, Qt::AlignHCenter);
    setFixedHeight(28);

    setLayout(hLayout);
}

void PeonyStatusBar::updateStatusBarStatus(QString string)
{
    if (m_label != nullptr)
        m_label->setText(string);
    if (string == "/")
        m_label->setText("");
}
