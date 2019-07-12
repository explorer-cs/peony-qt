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

#include <QPainter>
#include <QTimer>

#include <QDebug>

#include <QStandardPaths>

PeonyDesktopWindow::PeonyDesktopWindow(QWidget *parent) : QListView(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_X11NetWmWindowTypeDesktop);
    setAttribute(Qt::WA_DeleteOnClose);

    setBackgroundPath("/usr/share/backgrounds/ubuntukylin-default-settings.jpg");

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

    setAutoFillBackground(false);
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

void PeonyDesktopWindow::setBackgroundPath(QString path)
{
    if (bg_path == nullptr) {
        bg_path = path;
    } else {
        new_bg_path = path;
    }
}

void PeonyDesktopWindow::paintEvent(QPaintEvent *e)
{
    //return QListView::paintEvent(e);
    //TODO: this is very inefficient.
    //i need to improve paint event, do not draw a whole background image so frequently.
    //Fm::FolderView even perform better than QListView, perhaps i need implement a custom view.
    QPainter painter(this->viewport());
    if (new_bg_path != nullptr && bg_path != new_bg_path) {
        //fade out old bg, and trans to new bg, maybe it was controlled by timer.
        //when trans done, new bg path will replace old bg path.
        process_count = 0;
        QTimer *timer = new QTimer;
        timer->start(100);
        connect(timer, &QTimer::timeout, [=](){
            qDebug()<<"time out";
            process_count ++;
            if (process_count >= 10) {
                timer->deleteLater();
                qDebug()<<"delete later";
                bg_path = new_bg_path;
                new_bg_path = nullptr;
            } else {
                //compose two picture. two pictures both have a occupicity property,
                //which is controlled by process_count.
            }
        });
    } else {
        QImage image(bg_path);
        painter.drawImage(viewport()->rect(), image);
    }

    return QListView::paintEvent(e);
}

void PeonyDesktopWindow::dropEvent(QDropEvent *e)
{
    return QListView::dropEvent(e);
}
