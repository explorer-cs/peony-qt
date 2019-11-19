#include "directory-view-container.h"
#include "directory-view-plugin-iface.h"
#include "directory-view-plugin-iface2.h"
#include "directory-view-widget.h"
#include "directory-view-factory-manager.h"
#include "standard-view-proxy.h"
#include "file-utils.h"

#include "directory-view-factory-manager.h"

#include "file-item-proxy-filter-sort-model.h"

#include <QVBoxLayout>

using namespace Peony;

DirectoryViewContainer::DirectoryViewContainer(QWidget *parent) : QWidget(parent)
{
    m_model = new FileItemModel(this);
    m_proxy_model = new FileItemProxyFilterSortModel(this);
    m_proxy_model->setSourceModel(m_model);

    //m_proxy = new DirectoryView::StandardViewProxy;

    setContentsMargins(0, 0, 0, 0);
    m_layout = new QVBoxLayout(this);
    m_layout->setMargin(0);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    setLayout(m_layout);

//    connect(m_proxy, &DirectoryViewProxyIface::viewDirectoryChanged,
//            this, &DirectoryViewContainer::directoryChanged);

//    connect(m_proxy, &DirectoryViewProxyIface::viewSelectionChanged,
//            this, &DirectoryViewContainer::selectionChanged);

//    connect(m_proxy, &DirectoryViewProxyIface::menuRequest,
//            this, &DirectoryViewContainer::menuRequest);

    auto viewId = DirectoryViewFactoryManager2::getInstance()->getDefaultViewId();
    switchViewType(viewId);
}

DirectoryViewContainer::~DirectoryViewContainer()
{
//    m_proxy->closeProxy();
//    if (m_proxy->getView())
//        m_proxy->getView()->closeView();
}

const QStringList DirectoryViewContainer::getBackList()
{
    QStringList l;
    for (auto uri : m_back_list) {
        l<<uri;
    }
    return l;
}

const QStringList DirectoryViewContainer::getForwardList()
{
    QStringList l;
    for (auto uri : m_forward_list) {
        l<<uri;
    }
    return l;
}

bool DirectoryViewContainer::canGoBack()
{
    return !m_back_list.isEmpty();
}

void DirectoryViewContainer::goBack()
{
    if (!canGoBack())
        return;

    auto uri = m_back_list.takeLast();
    m_forward_list.prepend(getCurrentUri());
    Q_EMIT updateWindowLocationRequest(uri, false);
}

bool DirectoryViewContainer::canGoForward()
{
    return !m_forward_list.isEmpty();
}

void DirectoryViewContainer::goForward()
{
    if (!canGoForward())
        return;

    auto uri = m_forward_list.takeFirst();
    m_back_list.append(getCurrentUri());

    Q_EMIT updateWindowLocationRequest(uri, false);
}

bool DirectoryViewContainer::canCdUp()
{
    if (!m_view)
        return false;
    return !FileUtils::getParentUri(m_view->getDirectoryUri()).isNull();
}

void DirectoryViewContainer::cdUp()
{
    if (!canCdUp())
        return;

    auto uri = FileUtils::getParentUri(m_view->getDirectoryUri());
    if (uri.isNull())
        return;

    Q_EMIT updateWindowLocationRequest(uri, true);
}

void DirectoryViewContainer::setSortFilter(int FileTypeIndex, int FileMTimeIndex, int FileSizeIndex)
{
    qDebug()<<"setSortFilter:"<<FileTypeIndex<<"MTime:"<<FileMTimeIndex<<"size:"<<FileSizeIndex;
    m_proxy_model->setFilterConditions(FileTypeIndex, FileMTimeIndex, FileSizeIndex);
}

void DirectoryViewContainer::setShowHidden(bool showHidden)
{
    m_proxy_model->setShowHidden(showHidden);
}

void DirectoryViewContainer::goToUri(const QString &uri, bool addHistory, bool forceUpdate)
{
    if (forceUpdate)
        goto update;

    if (uri.isNull())
        return;

    if (getCurrentUri() == uri)
        return;

update:
    if (addHistory) {
        m_forward_list.clear();
        m_back_list.append(getCurrentUri());
    }

    m_current_uri = uri;

    if (m_view) {
        m_view->setDirectoryUri(uri);
        m_view->beginLocationChange();
        //m_active_view_prxoy->setDirectoryUri(uri);
    }
}

