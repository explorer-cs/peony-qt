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

class PeonyApplication : public SingleApplication
{
    Q_OBJECT
public:
    PeonyApplication(int &argc, char *argv[]);
    //QTranslation should install before window created.
    void initTranslation();
};

#endif // PEONYAPPLICATION_H
