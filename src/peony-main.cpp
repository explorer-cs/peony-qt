#include "peony-application.h"
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[]) {

    PeonyApplication app(argc, argv);
    QTranslator translator;
    app.installTranslator(&translator);

    QTranslator ui_trans;
    app.installTranslator(&ui_trans);

    qDebug()<<QLocale::system().name();
    qDebug()<<ui_trans.load("peony-qt_zh_CN.qm", ":/");
    //qDebug()<<ui_trans.load("peony-qt_zh_CN", "/home/lanyue/git/peony-qt/src/translations");
    translator.load("libfm-qt_"+QLocale::system().name(), "/usr/share/libfm-qt/translations");

    return app.exec();
}
