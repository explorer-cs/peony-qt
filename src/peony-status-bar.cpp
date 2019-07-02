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

    QLabel *statusLabel = new QLabel(tr("status bar"), this);
    m_label = statusLabel;
    hLayout->addWidget(statusLabel, 0, Qt::AlignCenter);

    setLayout(hLayout);
}

void PeonyStatusBar::updateStatusBarStatus(QString string)
{
    if (m_label != nullptr)
        m_label->setText(string);
}
