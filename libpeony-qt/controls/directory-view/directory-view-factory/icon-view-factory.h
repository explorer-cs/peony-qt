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

#ifndef ICONVIEWFACTORY_H
#define ICONVIEWFACTORY_H

#include "directory-view-plugin-iface.h"
#include "directory-view-plugin-iface2.h"
#include <QObject>

namespace Peony {

class IconViewFactory : public QObject, public DirectoryViewPluginIface
{
    Q_OBJECT
public:
    static IconViewFactory *getInstance();

    //plugin implement
    const QString name() override {return QObject::tr("Icon View");}
    PluginType pluginType() override {return PluginType::DirectoryViewPlugin;}
    const QString description() override {return QObject::tr("Show the folder children as icons.");}
    const QIcon icon() override {return QIcon::fromTheme("view-grid-symbolic", QIcon::fromTheme("folder"));}
    void setEnable(bool enable) override {Q_UNUSED(enable)}
    bool isEnable() override {return true;}

    //directory view plugin implemeny
    QString viewIdentity() override {return QObject::tr("Icon View");}
    QIcon viewIcon() override {return QIcon::fromTheme("view-grid-symbolic", QIcon::fromTheme("folder"));}
    bool supportUri(const QString &uri) override {return !uri.isEmpty();}

    DirectoryViewIface *create() override;

    int zoom_level_hint() override {return 100;}
    int priority(const QString &) override {return 0;}

private:
    explicit IconViewFactory(QObject *parent = nullptr);
    ~IconViewFactory() override;
};

class IconViewFactory2 : public QObject, public DirectoryViewPluginIface2
{
    Q_OBJECT
public:
    static IconViewFactory2 *getInstance();

    //plugin implement
    const QString name() override {return QObject::tr("Icon View");}
    PluginType pluginType() override {return PluginType::DirectoryViewPlugin;}
    const QString description() override {return QObject::tr("Show the folder children as icons.");}
    const QIcon icon() override {return QIcon::fromTheme("view-grid-symbolic", QIcon::fromTheme("folder"));}
    void setEnable(bool enable) override {Q_UNUSED(enable)}
    bool isEnable() override {return true;}

    //directory view plugin implemeny
    QString viewIdentity() override {return QObject::tr("Icon View");}
    QIcon viewIcon() override {return QIcon::fromTheme("view-grid-symbolic", QIcon::fromTheme("folder"));}
    bool supportUri(const QString &uri) override {return !uri.isEmpty();}

    DirectoryViewWidget *create() override;

    int zoom_level_hint() override {return 100;}
    int priority(const QString &) override {return 0;}

private:
    explicit IconViewFactory2(QObject *parent = nullptr);
    ~IconViewFactory2() override;
};

}

#endif // ICONVIEWFACTORY_H
