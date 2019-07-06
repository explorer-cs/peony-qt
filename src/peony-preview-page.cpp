#include "peony-preview-page.h"

#include <QHBoxLayout>
#include <QLabel>

PeonyPreviewPage::PeonyPreviewPage(QWidget *parent) : QWidget(parent)
{
    m_layout = new QHBoxLayout(this);
    m_layout->setMargin(0);
    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setAlignment(Qt::AlignCenter);
    setLayout(m_layout);

    defaultView = new QLabel(tr("select file you want to preview"), this);
    m_layout->addWidget(defaultView);
}

void PeonyPreviewPage::preview(QString filePath, PeonyPreviewPage::Type type)
{
    //it will be fill with other different view by type.
    //there is nothing now.
    static_cast<QLabel*>(defaultView)->setText(filePath);
}

void PeonyPreviewPage::setDefault()
{
    static_cast<QLabel*>(defaultView)->setText(tr("select file you want to preview"));
    defaultView->show();
}
