#include "peony-navigation-window.h"

#include "border-layout.h"

#include "peony-tool-bar.h"
#include "peony-location-bar.h"
#include "peony-status-bar.h"

#include "peony-folder-view.h"

#include <libfm-qt/placesview.h>
#include <libfm-qt/placesmodel.h>
#include <libfm-qt/pathbar.h>

#include <libfm-qt/fm-search.h>
#include <libfm-qt/filesearchdialog.h>
#include <libfm-qt/filelauncher.h>

#include <libfm-qt/fileoperation.h>

#include <QSplitter>
#include <QAction>
#include <QMenu>

PeonyNavigationWindow::PeonyNavigationWindow(QWidget *parent) : QWidget (parent)
{
    //QIcon::setThemeName("ukui-icon-theme");
    m_forward_list.clear();
    m_backward_list.clear();
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

    PeonyLocationBar *locationBar = new PeonyLocationBar(this);
    locationBar->createLocationBar();
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
        qDebug()<<"type:"<<type;
        if (type == 0){
            //this->goToPath(path);
            this->updateLocationBarPath(path);
        }
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
        m_backward_list.push_back(m_folder_view->path());
        m_forward_list.clear();
        Fm::ProxyFolderModel *model = m_folder_view->model();
        Fm::CachedFolderModel *sourceModel = static_cast<Fm::CachedFolderModel*>(model->sourceModel());
        Fm::FilePath parent_path = sourceModel->folder()->path().parent();
        if (parent_path.isValid()){
            updateLocationBarPath(parent_path);
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
        //tool bar
        connect(m_location_bar, &PeonyLocationBar::cdUpRequest, this, &PeonyNavigationWindow::cdUp);

        connect(m_tool_bar, &PeonyToolBar::changeViewModeRequest, m_folder_view, &PeonyFolderView::setViewMode);
        connect(m_tool_bar, &PeonyToolBar::changeSortModeRequest, [=](Fm::FolderModel::ColumnId sortmode){
            Fm::ProxyFolderModel *model = m_folder_view->model();
            model->sort(sortmode);
        });

        connect(m_tool_bar, &PeonyToolBar::searchRequest, [=](QString key){
            qDebug()<<"window get search key"<<key;
            QString current_path = m_folder_view->path().toString().get();
            qDebug()<<current_path;
            qDebug()<<m_folder_view->path().toString().get();
            QStringList paths;
            paths<<current_path;

            QString searchUri = "search://"+current_path+"?recursive=0&show_hidden=0&name_regex="+key;
            qDebug()<<"jump to"<<searchUri;
            std::string tmp = searchUri.toStdString();
            const char* c_str_uri = tmp.c_str();
            Fm::FilePath search_path = Fm::FilePath::fromUri(c_str_uri);
            Fm::FilePathList list;
            list.push_back(search_path);
            qDebug()<<search_path.uri().get();
            m_folder_view->launchPaths(this, list);
            //how to jump to a correct search uri?
            /*
            this->goToPath(Fm::FilePath::fromUri(c_str_uri));
            this->updateLocationBarPath(Fm::FilePath::fromUri(c_str_uri));
            */
        });

        //connect file op
        connect(m_tool_bar, &PeonyToolBar::copyToClipboardRequest, this, &PeonyNavigationWindow::copyToClipboard);
        connect(m_tool_bar, &PeonyToolBar::cutToClipboradRequest, this, &PeonyNavigationWindow::cutToClipboard);
        connect(m_tool_bar, &PeonyToolBar::pasteFromClipboardRequest, this, &PeonyNavigationWindow::pasteFromClipboard);
        connect(m_tool_bar, &PeonyToolBar::deleteSelectionRequest, this, &PeonyNavigationWindow::deleteSelection);

        //location bar
        Fm::FilePath path = m_folder_view->path();
        m_location_bar->pathBar()->setPath(path);
        connect(m_location_bar->pathBar(), &Fm::PathBar::chdir, [=](const Fm::FilePath &location){
            qDebug()<<"chdir signal";
            //this may not be a good way,
            //we'll set pathbar path twice when we clicked the pathbar button.
            updateLocationBarPath(location);
            Fm::ProxyFolderModel *model = m_folder_view->model();
            Fm::CachedFolderModel *sourceModel = static_cast<Fm::CachedFolderModel*>(model->sourceModel());
            sourceModel->setFolder(Fm::Folder::fromPath(location));
        });

        connect(m_location_bar, &PeonyLocationBar::backRequest, this, &PeonyNavigationWindow::goBack);
        connect(m_location_bar, &PeonyLocationBar::forwardRequest, this, &PeonyNavigationWindow::goForward);

        connect(m_location_bar, &PeonyLocationBar::historyMenuRequest, this, &PeonyNavigationWindow::showHistoryMenu);
        connect(m_location_bar, &PeonyLocationBar::reloadViewRequest, m_folder_view, &PeonyFolderView::reload);
        //folder view
        connect(m_folder_view, &PeonyFolderView::updatePathBarRequest, this, &PeonyNavigationWindow::updateLocationBarPath);

        connect(m_folder_view, &PeonyFolderView::pushBackListRequest, [=](Fm::FilePath location){
            m_backward_list.push_back(location);
        });

        connect(m_folder_view, &PeonyFolderView::selChanged, [=](){
            qDebug()<<"selection changed";
            Fm::FileInfoList infos = m_folder_view->selectedFiles();
            qDebug()<<infos.size();
            for (auto info:infos){
                qDebug()<<info.get()->path().toString().get();
            }

            if (infos.size() > 0){
                m_status_bar->updateStatusBarStatus(QString::number(infos.size())+" selected");
            } else {
                m_status_bar->updateStatusBarStatus(QString(m_folder_view->path().baseName().get()));
            }
        });
    }
}

