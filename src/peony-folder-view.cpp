#include "peony-folder-view.h"

#include <libfm-qt/foldermodel.h>
#include <libfm-qt/cachedfoldermodel.h>
#include <libfm-qt/filelauncher.h>

#include <libfm-qt/folderview_p.h>

#include <QStyledItemDelegate>

#include <QDebug>

static int clicked_count = 0;

PeonyFolderView::PeonyFolderView(QWidget *parent) : Fm::FolderView(parent)
{
    Fm::FilePath home = Fm::FilePath::homeDir();
    m_model = new Fm::FolderModel;
    m_model->setFolder(Fm::Folder::fromPath(Fm::FilePath::homeDir()));

    Fm::ProxyFolderModel *proxy_model = new Fm::ProxyFolderModel();
    proxy_model->sort(Fm::FolderModel::ColumnFileName, Qt::AscendingOrder);
    proxy_model->setSourceModel(m_model);

    proxy_model->setThumbnailSize(64);
    proxy_model->setShowThumbnails(true);

    this->setModel(proxy_model);
    m_proxy_model = proxy_model;

    //in libfmqt, when mouse move above, will draw a symbol at left-top of icon grid.
    //symbol draw is controlled by delegate paint method. i try using normal delegate,
    //but many bad things occur.
    /*
    QStyledItemDelegate *delegate = new QStyledItemDelegate;
    connect(this, &Fm::FolderView::selChanged, [=](){
        if (this->viewMode() == Fm::FolderView::IconMode){
            Fm::FolderViewListView *view = static_cast<Fm::FolderViewListView*>(this->childView());
            view->setItemDelegateForColumn(Fm::FolderModel::ColumnFileName, delegate);
        }
    });
    */
}

void PeonyFolderView::onFileClicked(int type, const std::shared_ptr<const Fm::FileInfo>& fileInfo)
{
    clicked_count++;
    qDebug()<<clicked_count;
    switch (type) {
    case 0:
        if (fileInfo->isDir()){
            qDebug()<<type<<fileInfo->name().c_str()<<fileInfo->name().length();
            if (clicked_count%2 == 0){
                //m_model->setFolder(Fm::Folder::fromPath(fileInfo->path()));
                //updatePathBarRequest will set folder, too.
                //DO NOT setFolder here, otherwise every file in new directory will show twice.
                Q_EMIT pushBackListRequest(this->path());
                Q_EMIT updatePathBarRequest(fileInfo->path());
            }
        } else {
            Fm::FileInfoList file_list;
            file_list.push_back(fileInfo);
            fileLauncher()->launchFiles(this, file_list);
        }
        break;
    case 1:
        //open in new window
        break;
    case 2:
        if (clicked_count%2 == 1)
            Fm::FolderView::onFileClicked(type, fileInfo);
        break;
    default:
        break;
    }
}
