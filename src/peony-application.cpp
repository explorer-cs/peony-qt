#include "peony-application.h"
#include "menu-iface.h"

#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QString>

#include "peony-navigation-window.h"
#include "peony-tool-bar.h"

#include <QTranslator>

#include "peony-folder-view.h" //testFolder

static bool is_translator_installed = false;
static void testPlugin();

PeonyApplication::PeonyApplication(int argc, char *argv[]) : SingleApplication (argc, argv)
{
    if (!this->isPrimary()) {
        //argc = 2;
        //argv[1] = "test";
        qDebug()<<"secondary";
        sendMessage(this->arguments().join(' ').toUtf8());
        qDebug()<<"exit";
        return;
    } else {
        //PCManFM-Qt using a session dbus service to support command line interaction (not freedesktop one).
        //I'm considering wether I should expose such dbus service for all applications.
        //It is not quite safe enough.
        connect(this, &SingleApplication::receivedMessage, [=](quint32 instanceId, QByteArray msg){
            qDebug()<<"id:"<<instanceId;
            qDebug()<<"msg:"<<msg;

            //client msg from secondary application should be handled by primary application
        });
    }

    //when first instance init, we need decide wether peony-qt should manage desktop.
    //if we are running in ukui, it should. or it is depends on argument "--force-desktop" or
    //"--no-desktop". maybe there are more arguments, such as file paths, search request, archive options.
    //there are all need to deal.

    QIcon::setThemeName("ukui-icon-theme");

    initTranslation();
    //testPlugin();

    //in libfmqt desgin, cache model is a static model shared by all window now, it is a serious problem.
    //it is shared by every path bar, every folder view, that means our signal will change all window location in our desgin.
    //use Fm::FolderModel directly, rather than cache model.
    PeonyNavigationWindow *w1 = new PeonyNavigationWindow(nullptr);
    w1->resize(1000,618);
    w1->show();

    //PeonyNavigationWindow *w2 = new PeonyNavigationWindow(Fm::FilePath::fromUri("computer:///"), nullptr);
    //w2->show();

}

void PeonyApplication::initTranslation()
{
    if (is_translator_installed)
        return;

    QTranslator *trans = new QTranslator(this);
    this->installTranslator(trans);
    is_translator_installed = trans->load("peony-qt_"+QLocale::system().name(), ":/translations");
}

void testPlugin()
{
    //check if first run
    //if not send message to server
    //else
    //  load plgin
    //  read from command line
    //  do with args
    QDir pluginsDir(qApp->applicationDirPath());
    qDebug()<<pluginsDir;
    pluginsDir.cdUp();
    pluginsDir.cd("testdir");
    pluginsDir.setFilter(QDir::Files);

    qDebug()<<pluginsDir.entryList().count();
    Q_FOREACH(QString fileName, pluginsDir.entryList(QDir::Files)) {
        qDebug()<<fileName;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        qDebug()<<pluginLoader.fileName();
        qDebug()<<pluginLoader.metaData();
        qDebug()<<pluginLoader.load();
        QObject *plugin = pluginLoader.instance();
        qDebug()<<"test start";
        if (plugin) {
            MenuInterface *iface = qobject_cast<MenuInterface *>(plugin);
            if (iface)
                qDebug()<<iface->testPlugin();
        }
        qDebug()<<"testEnd";
    }
}
