#include "peony-location-bar.h"

#include <QHBoxLayout>

#include <QToolBar>
#include <QPushButton>

#include <QMenu>
#include <QAction>
#include <QIcon>

#include <QLineEdit>

PeonyLocationSearchBar::PeonyLocationSearchBar(QWidget *parent) : QWidget(parent)
{

}

void PeonyLocationSearchBar::createLocationSearchBar()
{
    if (is_location_search_bar_created)
        return;

    QHBoxLayout *hLayout = new QHBoxLayout(this);

    QToolBar *navigationToolBar = new QToolBar(this);

    //navigation tool bar need synchronous the buttons' states, connect the history manager signal.

    QAction *goBackAction = new QAction(QIcon::fromTheme("gtk-go-back"), "Go Back", navigationToolBar);
    navigationToolBar->addAction(goBackAction);

    QAction *goForwardAction = new QAction(QIcon::fromTheme("gtk-go-forward"), "Go Forward", navigationToolBar);
    navigationToolBar->addAction(goForwardAction);

    QAction *listHistoryAction = new QAction(QIcon::fromTheme("gtk-go-down"), "List History", navigationToolBar);
    navigationToolBar->addAction(listHistoryAction);

    //we must ensure that cd Up Action is disable in location "/"
    QAction *cdUpAction = new QAction(QIcon::fromTheme("gtk-go-up"), "cd Up", navigationToolBar);
    navigationToolBar->addAction(cdUpAction);

    QAction *reloadAction = new QAction(QIcon::fromTheme("gtk-refresh"), "Refresh", navigationToolBar);
    navigationToolBar->addAction(reloadAction);

    hLayout->addWidget(navigationToolBar);

    //path bar, use setText slot get current path?
    QLineEdit *pathEntry = new QLineEdit(this);
    QAction *goToAction = new QAction(QIcon::fromTheme("gtk-go-down"), "", pathEntry);
    pathEntry->addAction(goToAction, QLineEdit::TrailingPosition);
    hLayout->addWidget(pathEntry, 20);

    QToolBar *viewToolBar = new QToolBar(this);
    QAction *previewAction = new QAction(QIcon::fromTheme("preview"), "Preview", viewToolBar);
    QAction *iconViewAction = new QAction(QIcon::fromTheme("icon-view"), "Icon View", viewToolBar);
    QAction *listViewAction = new QAction(QIcon::fromTheme("list-view"), "List View", viewToolBar);
    QList<QAction*> viewToolBarActions;
    viewToolBar->addActions(viewToolBarActions<<previewAction<<iconViewAction<<listViewAction);
    hLayout->addWidget(viewToolBar, 0, Qt::AlignRight);

    is_location_search_bar_created = true;
}
