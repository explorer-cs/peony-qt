#ifndef DIRECTORYVIEWEXTENSIONPLUGIN1TEST_H
#define DIRECTORYVIEWEXTENSIONPLUGIN1TEST_H

#include "directory-view-extensions-plugin1-test_global.h"

#include "directory-view-plugin-iface.h"

namespace Peony {

class DIRECTORYVIEWEXTENSIONSPLUGIN1TESTSHARED_EXPORT DirectoryViewExtensionPlugin1Test : public QObject, public DirectoryViewPluginIface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID DirectoryViewPluginIface_iid)
    Q_INTERFACES(Peony::DirectoryViewPluginIface)

public:
    explicit DirectoryViewExtensionPlugin1Test(QObject *parent =nullptr);

    //plugin implement
    const QString name() override {return QObject::tr("Icon View External");}
    PluginType pluginType() override {return PluginType::DirectoryViewPlugin;}
    const QString description() override {return QObject::tr("Show the folder children as icons.");}
    const QIcon icon() override {return QIcon::fromTheme("view-grid-symbolic", QIcon::fromTheme("folder"));}
    void setEnable(bool enable) override {Q_UNUSED(enable)}
    bool isEnable() override {return true;}

    //directory view plugin implemeny
    QString viewIdentity() override {return QObject::tr("Icon View External");}
    QIcon viewIcon() override {return QIcon::fromTheme("view-grid-symbolic", QIcon::fromTheme("folder"));}
    bool supportUri(const QString &uri) override {return !uri.isEmpty();}

    DirectoryViewIface *create() override;

    int zoom_level_hint() override {return 200;}
    int priority(const QString &) override {return 0;}
};

}

#endif // DIRECTORYVIEWEXTENSIONPLUGIN1TEST_H