void DirectoryViewContainer::switchViewType(const QString &viewId)
{
    /*
    if (!m_proxy)
        return;

    if (m_proxy->getView()) {
        if (viewId == m_proxy->getView()->viewId())
            return;
    }
    */

    auto viewManager = DirectoryViewFactoryManager2::getInstance();
    auto factory = viewManager->getFactory(viewId);
    if (!factory)
        return;

    auto sortType = 0;
    auto sortOrder = 0;

    auto oldView = m_view;
    QStringList selection;
    if (oldView) {
        sortType = oldView->getSortType();
        sortOrder = oldView->getSortOrder();
        selection = oldView->getSelections();
        m_layout->removeWidget(dynamic_cast<QWidget*>(oldView));
    }
    auto view = factory->create();
    m_view = view;
    //connect the view's signal.
    view->bindModel(m_model, m_proxy_model);
    //view->setProxy(m_proxy);

    view->setSortType(sortType);
    view->setSortOrder(sortOrder);

    connect(m_view, &DirectoryViewWidget::menuRequest, this, &DirectoryViewContainer::menuRequest);
    connect(m_view, &DirectoryViewWidget::viewDirectoryChanged, this, &DirectoryViewContainer::directoryChanged);
    connect(m_view, &DirectoryViewWidget::viewDoubleClicked, this, &DirectoryViewContainer::viewDoubleClicked);

    //m_proxy->switchView(view);
    m_layout->addWidget(dynamic_cast<QWidget*>(view), Qt::AlignBottom);
    DirectoryViewFactoryManager2::getInstance()->setDefaultViewId(viewId);
    if (!selection.isEmpty()) {
        view->setSelections(selection);
    }

    Q_EMIT viewTypeChanged();
}

void DirectoryViewContainer::refresh()
{
    if (!m_view)
        return;
    m_view->beginLocationChange();
}

void DirectoryViewContainer::bindNewProxy(DirectoryViewProxyIface *proxy)
{
    //disconnect old proxy
    //connect new proxy
}

const QStringList DirectoryViewContainer::getCurrentSelections()
{
    if (m_view)
        return m_view->getSelections();
    return QStringList();
}

const QString DirectoryViewContainer::getCurrentUri()
{
    if (m_view) {
        return m_view->getDirectoryUri();
    }
    return nullptr;
}

const QStringList DirectoryViewContainer::getAllFileUris()
{
    if (m_view)
        return m_view->getAllFileUris();
    return QStringList();
}

void DirectoryViewContainer::stopLoading()
{
    if (m_view) {
        m_view->stopLocationChange();
    }
}

void DirectoryViewContainer::tryJump(int index)
{
    QStringList l;
    l<<m_back_list<<getCurrentUri()<<m_forward_list;
    if (0 <= index && index < l.count()) {
        auto targetUri = l.at(index);
        m_back_list.clear();
        m_forward_list.clear();
        for (int i = 0; i < l.count(); i++) {
            if (i < index) {
                m_back_list<<l.at(i);
            }
            if (i > index) {
                m_forward_list<<l.at(i);
            }
        }
        Q_EMIT updateWindowLocationRequest(targetUri, false, true);
    }
}

FileItemModel::ColumnType DirectoryViewContainer::getSortType()
{
    if (!m_view)
        return FileItemModel::FileName;
    int type = m_view->getSortType();
    return FileItemModel::ColumnType(type);
}

void DirectoryViewContainer::setSortType(FileItemModel::ColumnType type)
{
    if (!m_view)
        return;
    m_view->setSortType(type);
}

Qt::SortOrder DirectoryViewContainer::getSortOrder()
{
    if (!m_view)
        return Qt::AscendingOrder;
    int order = m_view->getSortOrder();
    return Qt::SortOrder(order);
}

void DirectoryViewContainer::setSortOrder(Qt::SortOrder order)
{
    if (!m_view)
        return;
    m_view->setSortOrder(order);
}
