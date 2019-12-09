/*
 * Peony-Qt's Library
 *
 * Copyright (C) 2019, Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this library.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Authors: Yue Lan <lanyue@kylinos.cn>
 *
 */

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

    DirectoryViewWidget *create() override;

    int zoom_level_hint() override {return 75;}
    int priority(const QString &) override {return 0;}

private:
    explicit ListViewFactory2(QObject *parent = nullptr);
    ~ListViewFactory2() override;
};

}

#endif // LISTVIEWFACTORY_H
