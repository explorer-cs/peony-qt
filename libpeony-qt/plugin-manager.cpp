#include "plugin-manager.h"

#include "menu-plugin-manager.h"
#include "directory-view-factory-manager.h"
#include "preview-page-factory-manager.h"

#include "directory-view-plugin-iface.h"
#include "directory-view-plugin-iface2.h"
#include "preview-page-plugin-iface.h"
#include "style-plugin-iface.h"

#include "properties-window.h" //properties factory manager define is in this header
#include "properties-window-tab-page-plugin-iface.h"

#include "directory-view-widget.h"

#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include <QProxyStyle>

using namespace Peony;

static PluginManager *global_instance = nullptr;

PluginManager::PluginManager(QObject *parent) : QObject(parent)
{
    QDir pluginsDir("/usr/lib/peony-qt-extensions");
    pluginsDir.setFilter(QDir::Files);

    qDebug()<<pluginsDir.entryList().count();
    Q_FOREACH(QString fileName, pluginsDir.entryList(QDir::Files)) {
        qDebug()<<fileName;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        qDebug()<<pluginLoader.fileName();
        qDebug()<<pluginLoader.metaData();
        qDebug()<<pluginLoader.load();
        QObject *plugin = pluginLoader.instance();
        if (!plugin)
            continue;
        qDebug()<<"test start";
        PluginInterface *piface = dynamic_cast<PluginInterface*>(plugin);
        if (!piface)
            continue;
        m_hash.insert(piface->name(), piface);
        switch (piface->pluginType()) {
        case PluginInterface::MenuPlugin: {
            MenuPluginInterface *menuPlugin = dynamic_cast<MenuPluginInterface*>(piface);
            MenuPluginManager::getInstance()->registerPlugin(menuPlugin);
            break;
        }
        case PluginInterface::DirectoryViewPlugin: {
            DirectoryViewPluginIface *directoryViewFactory = dynamic_cast<DirectoryViewPluginIface*>(plugin);
            DirectoryViewFactoryManager::getInstance()->registerFactory(directoryViewFactory->viewIdentity(), directoryViewFactory);
            break;
        }
        case PluginInterface::PreviewPagePlugin: {
            PreviewPagePluginIface *previewPageFactory = dynamic_cast<PreviewPagePluginIface*>(plugin);
            PreviewPageFactoryManager::getInstance()->registerFactory(previewPageFactory->name(), previewPageFactory);
            break;
        }
        case PluginInterface::PropertiesWindowPlugin: {
            PropertiesWindowTabPagePluginIface *propertiesWindowTabPageFactory = dynamic_cast<PropertiesWindowTabPagePluginIface*>(plugin);
            PropertiesWindowPluginManager::getInstance()->registerFactory(propertiesWindowTabPageFactory);
            break;
        }
        case PluginInterface::ColumnProviderPlugin: {
            //FIXME:
            break;
        }
        case  PluginInterface::StylePlugin: {
            /*!
              \todo
              manage the style plugin
              */
            auto styleProvider = dynamic_cast<StylePluginIface*>(plugin);
            QApplication::setStyle(styleProvider->getStyle());
            break;
        }
        case PluginInterface::DirectoryViewPlugin2: {
            auto p = dynamic_cast<DirectoryViewPluginIface2*>(plugin);
            auto w = new DirectoryViewWidget;
            p->fillDirectoryView(w);
            w->show();
            break;
        }
        default:
            break;
        }
    }
}

PluginManager::~PluginManager()
{
    m_hash.clear();
    MenuPluginManager::getInstance()->close();
    //FIXME: use private deconstructor.
    DirectoryViewFactoryManager::getInstance()->deleteLater();
    PreviewPageFactoryManager::getInstance()->deleteLater();
}

PluginManager *PluginManager::getInstance()
{
    if (!global_instance) {
        global_instance = new PluginManager;
    }
    return global_instance;
}

void PluginManager::setPluginEnableByName(const QString &name, bool enable)
{
    m_hash.value(name)->setEnable(enable);
}

void PluginManager::close()
{
    if (global_instance)
        global_instance->deleteLater();
}

void PluginManager::init()
{
    PluginManager::getInstance();
}
