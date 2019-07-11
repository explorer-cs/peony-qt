#include "peony-desktop-window.h"

#include <QSettings>
#include <QDesktopWidget>
#include <QApplication>
#include <libfm-qt/foldermodel.h>
#include <libfm-qt/proxyfoldermodel.h>

#include <QStandardPaths>

PeonyDesktopWindow::PeonyDesktopWindow(QWidget *parent) : QListView(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);
    setAttribute(Qt::WA_DeleteOnClose);

    setViewMode(QListView::IconMode);
    setGridSize(QSize(72, 72));
    setIconSize(QSize(48, 48));
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

    m_model = new Fm::FolderModel;
    m_model->setFolder(Fm::Folder::fromPath(Fm::FilePath::fromLocalPath(desktopDir.toUtf8())));

    m_proxy_model = new Fm::ProxyFolderModel;
    m_proxy_model->setSourceModel(m_model);

    setModel(m_proxy_model);
    showFullScreen();
}

PeonyDesktopWindow::~PeonyDesktopWindow()
{
    delete m_proxy_model;
    delete m_model;
}
