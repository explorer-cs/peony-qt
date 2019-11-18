#ifndef DIRECTORYVIEWEXTENSIONSTESTPLUGIN_H
#define DIRECTORYVIEWEXTENSIONSTESTPLUGIN_H

#include "directory-view-extensions-plugin-test_global.h"
#include "directory-view-plugin-iface2.h"

namespace Peony {

class DIRECTORYVIEWEXTENSIONSPLUGINTESTSHARED_EXPORT DirectoryViewExtensionsTestPlugin : public QObject, public DirectoryViewPluginIface2
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DirectoryViewPluginIface2_iid)
    Q_INTERFACES(Peony::DirectoryViewPluginIface2)
public:
    explicit DirectoryViewExtensionsTestPlugin(QObject *parent = nullptr);

    //plugin implement
    const QString name() override {return QObject::tr("Icon View2");}
    PluginType pluginType() override {return PluginType::DirectoryViewPlugin2;}
    const QString description() override {return QObject::tr("Show the folder children as icons.");}
    const QIcon icon() override {return QIcon::fromTheme("view-grid-symbolic", QIcon::fromTheme("folder"));}
    void setEnable(bool enable) override {Q_UNUSED(enable)}
    bool isEnable() override {return true;}

    //directory view plugin implemeny
    QString viewIdentity() override {return QObject::tr("Icon View2");}
    QIcon viewIcon() override {return QIcon::fromTheme("view-grid-symbolic", QIcon::fromTheme("folder"));}
    bool supportUri(const QString &uri) override {return !uri.isEmpty();}

    void fillDirectoryView(DirectoryViewWidget *view) override;

    int zoom_level_hint() override {return 100;}
    int priority(const QString &) override {return 0;}
};

}

#endif // DIRECTORYVIEWEXTENSIONSTESTPLUGIN_H
