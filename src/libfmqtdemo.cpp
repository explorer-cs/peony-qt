#include "libfmqtdemo.h"

#include <libfm-qt/dirtreemodel.h>
#include <libfm-qt/dirtreeview.h>

#include <libfm-qt/foldermodel.h>
#include <libfm-qt/folderview.h>
#include <libfm-qt/cachedfoldermodel.h>
#include <libfm-qt/proxyfoldermodel.h>

#include <QDebug>
#include <gio/gio.h>

#include <QTreeView>

LibFmQtDemo::LibFmQtDemo(QObject *parent) : QObject(parent)
{
    /*
    Fm::DirTreeModel *model = new Fm::DirTreeModel(nullptr);
    Fm::DirTreeView *view = new Fm::DirTreeView(nullptr);
    view->setModel(model);

    //view->chdir(Fm::FilePath::fromLocalPath("/home/lanyue"));
    view->show();
    */
    Fm::FolderView *folder_view = new Fm::FolderView;

    Fm::FilePath home = Fm::FilePath::fromUri("sftp://112.124.201.32");
    Fm::CachedFolderModel* model = Fm::CachedFolderModel::modelFromPath(home);
    Fm::ProxyFolderModel *proxy_model = new Fm::ProxyFolderModel();
    proxy_model->sort(Fm::FolderModel::ColumnFileName, Qt::AscendingOrder);
    proxy_model->setSourceModel(model);

    proxy_model->setThumbnailSize(64);
    proxy_model->setShowThumbnails(true);

    folder_view->setModel(proxy_model);
    folder_view->show();

    QTreeView *tree_view = new QTreeView;
    tree_view->setModel(proxy_model);
    tree_view->show();
}
