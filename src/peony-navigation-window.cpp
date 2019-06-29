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
    m_tool_bar = toolBar;

    PeonyLocationSearchBar *locationBar = new PeonyLocationSearchBar(this);
    locationBar->createLocationSearchBar();
    m_location_bar = locationBar;

    m_layout->addWidget(toolBar, BorderLayout::North);
    m_layout->addWidget(locationBar, BorderLayout::North);
}

void PeonyNavigationWindow::createStatusBar()
{
    if (m_layout == nullptr)
        return;

    PeonyStatusBar *statusBar = new PeonyStatusBar(this);
    statusBar->createStatusBar();
    m_status_bar = statusBar;
    //statusBar->setFixedHeight(24);

    m_layout->addWidget(statusBar, BorderLayout::South);
}

void PeonyNavigationWindow::createFolderView()
{
    if (m_layout == nullptr)
        return;

    m_view_splitter = new QSplitter(Qt::Horizontal, this);

    PeonyFolderView *folder_view = new PeonyFolderView(this);
    m_folder_view = folder_view;
    m_view_splitter->addWidget(folder_view);

    Fm::PlacesView *side_bar_view = new Fm::PlacesView(this);
    connect(side_bar_view, &Fm::PlacesView::chdirRequested, [=](int type, const Fm::FilePath& path){
        qDebug()<<type;
        if (type == 0)
            this->goToPath(path);
    });
    side_bar_view->setMaximumWidth(200);
    m_view_splitter->insertWidget(0, side_bar_view);

    m_view_splitter->setHandleWidth(1.0);

    connect(folder_view, &PeonyFolderView::clicked, folder_view, &PeonyFolderView::onFileClicked);

    m_layout->addWidget(m_view_splitter, BorderLayout::Center);
}

void PeonyNavigationWindow::cdUp()
{
    if (m_folder_view != nullptr){
        Fm::ProxyFolderModel *model = m_folder_view->model();
        Fm::CachedFolderModel *sourceModel = static_cast<Fm::CachedFolderModel*>(model->sourceModel());
        Fm::FilePath parent_path = sourceModel->folder()->path().parent();
        if (parent_path.isValid()){
            sourceModel->setFolder(Fm::Folder::fromPath(parent_path));
        }
    }
}

void PeonyNavigationWindow::updateLocationBarString(QString string)
{
    if (m_status_bar != nullptr)
        m_status_bar->updateStatusBarStatus(string);
}

void PeonyNavigationWindow::initSignal()
{
    if (m_layout != nullptr){
        connect(m_location_bar, &PeonyLocationSearchBar::cdUpRequest, this, &PeonyNavigationWindow::cdUp);

        connect(m_tool_bar, &PeonyToolBar::changeViewModeRequest, m_folder_view, &PeonyFolderView::setViewMode);
        connect(m_tool_bar, &PeonyToolBar::changeSortModeRequest, [=](Fm::FolderModel::ColumnId sortmode){
            Fm::ProxyFolderModel *model = m_folder_view->model();
            model->sort(sortmode);
        });
    }
}

void PeonyNavigationWindow::goToPath(Fm::FilePath path)
{
    if (m_layout != nullptr){
        Fm::ProxyFolderModel *model = m_folder_view->model();
        Fm::CachedFolderModel *sourceModel = static_cast<Fm::CachedFolderModel*>(model->sourceModel());
        sourceModel->setFolder(Fm::Folder::fromPath(path));
    }
}
