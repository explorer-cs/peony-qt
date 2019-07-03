#include "peony-application.h"
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[]) {

    PeonyApplication app(argc, argv);
    QTranslator translator;
    app.installTranslator(&translator);
    translator.load("libfm-qt_"+QLocale::system().name(), "/usr/share/libfm-qt/translations");

    return app.exec();
}
