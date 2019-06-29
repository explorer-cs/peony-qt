#include "peony-folder-view.h"

#include <libfm-qt/foldermodel.h>
#include <libfm-qt/cachedfoldermodel.h>
#include <libfm-qt/filelauncher.h>

#include <QDebug>

static int clicked_count = 0;

PeonyFolderView::PeonyFolderView(QWidget *parent) : Fm::FolderView(parent)
{
    Fm::FilePath home = Fm::FilePath::homeDir();
    Fm::CachedFolderModel* model = Fm::CachedFolderModel::modelFromPath(home);
    m_cache_model = model;
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
    clicked_count++;
    qDebug()<<clicked_count;
    switch (type) {
    case 0:
        if (fileInfo->isDir()){
            qDebug()<<type<<fileInfo->name().c_str()<<fileInfo->name().length();
            if (clicked_count%2 == 0)
                m_model->setFolder(Fm::Folder::fromPath(fileInfo->path()));
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
