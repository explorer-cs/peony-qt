#include "peony-tool-bar.h"
#include <QHBoxLayout>

#include <QToolBar>
#include <QPushButton>

#include <QMenu>
#include <QAction>
#include <QIcon>

#include <QLineEdit>

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
    toolBar->addAction(newDirectoryAction);

    const QIcon newWindowIcon = QIcon::fromTheme("gtk-new", QIcon::fromTheme("add"));
    QAction *newWindowIconAction = new QAction(newWindowIcon, "New Window", this);
    newWindowIconAction->setToolTip("Create new peony window");
    //connect
    toolBar->addAction(newWindowIconAction);

    QAction *openInTerminalAction = new QAction(QIcon::fromTheme("terminal"), "Opem in terminal", this);
    toolBar->addAction(openInTerminalAction);

    toolBar->addSeparator();

    //1.check view settings, get current view state.
    //2.monitor settings, when view settings changed, change action icon, too.
    const QIcon currentViewIcon = QIcon::fromTheme("add");
    QAction *selectViewAction = new QAction(currentViewIcon, "add", this);

    QMenu *viewSelectMenu = new QMenu(this);
    QAction *iconViewAction = new QAction(QIcon::fromTheme("gtk-go-up"), "Icon View", viewSelectMenu);
    viewSelectMenu->addAction(iconViewAction);
    QAction *chooseIconViewAction = new QAction(QIcon::fromTheme("gtk-go-down"), "List View", viewSelectMenu);
    viewSelectMenu->addAction(chooseIconViewAction);
    selectViewAction->setMenu(viewSelectMenu);
    toolBar->addAction(selectViewAction);

    //what the aim of this action?
    QAction *sortAction = new QAction(QIcon::fromTheme("gtk-sort-ascending"), "Sort Ascending", this);
    QMenu *sortMenu = new QMenu(this);
    QAction *sortByName = new QAction("By Name", sortMenu);
    QAction *sortByType = new QAction("By Type", sortMenu);
    QList<QAction*> sortMenuActions;
    sortMenu->addActions(sortMenuActions<<sortByName<<sortByType);
    sortAction->setMenu(sortMenu);
    toolBar->addAction(sortAction);

    //copy, paste and cut, send signal to window, let window handle it.
    toolBar->addSeparator();

    QAction *copyAction = new QAction(QIcon::fromTheme("gtk-copy"), "Copy", this);
    QAction *pasteAction = new QAction(QIcon::fromTheme("gtk-paste"), "Paste", this);
    QAction *cutAction = new QAction(QIcon::fromTheme("gtk-cut"), "Cut", this);
    QList<QAction*> fileOpActions;
    toolBar->addActions(fileOpActions<<copyAction<<pasteAction<<cutAction);

    //delete or trash?
    toolBar->addSeparator();
    QAction *deleteAction = new QAction(QIcon::fromTheme("gtk-close"), "Delete", this);
    toolBar->addAction(deleteAction);

    //share, burrner and archive
    toolBar->addSeparator();
    QAction *shareAction = new QAction(QIcon::fromTheme("gtk-share"), "Share", this);
    QAction *burrnerAction = new QAction(QIcon::fromTheme("gtk-cdrom"), "Burrner", this);
    QAction *archiveAction = new QAction(QIcon::fromTheme("add-files-to-archive"), "Archive", this);
    QList<QAction*> additionalActions;
    toolBar->addActions(additionalActions<<shareAction<<burrnerAction<<archiveAction);

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
