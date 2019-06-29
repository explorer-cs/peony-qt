#include "peony-tool-bar.h"
#include <QHBoxLayout>

#include <QToolBar>
#include <QPushButton>

#include <QMenu>
#include <QAction>
#include <QIcon>

#include <QLineEdit>

#include <QDebug>

PeonyToolBar::PeonyToolBar(QWidget *parent) : QWidget(parent)
{

}

void PeonyToolBar::createToolBar()
{
    if (is_tool_bar_created)
        return;

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    this->setLayout(hLayout);

    //hLayout->addSpacing(30);
    hLayout->setContentsMargins(20, 20, 20, 0);

    QToolBar *toolBar = new QToolBar("New...", this);

    const QIcon newDirectoryIcon = QIcon::fromTheme("folder-add", QIcon::fromTheme("new"));
    QAction *newDirectoryAction = new QAction(newDirectoryIcon, "&New Directory", this);
    newDirectoryAction->setShortcuts(QKeySequence::New);
    newDirectoryAction->setStatusTip("New a folder");
    //connect action trigged signal
    newDirectoryAction->setShortcut(QKeySequence::Copy);//just a test for shortcut
    connect(newDirectoryAction, &QAction::triggered, [=](){qDebug()<<"request create folder";Q_EMIT this->createFolderRequest();});

    toolBar->addAction(newDirectoryAction);

    const QIcon newWindowIcon = QIcon::fromTheme("gtk-new", QIcon::fromTheme("add"));
    QAction *newWindowIconAction = new QAction(newWindowIcon, "New Window", this);
    newWindowIconAction->setToolTip("Create new peony window");
    //connect
    toolBar->addAction(newWindowIconAction);

    QAction *openInTerminalAction = new QAction(QIcon::fromTheme("terminal"), "Opem in terminal", this);
    connect(openInTerminalAction, &QAction::triggered, [=](){Q_EMIT this->openInTerminalRequest();});
    toolBar->addAction(openInTerminalAction);

    toolBar->addSeparator();

    //1.check view settings, get current view state.
    //2.monitor settings, when view settings changed, change action icon, too.
    const QIcon currentViewIcon = QIcon::fromTheme("add");
    QAction *selectViewAction = new QAction(currentViewIcon, "add", this);

    QMenu *viewSelectMenu = new QMenu(this);
    QAction *chooseIconViewAction = new QAction(QIcon::fromTheme("gtk-go-up"), "Icon View", viewSelectMenu);
    connect(chooseIconViewAction, &QAction::triggered, [=](){Q_EMIT this->changeViewModeRequest(Fm::FolderView::IconMode);});
    viewSelectMenu->addAction(chooseIconViewAction);
    QAction *chooseListViewAction = new QAction(QIcon::fromTheme("gtk-go-down"), "List View", viewSelectMenu);
    connect(chooseListViewAction, &QAction::triggered, [=](){Q_EMIT this->changeViewModeRequest(Fm::FolderView::DetailedListMode);});
    viewSelectMenu->addAction(chooseListViewAction);
    selectViewAction->setMenu(viewSelectMenu);
    toolBar->addAction(selectViewAction);

    //what the aim of this action?
    QAction *sortAction = new QAction(QIcon::fromTheme("gtk-sort-ascending"), "Sort Ascending", this);
    QMenu *sortMenu = new QMenu(this);
    QAction *sortByName = new QAction("By Name", sortMenu);
    connect(sortByName, &QAction::triggered, [=](){Q_EMIT this->changeSortModeRequest(Fm::FolderModel::ColumnFileName);});
    QAction *sortByType = new QAction("By Type", sortMenu);
    connect(sortByType, &QAction::triggered, [=](){Q_EMIT this->changeSortModeRequest(Fm::FolderModel::ColumnFileType);});
    QList<QAction*> sortMenuActions;
    sortMenu->addActions(sortMenuActions<<sortByName<<sortByType);
    sortAction->setMenu(sortMenu);
    toolBar->addAction(sortAction);

    //copy, paste and cut, send signal to window, let window handle it.
    toolBar->addSeparator();

    QAction *copyAction = new QAction(QIcon::fromTheme("gtk-copy"), "Copy", this);
    connect(copyAction, &QAction::triggered, [=](){Q_EMIT this->copyToClipboardRequest();});
    QAction *pasteAction = new QAction(QIcon::fromTheme("gtk-paste"), "Paste", this);
    connect(pasteAction, &QAction::triggered, [=](){Q_EMIT this->pasteFromClipboardRequest();});
    QAction *cutAction = new QAction(QIcon::fromTheme("gtk-cut"), "Cut", this);
    connect(cutAction, &QAction::triggered, [=](){Q_EMIT this->cutToClipboradRequest();});
    QList<QAction*> fileOpActions;
    toolBar->addActions(fileOpActions<<copyAction<<pasteAction<<cutAction);

    //delete or trash?
    toolBar->addSeparator();
    QAction *deleteAction = new QAction(QIcon::fromTheme("gtk-close"), "Delete", this);
    connect(deleteAction, &QAction::triggered, [=](){Q_EMIT this->deleteSelectionRequest();});
    toolBar->addAction(deleteAction);

    //share, burrner and archive
    toolBar->addSeparator();
    QAction *shareAction = new QAction(QIcon::fromTheme("gtk-share"), "Share", this);
    connect(shareAction, &QAction::triggered, [=](){Q_EMIT this->shareSelectionRequest();});
    QAction *burnAction = new QAction(QIcon::fromTheme("gtk-cdrom"), "Burrner", this);
    connect(burnAction, &QAction::triggered, [=](){Q_EMIT this->burnRequest();});
    QAction *archiveAction = new QAction(QIcon::fromTheme("add-files-to-archive"), "Archive", this);
    connect(archiveAction, &QAction::triggered, [=](){Q_EMIT this->archiveSeletionRequest();});
    QList<QAction*> additionalActions;
    toolBar->addActions(additionalActions<<shareAction<<burnAction<<archiveAction);

    toolBar->addSeparator();
    QAction *moreAction = new QAction(QIcon::fromTheme("system-run"), "More", this);
    QMenu *moreMenu = new QMenu(this);
    QAction *testAction = new QAction("test", moreMenu);
    moreMenu->addAction(testAction);
    moreAction->setMenu(moreMenu);
    toolBar->addAction(moreAction);

    hLayout->addWidget(toolBar, 0, Qt::AlignLeft);

    //is tool bar need support plugin implement?

    QLineEdit *searchEntry = new QLineEdit(this);
    QAction *searchAction = new QAction(QIcon::fromTheme("search"), "search", searchEntry);
    searchEntry->addAction(searchAction, QLineEdit::TrailingPosition);
    hLayout->addWidget(searchEntry, 0, Qt::AlignRight);

    //do not create again
    is_tool_bar_created = true;
}
