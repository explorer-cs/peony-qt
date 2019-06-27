#include "peony-folder-view.h"

#include <libfm-qt/foldermodel.h>
#include <libfm-qt/cachedfoldermodel.h>

#include <QDebug>

PeonyFolderView::PeonyFolderView(QWidget *parent) : Fm::FolderView(parent)
{
    Fm::FilePath home = Fm::FilePath::homeDir();
    Fm::CachedFolderModel* model = Fm::CachedFolderModel::modelFromPath(home);
    Fm::ProxyFolderModel *proxy_model = new Fm::ProxyFolderModel();
    proxy_model->sort(Fm::FolderModel::ColumnFileName, Qt::AscendingOrder);
    proxy_model->setSourceModel(model);

    proxy_model->setThumbnailSize(64);
    proxy_model->setShowThumbnails(true);

    this->setModel(proxy_model);
    m_model = model;
    m_proxy_model = proxy_model;
    //this->setViewMode(Fm::FolderView::DetailedListMode);
}

void PeonyFolderView::onFileClicked(int type, const std::shared_ptr<const Fm::FileInfo>& fileInfo)
{
    qDebug()<<type;
    switch (type) {
    case 0:
        if (fileInfo->isDir()){
            qDebug()<<type<<fileInfo->name().c_str();
            fileInfo->path().toString();
            m_model->setFolder(Fm::Folder::fromPath(fileInfo->path()));
            fileInfo->path().parent();
            //m_model->setFolder(Fm::Folder::fromPath(Fm::FilePath::fromUri("trash:///")));
            //m_model->setFolder(Fm::Folder::fromPath(fileInfo->path().parent()));
            break;
        } else {
            //if (m_model->path().parent().isValid())
            //    m_model->setFolder(Fm::Folder::fromPath(m_model->path().parent()));
            //Fm::FolderView::onFileClicked(type, fileInfo);
        }
        break;
    case 2:
        //return Fm::FolderView::onFileClicked(type, fileInfo);
        //break;
    default:
        Fm::FolderView::onFileClicked(type, fileInfo);
        break;
    }
}
