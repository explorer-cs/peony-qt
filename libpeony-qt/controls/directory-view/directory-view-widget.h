#ifndef DIRECTORYVIEWWIDGET_H
#define DIRECTORYVIEWWIDGET_H

#include <QWidget>

namespace Peony {

class FileItemModel;
class FileItemProxyFilterSortModel;
class FileItemModel;
class FileItemProxyFilterSortModel;

class DirectoryViewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirectoryViewWidget(QWidget *parent = nullptr);
    virtual ~DirectoryViewWidget() {}

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
    //virtual void open(const QStringList &uris, bool newWindow) {}
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

}

#endif // DIRECTORYVIEWWIDGET_H
