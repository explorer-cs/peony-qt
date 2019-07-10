#ifndef PEONYAPPLICATION_H
#define PEONYAPPLICATION_H

/**
 * @brief A single application use client/server desgin.
 * @details single application with c/s conmunication mode is a common desgin used for many file manager, like peony, caja, and nautilus.
 * <pre>
 * There are several benefits to this type application:
 *  * Reduce wastes of resources
 *  * Make application(s) easy to manage
 *  * Reduce IPC
 * </pre>
 * @note In peony qt, single application is impliment from SingleApplication, which is based on a third part library.
 */

//#include <QApplication>
#include "singleapplication.h"

#include <QCommandLineParser>

class DBusInterface;

class PeonyApplication : public SingleApplication
{
    Q_OBJECT
public:
    PeonyApplication(int &argc, char *argv[]);
    ~PeonyApplication();
    //QTranslation should install before window created.
    void initTranslation();
    void parseCmd(QStringList cmd);

private:
    DBusInterface *m_dbus_iface = nullptr;
};

#endif // PEONYAPPLICATION_H
