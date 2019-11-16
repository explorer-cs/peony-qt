#ifndef DIRECTORYVIEWPLUGINIFACE2_H
#define DIRECTORYVIEWPLUGINIFACE2_H

#include "plugin-iface.h"

#include <QWidget>

namespace Peony {

class FileItemModel;
class FileItemProxyFilterSortModel;
class DirectoryView;
class FileItemModel;
class FileItemProxyFilterSortModel;

class DirectoryView : public QWidget
{
    Q_OBJECT
public:
    explicit DirectoryView(QWidget *parent = nullptr);
    virtual ~DirectoryView() {}

    const virtual QString viewId() {return tr("Directory View");}

    //location
    const virtual QString getDirectoryUri() {return nullptr;}

    //selections
    const virtual QStringList getSelections() {return QStringList();}

    //children
    const virtual QStringList getAllFileUris() {return QStringList();}

    virtual int getSortType() {return 0;}
    virtual Qt::SortOrder getSortOrder() {return Qt::AscendingOrder;}

Q_SIGNALS:
    //loaction
    //FIXME: support open in new TAB?
    void viewDoubleClicked(const QString &uri);
    void viewDirectoryChanged();
    void viewSelectionChanged();
    void sortTypeChanged(int type);
    void sortOrderChanged(Qt::SortOrder order);

    //menu
    void menuRequest(const QPoint &pos);

    //window
    void updateWindowLocationRequest(const QString &uri);

public Q_SLOTS:
    virtual void bindModel(FileItemModel *model, FileItemProxyFilterSortModel *proxyModel) {}

    //location
    virtual void open(const QStringList &uris, bool newWindow) {}
    virtual void setDirectoryUri(const QString &uri) {}
    virtual void beginLocationChange() {}
    virtual void stopLocationChange() {}

    virtual void closeDirectoryView() {}

    //selections
    virtual void setSelections(const QStringList &uris) {}
    virtual void invertSelections() {}
    virtual void scrollToSelection(const QString &uri) {}

    //clipboard
    //cut items should be drawn differently.
    virtual void setCutFiles(const QStringList &uris) {}

    virtual void setSortType(int sortType) {}
    virtual void setSortOrder(int sortOrder) {}

    virtual void editUri(const QString &uri) {}
    virtual void editUris(const QStringList uris) {}
};

class DirectoryViewPluginIface2 : public PluginInterface
{
public:
    virtual ~DirectoryViewPluginIface2() {}

    virtual QString viewIdentity() = 0;
    virtual QIcon viewIcon() = 0;
    virtual bool supportUri(const QString &uri) = 0;

    virtual int zoom_level_hint() = 0;

    virtual int priority(const QString &directoryUri) = 0;

    virtual void fillDirectoryView(DirectoryView *view) = 0;
};

}

#endif // DIRECTORYVIEWPLUGINIFACE2_H