void PeonyNavigationWindow::goToPath(const Fm::FilePath &path)
{
    qDebug()<<"go to path............";
    qDebug()<<"current:"<<m_folder_view->path().toString().get()<<"new:"<<path.toString().get();
    if (m_layout != nullptr){
        Fm::ProxyFolderModel *model = m_folder_view->model();
        Fm::CachedFolderModel *sourceModel = static_cast<Fm::CachedFolderModel*>(model->sourceModel());
        sourceModel->setFolder(Fm::Folder::fromPath(path));
    }
}

void PeonyNavigationWindow::updateLocationBarPath(Fm::FilePath path)
{
    if (m_location_bar) {
        m_location_bar->pathBar()->setPath(path);
        m_status_bar->updateStatusBarStatus(path.baseName().get());
    }
}

void PeonyNavigationWindow::goBack()
{
    //only double clicked and cdUp will push current path to back list.
    qDebug()<<"go back";
    if (!m_backward_list.empty()){
        //push current path to foward list.
        m_forward_list.push_back(m_folder_view->path());
        qDebug()<<"real go back";
        Fm::FilePath path = m_backward_list.back();
        qDebug()<<path.uri().get();
        updateLocationBarPath(path);
        m_backward_list.pop_back();
    }
}

void PeonyNavigationWindow::goForward()
{
    //cdUp will clear the list.
    qDebug()<<"go forward";
    if (!m_forward_list.empty()){
        qDebug()<<"real go forward";
        Fm::FilePath path = m_forward_list.back();
        qDebug()<<path.uri().get();
        m_backward_list.push_back(m_folder_view->path());
        updateLocationBarPath(path);
        m_forward_list.pop_back();
    }
}

void PeonyNavigationWindow::showHistoryMenu(QAction *, QMenu *historyMenu)
{
    //maybe backward list and forward list need merge,
    //it is diffcult to handle a history menu when they are splitted.
    historyMenu->clear();
    //QAction *currentItem = new QAction(QIcon::fromTheme("add"), m_folder_view->path().toString().get(), historyMenu);
    if (!m_backward_list.empty() || !m_forward_list.empty()){
        for (Fm::FilePath back_path : m_backward_list){
            QAction *menuItem = new QAction(back_path.toString().get(), historyMenu);
            historyMenu->addAction(menuItem);
            connect(menuItem, &QAction::triggered, [=](){
                this->goToPath(back_path);
                //backward list and forward list need change.
            });
        }
        //do not connect currentItem action.
        //historyMenu->addAction(currentItem);
        for (Fm::FilePath forward_path : m_forward_list){
            QAction *menuItem = new QAction(forward_path.toString().get(), historyMenu);
            historyMenu->addAction(menuItem);
            connect(menuItem, &QAction::triggered, [=](){
                this->goToPath(forward_path);
                //backward list and forward list need change.
            });
        }
        historyMenu->exec(QCursor::pos());
    }
}

//file operation
void PeonyNavigationWindow::copyToClipboard()
{
    last_file_op_type = Copy;
    Fm::FilePathList list = m_folder_view->selectedFiles().paths();
    m_clipborad_list.clear();
    m_clipborad_list.insert(m_clipborad_list.begin(), list.begin(), list.end());
}

void PeonyNavigationWindow::pasteFromClipboard()
{
    switch (last_file_op_type) {
    case Copy:
        Fm::FileOperation::copyFiles(m_clipborad_list, m_folder_view->path(), this);
        break;
    case Cut:
        Fm::FileOperation::moveFiles(m_clipborad_list, m_folder_view->path(), this);
        break;
    default:
        break;
    }
}

void PeonyNavigationWindow::cutToClipboard()
{
    last_file_op_type = Cut;
    Fm::FilePathList list = m_folder_view->selectedFiles().paths();
    m_clipborad_list.clear();
    m_clipborad_list.insert(m_clipborad_list.begin(), list.begin(), list.end());
    //hide or alpha selected files.
}

void PeonyNavigationWindow::deleteSelection()
{
    Fm::FilePathList list = m_folder_view->selectedFiles().paths();
    Fm::FileOperation::trashFiles(list, /*true*/false, this);
}
//file operation
