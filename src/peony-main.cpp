#include "peony-application.h"
#include <QTranslator>
#include <QDebug>

#include <libfm-qt/libfmqt.h> //register search vfs and menu vfs.

int main(int argc, char *argv[]) {

    PeonyApplication app(argc, argv);
    if (app.isPrimary()) {
        Fm::LibFmQt context;

        QTranslator translator;
        app.installTranslator(context.translator());
        //app.installTranslator(&translator);
        //translator.load("libfm-qt_"+QLocale::system().name(), "/usr/share/libfm-qt/translations");

        return app.exec();
    }
}
