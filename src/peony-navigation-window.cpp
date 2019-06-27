#include "peony-navigation-window.h"

#include "border-layout.h"

#include "peony-tool-bar.h"
#include "peony-location-bar.h"
#include "peony-status-bar.h"

#include "peony-folder-view.h"

#include <libfm-qt/placesview.h>
#include <libfm-qt/placesmodel.h>

#include <QSplitter>

PeonyNavigationWindow::PeonyNavigationWindow(QWidget *parent) : QWidget (parent)
{
    //QIcon::setThemeName("ukui-icon-theme");
}

void PeonyNavigationWindow::initLayout()
{
    if (m_layout == nullptr)
        m_layout = new BorderLayout(this);

    setLayout(m_layout);

    createToolBars();
    createStatusBar();
    createFolderView();
}

void PeonyNavigationWindow::createToolBars()
{
    if (m_layout == nullptr)
        return;

    PeonyToolBar *toolBar = new PeonyToolBar(this);
    toolBar->createToolBar();

    PeonyLocationSearchBar *locationBar = new PeonyLocationSearchBar(this);
    locationBar->createLocationSearchBar();

    m_layout->addWidget(toolBar, BorderLayout::North);
    m_layout->addWidget(locationBar, BorderLayout::North);
}

void PeonyNavigationWindow::createStatusBar()
{
    if (m_layout == nullptr)
        return;

    PeonyStatusBar *statusBar = new PeonyStatusBar(this);
    statusBar->createStatusBar();
    //statusBar->setFixedHeight(24);

    m_layout->addWidget(statusBar, BorderLayout::South);
}

void PeonyNavigationWindow::createFolderView()
{
    if (m_layout == nullptr)
        return;

    m_view_splitter = new QSplitter(Qt::Horizontal, this);

    PeonyFolderView *folder_view = new PeonyFolderView(this);
    m_view_splitter->addWidget(folder_view);

    Fm::PlacesView *side_bar_view = new Fm::PlacesView(this);
    side_bar_view->setMaximumWidth(200);
    m_view_splitter->insertWidget(0, side_bar_view);

    m_view_splitter->setHandleWidth(1.0);

    connect(folder_view, &PeonyFolderView::clicked, folder_view, &PeonyFolderView::onFileClicked);

    m_layout->addWidget(m_view_splitter, BorderLayout::Center);
}
