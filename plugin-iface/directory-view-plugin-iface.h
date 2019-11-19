#ifndef DIRECTORYVIEWPLUGINIFACE_H
#define DIRECTORYVIEWPLUGINIFACE_H

#include "plugin-iface.h"

#include <QString>

#define DirectoryViewPluginIface_iid "org.ukui.peony-qt.plugin-iface.DirectoryViewPluginInterface"

namespace Peony {

class DirectoryViewIface;
class DirectoryViewProxyIface;

class FileItemModel;
class FileItemProxyFilterSortModel;

/*!
  \todo
  I have to redesgin the DirectoryView extensions framework, because
  current framework can't load DirectoryView Plugin correctly.

  The new framework will deprecate the proxy and do not create DirectoryView by it self.
  New directory view factory will just only "fill" the provided view container from peony-qt,
  and connect the view container's signal.

  I will define an instantiable QWidget interface classes providing a set of signals and slots as interface.
  The new DirectoryViewIface is a part of libpeony-qt (not only in plugin-iface), and directory plugin should include the libraries
  and implement the factory's filling function, connect the interface signal and slot to implement the UI.
  */

/*!
 * \brief The DirectoryViewPluginIface class
 * \details
 * It is a few complex to implement a peony-qt's directory view's plugin.
 * There are 3 interface class must to be implemented.
 * 1. DirectoryViewPluginIface:
 * This is the entry of a plugin, peony-qt's application will dynamicly
 * load external view modules when it started. After a plugin loaded
 * it will be registered in peony-qt's application globally, then we can
 * create such a view by create();
 * \see DirectoryViewFactoryManager
 *
 * 2. DirectoryViewIface:
 * This class define a set of methods a dervied class of directory view
 * must implement. a dervied view could most be a dervied QAbstractItemView,
 * such as QListView or QTreeView, but not certain.
 * the directory view must implement these virtual method and make them
 * seem worked in there own implement.
 * \see DirectoryViewIface
 *
 * 3. DirectoryViewProxyIface
 * \see DirectoryViewProxyIface
 *
 * \deprecated
 * This interface is deprecated.
 * To make a directory view extension,
 * you have to implement DirectoryViewPluginIface2, not this.
 *
 * \see
 * DirectoryViewPluginIface2
 */
class DirectoryViewPluginIface : public PluginInterface
{
public:
    virtual ~DirectoryViewPluginIface() {}

    virtual QString viewIdentity() = 0;
    virtual QIcon viewIcon() = 0;
    virtual bool supportUri(const QString &uri) = 0;

    /*!
     * \brief zoom_level_hint
     * \return the description value of this type of view's zoom level.
     * \details
     * Peony-Qt doesn't deal the zoom level in a kind of view. Insteadly,
     * there is a value for a view for describing its zoom level. Through
     * this zoom level, Peony-Qt can realize "zoom in/out" by switch the view's
     * type.
     *
     * We usually use the Peony-Qt's default icon view. Its zoom level hint is 100.
     * If you want to implement a larger directory view extension you should set the
     * zoom level hint bigger than the default one, vice versa.
     */
    virtual int zoom_level_hint() = 0;

    /*!
     * \brief priority
     * \return the priority of view that should be show when went to a location.
     * \details
     * In Peony-Qt, there are some special view should be shown when we went to some
     * special directory, such as computer, it should be shown as a computer view.
     * Peony-Qt first judge if it should switch the view if there is a higher-priority
     * view than current view when the location change begin.
     *
     * The default icon view and details view's priority value is 0. If you want to
     * implement an extension view replace the current view in some location, you should
     * set the priority value larger than the default one.
     */
    virtual int priority(const QString &directoryUri) = 0;

    virtual DirectoryViewIface *create() = 0;
};

/*!
 * \brief The DirectoryViewIface class
 * \details
 * This class define a set of interface for view operation.
 * In peony-qt the directory view is desgin to be extensive,
 * and every kind of view will have a global factory to create
 * the view's instance. the factories will be managed by peony-qt
 * in global, too.
 * A peony-qt's file item view will implement this interface.
 * \note
 * Every DirectoryViewIface instance should have a DirectoryViewProxyIface
 * instance, we usually use this proxy control the view.
 * \see DirectoryViewProxyIface
 *
 * \deprecated
 * This interface is deprecated.
 * To make a directory view extension,
 * you have to implement DirectoryViewPluginIface2, not this.
 *
 * \see
 * DirectoryViewPluginIface2
 */
class DirectoryViewIface
{

public:
    virtual ~DirectoryViewIface() {}

    virtual void bindModel(FileItemModel *sourceModel, FileItemProxyFilterSortModel *proxyModel) = 0;
    virtual void setProxy(DirectoryViewProxyIface *proxy) = 0;

