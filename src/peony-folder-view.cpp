#include "peony-folder-view.h"

#include <libfm-qt/foldermodel.h>
#include <libfm-qt/cachedfoldermodel.h>
#include <libfm-qt/filelauncher.h>

#include <libfm-qt/folderview_p.h>
#include <libfm-qt/mountoperation.h>
#include <libfm-qt/core/dirlistjob.h>

#include <libfm-qt/mountoperation.h>

#include <QMessageBox>
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

                //for remote locaiton, uri in file info is not target uri.
                //use g_file_query_info qurey G_FILE_ATTRIBUTE_STANDARD_TARGET_URI
                //and get it by g_file_info_get_attribute_string (info, G_FILE_ATTRIBUTE_STANDARD_TARGET_URI)
                //at last, go to FilePath form target uri.

                //TODO:
                //for most remote location, we need mount remote location to gvfs by GMountOperation.
                //we need show a dialog for passing athucation info to do a mount.
                //For now, this is done in PeonyNavigationWindow::updateLocationBarPath().

                GFile *file = g_file_new_for_uri(fileInfo->path().uri().get());
                GFileInfo *info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_TARGET_URI, G_FILE_QUERY_INFO_NONE, nullptr, nullptr);
                const char *target_uri = g_file_info_get_attribute_string(info, G_FILE_ATTRIBUTE_STANDARD_TARGET_URI);

                Fm::FilePath target_path = Fm::FilePath::fromUri(target_uri);

                g_object_unref(info);
                g_object_unref(file);
                if (!target_path.isValid()){
                    qDebug()<<fileInfo->path().uri().get();
                    qDebug()<<fileInfo->path().toString().get();
                    Q_EMIT chdirRequest(fileInfo->path(), true);
                } else {
                    Q_EMIT chdirRequest(target_path, true);
                }
            }
        } else if (fileInfo->isMountable() && clicked_count%2 == 0) {
            qDebug()<<"mountable file, maybe a volume?"<<"target:"<<fileInfo->target().c_str();
            if (!fileInfo->target().empty()) {
                Q_EMIT chdirRequest(Fm::FilePath::fromLocalPath(fileInfo->target().c_str()), true);
            } else {
                //must mount the volume at first.
                Fm::MountOperation *op = new Fm::MountOperation(true, this);
                op->setAutoDestroy(true);
                op->mountMountable(fileInfo->path());
                connect(op, &Fm::MountOperation::finished, [=](){
                    //after mount op finished, we can query the target uri info manually.
                    qDebug()<<"finish"<<"info target:"<<fileInfo->target().c_str();
                    GFile *file = g_file_new_for_uri(fileInfo->path().uri().get());
                    GFileInfo *info = g_file_query_info(file, G_FILE_ATTRIBUTE_STANDARD_TARGET_URI, G_FILE_QUERY_INFO_NONE, nullptr, nullptr);
                    const char *target_uri = g_file_info_get_attribute_string(info, G_FILE_ATTRIBUTE_STANDARD_TARGET_URI);
                    qDebug()<<"query again:"<<target_uri;
                    Fm::FilePath target_path = Fm::FilePath::fromUri(target_uri);
                    g_object_unref(info);
                    g_object_unref(file);
                    Q_EMIT this->chdirRequest(target_path, true);
                });
                op->wait();
            }
            //Fm::FolderView::onFileClicked(type, fileInfo);
        } else {
            Fm::FolderView::onFileClicked(type, fileInfo);
        }
        break;
    case 1:
        //middle clicked
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

void PeonyFolderView::reload()
{
    folder()->reload();
}

void PeonyFolderView::testVolume()
{

}
