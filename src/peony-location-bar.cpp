#include "peony-location-bar.h"

#include <QHBoxLayout>

#include <QToolBar>
#include <QPushButton>

#include <QMenu>
#include <QAction>
#include <QIcon>

#include <QLineEdit>

#include <libfm-qt/pathbar.h>

PeonyLocationBar::PeonyLocationBar(QWidget *parent) : QWidget(parent)
{

}

void PeonyLocationBar::createLocationBar()
{
    if (is_location_search_bar_created)
        return;

    QHBoxLayout *hLayout = new QHBoxLayout(this);

    QToolBar *navigationToolBar = new QToolBar(this);

    //navigation tool bar need synchronous the buttons' states, connect the history manager signal.

    QAction *goBackAction = new QAction(QIcon::fromTheme("gtk-go-back"), tr("Go Back"), navigationToolBar);
    connect(goBackAction, &QAction::triggered, [=](){Q_EMIT this->backRequest();});
    navigationToolBar->addAction(goBackAction);

    QAction *goForwardAction = new QAction(QIcon::fromTheme("gtk-go-forward"), tr("Go Forward"), navigationToolBar);
    connect(goForwardAction, &QAction::triggered, this, &PeonyLocationBar::forwardRequest);
    navigationToolBar->addAction(goForwardAction);

    QAction *listHistoryAction = new QAction(QIcon::fromTheme("gtk-go-down"), tr("List History"), navigationToolBar);
    QMenu *historyMenu = new QMenu(this);
    connect(listHistoryAction, &QAction::triggered, [=](){
        Q_EMIT this->historyMenuRequest(listHistoryAction, historyMenu);
    });
    navigationToolBar->addAction(listHistoryAction);

    //we must ensure that cd Up Action is disable in location "/"
    QAction *cdUpAction = new QAction(QIcon::fromTheme("gtk-go-up"), tr("cd Up"), navigationToolBar);
    connect(cdUpAction, &QAction::triggered, [=](){Q_EMIT this->cdUpRequest();});
    navigationToolBar->addAction(cdUpAction);

    QAction *reloadAction = new QAction(QIcon::fromTheme("gtk-refresh"), tr("Refresh"), navigationToolBar);
    navigationToolBar->addAction(reloadAction);
    reloadAction->setShortcut(QKeySequence(tr("F5", "Reload")));
    connect(reloadAction, &QAction::triggered, this, &PeonyLocationBar::reloadViewRequest);

    hLayout->addWidget(navigationToolBar);

    //path bar, use setText slot get current path?
    Fm::PathBar *pathBar = new Fm::PathBar(this);
    m_path_bar = pathBar;
    hLayout->addWidget(pathBar, 20);

    /*
    QLineEdit *pathEntry = new QLineEdit(this);
    m_location_edit_line = pathEntry;
    QAction *goToAction = new QAction(QIcon::fromTheme("gtk-go-down"), "", pathEntry);
    pathEntry->addAction(goToAction, QLineEdit::TrailingPosition);
    hLayout->addWidget(pathEntry, 20);
    */

    QToolBar *viewToolBar = new QToolBar(this);
    QAction *previewAction = new QAction(QIcon::fromTheme("preview"), tr("Preview"), viewToolBar);
    previewAction->setCheckable(true);
    connect(previewAction, &QAction::triggered, [=](bool triggered){
        qDebug()<<triggered;
        previewAction->setChecked(triggered);
        Q_EMIT this->previewPageStateChangeRequest(triggered);
    });

    QAction *iconViewAction = new QAction(QIcon::fromTheme("icon-view"), tr("Icon View"), viewToolBar);
    QAction *listViewAction = new QAction(QIcon::fromTheme("list-view"), tr("List View"), viewToolBar);
    connect(iconViewAction, &QAction::triggered, [=](){
        Q_EMIT this->switchViewModeRequest(Fm::FolderView::IconMode);
    });
    connect(listViewAction, &QAction::triggered, [=](){
        Q_EMIT this->switchViewModeRequest(Fm::FolderView::DetailedListMode);
    });

    QList<QAction*> viewToolBarActions;
    viewToolBar->addActions(viewToolBarActions<<previewAction<<iconViewAction<<listViewAction);
    hLayout->addWidget(viewToolBar, 0, Qt::AlignRight);

    is_location_search_bar_created = true;
}

void PeonyLocationBar::updateLocationBarStatus(QString string)
{
    if (m_location_edit_line != nullptr)
        m_location_edit_line->setText(string);
}

void PeonyLocationBar::updateLocationBarStatus(const Fm::FilePath &path)
{
    m_path_bar->setPath(path);
}
