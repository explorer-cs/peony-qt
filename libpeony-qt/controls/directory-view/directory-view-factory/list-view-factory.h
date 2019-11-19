#ifndef LISTVIEWFACTORY_H
#define LISTVIEWFACTORY_H

#include <QObject>
#include "peony-core_global.h"

#include "directory-view-plugin-iface.h"
#include "directory-view-plugin-iface2.h"

namespace Peony {

class PEONYCORESHARED_EXPORT ListViewFactory : public QObject, public DirectoryViewPluginIface
{
    Q_OBJECT
public:
    static ListViewFactory* getInstance();

    //plugin implement
    const QString name() override {return QObject::tr("List View");}
    PluginType pluginType() override {return PluginType::DirectoryViewPlugin;}
    const QString description() override {return QObject::tr("Show the folder children as rows in a list.");}
    const QIcon icon() override {return QIcon::fromTheme("view-list-symbolic", QIcon::fromTheme("folder"));}
    void setEnable(bool enable) override {Q_UNUSED(enable)}
    bool isEnable() override {return true;}

    //directory view plugin implemeny
    QString viewIdentity() override {return QObject::tr("List View");}
    QIcon viewIcon() override {return QIcon::fromTheme("view-list-symbolic", QIcon::fromTheme("folder"));}
    bool supportUri(const QString &uri) override {return !uri.isEmpty();}

    DirectoryViewIface *create() override;

    int zoom_level_hint() override {return 75;}
    int priority(const QString &) override {return 0;}

private:
    explicit ListViewFactory(QObject *parent = nullptr);
    ~ListViewFactory() override;
};

class PEONYCORESHARED_EXPORT ListViewFactory2 : public QObject, public DirectoryViewPluginIface2
{
    Q_OBJECT
public:
    static ListViewFactory2* getInstance();

    //plugin implement
    const QString name() override {return QObject::tr("List View 2");}
    PluginType pluginType() override {return PluginType::DirectoryViewPlugin;}
    const QString description() override {return QObject::tr("Show the folder children as rows in a list.");}
    const QIcon icon() override {return QIcon::fromTheme("view-list-symbolic", QIcon::fromTheme("folder"));}
    void setEnable(bool enable) override {Q_UNUSED(enable)}
    bool isEnable() override {return true;}

    //directory view plugin implemeny
    QString viewIdentity() override {return QObject::tr("List View 2");}
    QIcon viewIcon() override {return QIcon::fromTheme("view-list-symbolic", QIcon::fromTheme("folder"));}
    bool supportUri(const QString &uri) override {return !uri.isEmpty();}

    DirectoryViewWidget *create() override;

    int zoom_level_hint() override {return 75;}
    int priority(const QString &) override {return 0;}

private:
    explicit ListViewFactory2(QObject *parent = nullptr);
    ~ListViewFactory2() override;
};

}

#endif // LISTVIEWFACTORY_H
