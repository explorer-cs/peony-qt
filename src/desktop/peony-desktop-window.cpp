#include "peony-desktop-window.h"

#include <QSettings>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QApplication>
#include <libfm-qt/foldermodel.h>
#include <libfm-qt/proxyfoldermodel.h>
#include <libfm-qt/filemenu.h>

#include "peony-desktop-model.h"
#include "peony-application.h"

#include <QDebug>

#include <QStandardPaths>

PeonyDesktopWindow::PeonyDesktopWindow(QWidget *parent) : QListView(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);
    setAttribute(Qt::WA_DeleteOnClose);

    setViewMode(QListView::IconMode);
    setGridSize(QSize(128, 108));
    setIconSize(QSize(48, 48));
    setContentsMargins(12, 12, 12, 12);
    setSpacing(12);
    setUniformItemSizes(true);
    setMovement(QListView::Snap);
    setResizeMode(QListView::Adjust);
    setFlow(QListView::TopToBottom);

    setSelectionMode(QAbstractItemView::ExtendedSelection);

    viewport()->setAutoFillBackground(false);

    setFrameShape(QFrame::NoFrame);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //NoEditTriggers
    setEditTriggers(0);

    installEventFilter(this);
    viewport()->installEventFilter(this);

    /*
    if(QApplication::desktop()->isVirtualDesktop()) {

    }
    */
    QString desktopDir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    m_model = new PeonyDesktopModel;
    auto folder = Fm::Folder::fromPath(Fm::FilePath::fromLocalPath(desktopDir.toUtf8()));
    m_model->setFolder(folder);

    //actually, desktop window not need sort.
    //we must read every file in desktop postion and move them to last pos it was saved.

    connect(folder.get(), &Fm::Folder::finishLoading, [=](){
        qDebug()<<"desktop folder finish loading, trying to insert vitrual element";
        m_model->insertComputerHomeTrash();
    });

    setModel(m_model);
    showFullScreen();

    connect(this, &QAbstractItemView::doubleClicked, [=](const QModelIndex &index){
        auto info = m_model->fileInfoFromIndex(index);
        qDebug()<<info.get()->path().uri().get();
        if (info.get()->isDir()) {
            PeonyApplication::openNewWindow(info.get()->path().uri().get());
        } else {
            QDesktopServices::openUrl(QUrl(info.get()->path().uri().get()));
        }
    });
}

PeonyDesktopWindow::~PeonyDesktopWindow()
{
    delete m_proxy_model;
    delete m_model;
}

void PeonyDesktopWindow::dropEvent(QDropEvent *e)
{
    QListView::dropEvent(e);
}
