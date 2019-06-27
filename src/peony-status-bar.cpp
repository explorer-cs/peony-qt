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

    QLabel *statusLabel = new QLabel("status bar", this);
    hLayout->addWidget(statusLabel, 0, Qt::AlignCenter);

    setLayout(hLayout);
}
