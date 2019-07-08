#include "peony-navigation-window.h"

#include "border-layout.h"

#include "peony-tool-bar.h"
#include "peony-location-bar.h"
#include "peony-status-bar.h"

#include "peony-folder-view.h"

#include "peony-preview-page.h"

#include <libfm-qt/placesview.h>
#include <libfm-qt/placesmodel.h>
#include <libfm-qt/pathbar.h>

#include <libfm-qt/fm-search.h>
#include <libfm-qt/filesearchdialog.h>
#include <libfm-qt/filelauncher.h>

#include <libfm-qt/fileoperation.h>
#include <libfm-qt/mountoperation.h>

#include <libfm-qt/utilities.h>

#include <QMessageBox>

#include <QSplitter>
#include <QAction>
#include <QMenu>

#include <QProcess>

PeonyNavigationWindow::PeonyNavigationWindow(QWidget *parent) : QWidget (parent)
{
    //QIcon::setThemeName("ukui-icon-theme");
    m_forward_list.clear();
    m_backward_list.clear();

    initLayout();
    initSignal();
    goToPath(Fm::FilePath::homeDir(), false);
    m_location_bar->pathBar()->setPath(Fm::FilePath::homeDir());
}

PeonyNavigationWindow::PeonyNavigationWindow(Fm::FilePath target_dir ,QWidget *parent) : QWidget (parent)
{
    m_forward_list.clear();
    m_backward_list.clear();

    initLayout();
    initSignal();
    goToPath(target_dir, false);
    m_location_bar->pathBar()->setPath(target_dir);
}

void PeonyNavigationWindow::initLayout()
{
    if (m_layout == nullptr)
        m_layout = new BorderLayout(this);

    setLayout(m_layout);

    createToolBars();
    createStatusBar();
    createFolderView();

    //m_view_spliter is created by PeonyNavigationWindow::createFolderView()
    //splitter index:
    //0.side bar, 1.folder view, 2.preview page
    m_preview_page = new PeonyPreviewPage(this);
    m_view_splitter->addWidget(m_preview_page);
    m_view_splitter->setCollapsible(2, false);
    m_preview_page->hide();
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
            this->updateLocationBarPath(path);
        }
    });
    side_bar_view->setMaximumWidth(200);
    m_view_splitter->insertWidget(0, side_bar_view);
    m_view_splitter->setCollapsible(0, false);

    m_view_splitter->setHandleWidth(1.0);

    connect(folder_view, &PeonyFolderView::clicked, folder_view, &PeonyFolderView::onFileClicked);

    m_layout->addWidget(m_view_splitter, BorderLayout::Center);
}