    const virtual QString viewId() = 0;

    //location
    const virtual QString getDirectoryUri() = 0;

    //selections
    const virtual QStringList getSelections() = 0;

    //children
    const virtual QStringList getAllFileUris() = 0;

    /*
    //loaction
    void openRequest(const QStringList &uri, bool newWindow);
    void viewDirectoryChanged();
    void viewSelectionChanged();

    //menu
    void menuRequest(const QPoint &pos);

    //clipboard
    void cutSelectionsRequest();
    void copySelectionsRequest();
    void pasteSelectionsRequest();

    //zoom
    void zoomedIn();
    void zoomedOut();
    */

    //location
    virtual void open(const QStringList &uris, bool newWindow) = 0;
    virtual void setDirectoryUri(const QString &uri) = 0;
    virtual void beginLocationChange() = 0;
    virtual void stopLocationChange() = 0;
    virtual void closeView() = 0;

    //selections
    virtual void setSelections(const QStringList &uris) = 0;
    virtual void invertSelections() = 0;
    virtual void scrollToSelection(const QString &uri) = 0;

    //clipboard
    virtual void setCutFiles(const QStringList &uris) = 0;

    virtual DirectoryViewProxyIface *getProxy() = 0;

    virtual int getSortType() = 0;
    virtual void setSortType(int sortType) = 0;

    virtual int getSortOrder() = 0;
    virtual void setSortOrder(int sortOrder) = 0;

    //edit
    virtual void editUri(const QString &uri) = 0;
    /*!
     * \brief editUris
     * \param uris
     * \todo
     * implement batch rename
     */
    virtual void editUris(const QStringList uris) = 0;
};

/*!
 * \brief The DirectoryViewProxyIface class
 * \details
 * Unfortunately, Qt doesn't allow us dervied from over one QObject based classes.
 * This means we have to define our signal and slot interface outside the DirectoryViewIface.
 * This class define the signal and slot of a DirectoryViewIface should have.
 * Except the view proxy method, this class also define some higher level
 * function above the view class, such as history and clipboard.
 *
 * \note
 * You must bind a DirectoryViewIface(dervied) instance in your dervied class instance.
 * In libpeony-qt, there have been a proxy has been implemented,
 * which are used in icon view and list view by default.
 *
 * \see DirectoryViewIface, StandardViewProxy, IconView, ListView.
 *
 * \deprecated
 * This interface is deprecated.
 * To make a directory view extension,
 * you have to implement DirectoryViewPluginIface2, not this.
 *
 * \see
 * DirectoryViewPluginIface2
 */
class DirectoryViewProxyIface: public QObject
{
    Q_OBJECT
public:
    explicit DirectoryViewProxyIface(QObject *parent = nullptr) : QObject(parent) {}
    ~DirectoryViewProxyIface() {}

    virtual void switchView(DirectoryViewIface *view) = 0;

    virtual DirectoryViewIface *getView() = 0;

    //location
    const virtual QString getDirectoryUri() = 0; 

    //selections
    const virtual QStringList getSelections() = 0;

    //children
    const virtual QStringList getAllFileUris() = 0;

Q_SIGNALS:
    //loaction
    //FIXME: support open in new TAB?
    void openRequest(const QStringList &uri, bool newWindow);
    void viewDoubleClicked(const QString &uri);
    void viewDirectoryChanged();
    void viewSelectionChanged();

    //menu
    void menuRequest(const QPoint &pos);

    //window
    void updateWindowLocationRequest(const QString &uri);

    virtual int getSortType() = 0;
    virtual int getSortOrder() = 0;

public Q_SLOTS:
    //location
    virtual void open(const QStringList &uris, bool newWindow) = 0;
    virtual void setDirectoryUri(const QString &uri) = 0;
    virtual void beginLocationChange() = 0;
    virtual void stopLocationChange() = 0;

    virtual void closeProxy() = 0;

    //selections
    virtual void setSelections(const QStringList &uris) = 0;
    virtual void invertSelections() = 0;
    virtual void scrollToSelection(const QString &uri) = 0;

    //clipboard
    //cut items should be drawn differently.
    virtual void setCutFiles(const QStringList &uris) = 0;

    virtual void setSortType(int sortType) = 0;
    virtual void setSortOrder(int sortOrder) = 0;

    virtual void editUri(const QString &uri) = 0;
    virtual void editUris(const QStringList uris) = 0;
};

}

Q_DECLARE_INTERFACE(Peony::DirectoryViewPluginIface, DirectoryViewPluginIface_iid)

#endif // DIRECTORYVIEWPLUGINIFACE_H