void PeonyNavigationWindow::cdUp()
{
    if (m_folder_view != nullptr){
        m_forward_list.clear();
        Fm::FilePath parent_path = m_folder_view->path().parent();
        if (parent_path.isValid()){
            goToPath(parent_path, true);
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
        connect(m_tool_bar, &PeonyToolBar::createFolderRequest, [=](){
            Fm::createFileOrFolder(Fm::CreateNewFolder, m_folder_view->path(), nullptr, nullptr);
        });
        connect(m_tool_bar, &PeonyToolBar::newWindowRequest, [=](){
            PeonyNavigationWindow *new_window = new PeonyNavigationWindow(m_folder_view->path(), nullptr);
            new_window->resize(1000, 618);
            new_window->show();
        });
        connect(m_tool_bar, &PeonyToolBar::openInTerminalRequest, [=](){
            qDebug()<<"open terminal";
            qDebug()<<m_folder_view->path().toString().get();
            std::string path = m_folder_view->path().toString().get();
            const gchar* directory = path.c_str();
            gchar **argv = nullptr;
            qDebug()<<g_shell_parse_argv ("mate-terminal", nullptr, &argv, nullptr);
            qDebug()<<argv;
            GError *err = nullptr;
            g_spawn_sync (directory,
                       argv,
                       nullptr,
                       G_SPAWN_SEARCH_PATH,
                       nullptr,
                       nullptr,
                       nullptr,
                       nullptr,
                       nullptr,
                       &err);
            if (err) {
                qDebug()<<err->message;
                g_error_free(err);
                err = nullptr;
            }
            g_strfreev (argv);
        });

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
            //vfs search:/// must be register, Fm::LibfmQt instance will do that.
            this->updateLocationBarPath(Fm::FilePath::fromUri(c_str_uri));
        });

        //connect file op
        connect(m_tool_bar, &PeonyToolBar::copyToClipboardRequest, this, &PeonyNavigationWindow::copyToClipboard);
        connect(m_tool_bar, &PeonyToolBar::cutToClipboradRequest, this, &PeonyNavigationWindow::cutToClipboard);
        connect(m_tool_bar, &PeonyToolBar::pasteFromClipboardRequest, this, &PeonyNavigationWindow::pasteFromClipboard);
        connect(m_tool_bar, &PeonyToolBar::deleteSelectionRequest, this, &PeonyNavigationWindow::deleteSelection);

        //share

        //burner
        connect(m_tool_bar, &PeonyToolBar::burnRequest, [=](){
            QProcess p;
            p.startDetached("burner");
        });
        //archives
        connect(m_tool_bar, &PeonyToolBar::archiveSeletionRequest, [=](){
            Fm::FilePathList files = m_folder_view->selectedFiles().paths();
            QStringList strs;
            QString tmp_str = " ";
            for (Fm::FilePath file : files) {
                tmp_str = tmp_str + " " + file.toString().get();
            }
            qDebug()<<tmp_str;
            QProcess p;
            p.startDetached("parchives -d" + tmp_str);
        });

        //location bar
        connect(m_location_bar, &PeonyLocationBar::cdUpRequest, this, &PeonyNavigationWindow::cdUp);
        connect(m_location_bar->pathBar(), &Fm::PathBar::chdir, [=](const Fm::FilePath &location){
            qDebug()<<"chdir signal"<<"target path:"<<location.uri().get();
            if (location == m_folder_view->path())
                return ;
            bool has_computer_schema = location.hasUriScheme("computer:///");
            auto folder = Fm::Folder::fromPath(location);
            connect(folder.get(), &Fm::Folder::error, [=](const Fm::GErrorPtr& err, Fm::Job::ErrorSeverity severity, Fm::Job::ErrorAction& response){
                if (has_computer_schema) {
                    //local volume mount has been done by PeonyFolderView::onFileClicked.
                    response = Fm::Job::ErrorAction::CONTINUE;
                    return ;
                }
                if(err.domain() == G_IO_ERROR) {
                        if(err.code() == G_IO_ERROR_NOT_MOUNTED && severity < Fm::Job::ErrorSeverity::CRITICAL) {
                            auto& path = folder->path();
                            Fm::MountOperation* op = new Fm::MountOperation(true);
                            op->setAutoDestroy(true);
                            op->mountEnclosingVolume(path);
                            if(op->wait()) {
                                response = Fm::Job::ErrorAction::RETRY;
                                return;
                            }
                        }
                    }
                    if(severity >= Fm::Job::ErrorSeverity::MODERATE) {
                        QMessageBox::critical(this, tr("Error"), err.message());
                    }
                    response = Fm::Job::ErrorAction::CONTINUE;
            });
            //this may not be a good way,
            //we'll set pathbar path twice when we clicked the pathbar button.
            this->goToPath(location, true);
            updateLocationBarPath(location);
        });

        connect(m_location_bar, &PeonyLocationBar::backRequest, this, &PeonyNavigationWindow::goBack);
        connect(m_location_bar, &PeonyLocationBar::forwardRequest, this, &PeonyNavigationWindow::goForward);

        connect(m_location_bar, &PeonyLocationBar::historyMenuRequest, this, &PeonyNavigationWindow::showHistoryMenu);
        connect(m_location_bar, &PeonyLocationBar::reloadViewRequest, m_folder_view, &PeonyFolderView::reload);

        connect(m_location_bar, &PeonyLocationBar::switchViewModeRequest, [=](Fm::FolderView::ViewMode mode){
            if (mode == m_folder_view->viewMode())
                return ;
            m_folder_view->setViewMode(mode);
        });

        //folder view
        connect(m_folder_view, &PeonyFolderView::chdirRequest, [=](const Fm::FilePath path, bool addHistory){
            //maybe updateLocationBarPath need add arg addHistory?
            this->updateLocationBarPath(path);
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

        //preview page and location bar control

        //connect/disconnect m_preview_page signal dynamiclly.
        connect(m_folder_view, &PeonyFolderView::selChanged, m_preview_page, &PeonyPreviewPage::selChanged);

        connect(m_location_bar, &PeonyLocationBar::previewPageStateChangeRequest, [=](bool show){
            if (show) {
                connect(m_preview_page, &PeonyPreviewPage::selChanged, [=](){
                    if (!m_preview_page->isVisible())
                        return ;
                    qDebug()<<"preview";
                    Fm::FilePathList selections = m_folder_view->selectedFiles().paths();
                    if (selections.empty()) {
                        m_preview_page->setDefault();
                    } else {
                        auto path = selections.begin();
                        qDebug()<<path->uri().get();
                        m_preview_page->preview(path->localPath().get());
                    }
                });

                //when open a preview page, it might be better to start a preview
                //if files selected.
                Fm::FilePathList selections = m_folder_view->selectedFiles().paths();
                if (selections.empty()) {
                    m_preview_page->setDefault();
                } else {
                    auto path = selections.begin();
                    qDebug()<<path->uri().get();
                    m_preview_page->preview(path->localPath().get());
                }
                m_preview_page->show();
            } else {
                m_preview_page->disconnect(nullptr, nullptr, nullptr, nullptr);
                m_preview_page->hide();
            }
        });
    }
}

void PeonyNavigationWindow::goToPath(const Fm::FilePath &path, bool addHistory)
{
    qDebug()<<"go to path............";
    qDebug()<<"current:"<<m_folder_view->path().toString().get()<<"new:"<<path.toString().get();
    if (m_layout != nullptr){
        if (m_folder_view->path() == path)
            return;
        if (addHistory) {
            m_backward_list.push_back(m_folder_view->path());
            if (m_backward_list.size() > 10)
                m_backward_list.erase(m_backward_list.begin());
        }
        Fm::ProxyFolderModel *model = m_folder_view->model();
        Fm::FolderModel *sourceModel = static_cast<Fm::FolderModel*>(model->sourceModel());
        sourceModel->setFolder(nullptr);
        sourceModel->setFolder(Fm::Folder::fromPath(path));
        model->setSourceModel(sourceModel);
    }
}

void PeonyNavigationWindow::updateLocationBarPath(Fm::FilePath path)
{
    if (m_location_bar && (m_folder_view->path() != path)) {
        m_location_bar->pathBar()->setPath(path);
        if (QString(path.baseName().get()) != "/")
            m_status_bar->updateStatusBarStatus(path.baseName().get());
        else {
            m_status_bar->updateStatusBarStatus(path.displayName().get());
        }
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
                this->goToPath(back_path, false);
                m_location_bar->pathBar()->setPath(back_path);
                m_status_bar->updateStatusBarStatus(back_path.baseName().get());
                //backward list and forward list need change.
            });
        }
        //do not connect currentItem action.
        //historyMenu->addAction(currentItem);
        for (Fm::FilePath forward_path : m_forward_list){
            QAction *menuItem = new QAction(forward_path.toString().get(), historyMenu);
            historyMenu->addAction(menuItem);
            connect(menuItem, &QAction::triggered, [=](){
                this->goToPath(forward_path, false);
                m_location_bar->pathBar()->setPath(forward_path);
                m_status_bar->updateStatusBarStatus(forward_path.baseName().get());
                //backward list and forward list need change.
            });
        }
        historyMenu->exec(QCursor::pos());
    }
}

//file operation
void PeonyNavigationWindow::copyToClipboard()
{
    Fm::FilePathList list = m_folder_view->selectedFiles().paths();
    Fm::copyFilesToClipboard(list);
}

void PeonyNavigationWindow::pasteFromClipboard()
{
    Fm::pasteFilesFromClipboard(m_folder_view->path());
}

void PeonyNavigationWindow::cutToClipboard()
{
    Fm::FilePathList list = m_folder_view->selectedFiles().paths();
    Fm::cutFilesToClipboard(list);
}

void PeonyNavigationWindow::deleteSelection()
{
    Fm::FilePathList list = m_folder_view->selectedFiles().paths();
    Fm::FileOperation::trashFiles(list, /*true*/false, this);
    //m_folder_view->reload();
}
//file operation
