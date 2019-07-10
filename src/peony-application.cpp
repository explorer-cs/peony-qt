#include "peony-application.h"
#include "menu-iface.h"

#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QString>

#include "peony-navigation-window.h"
#include "peony-tool-bar.h"
#include "dbusinterface.h"

#include <QTranslator>
#include <QMessageBox>

#include "peony-folder-view.h" //testFolder

static bool is_translator_installed = false;
static void testPlugin();

PeonyApplication::PeonyApplication(int &argc, char *argv[]) : SingleApplication (argc, argv)
{
    setApplicationVersion("0.0.1");

    //when first instance init, we need decide wether peony-qt should manage desktop.
    //if we are running in ukui, it should. or it is depends on argument "--force-desktop" or
    //"--no-desktop". maybe there are more arguments, such as file paths, search request, archive options.
    //there are all need to deal.

    QIcon::setThemeName("ukui-icon-theme");

    initTranslation();

    if (!this->isPrimary()) {
        //argc = 2;
        //argv[1] = "test";
        qDebug()<<"secondary";
        this->parseCmd(arguments());
        sendMessage(this->arguments().join(' ').toUtf8());
        qDebug()<<"exit";
        return;
    } else {

        //setQuitOnLastWindowClosed(false);


        //PCManFM-Qt using a session dbus service to support command line interaction (not freedesktop one).
        //I'm considering wether I should expose such dbus service for all applications.
        //It is not quite safe enough.
        this->parseCmd(arguments());
        connect(this, &SingleApplication::receivedMessage, [=](quint32 instanceId, QByteArray msg){
            qDebug()<<"id:"<<instanceId;
            qDebug()<<"msg:"<<msg;

            QString cmdStr = msg;
            QStringList cmd = cmdStr.split(' ');
            this->parseCmd(cmd);
            //client msg from secondary application should be handled by primary application
        });
    }

    //testPlugin();
}

PeonyApplication::~PeonyApplication()
{
    if (m_dbus_iface != nullptr)
        delete m_dbus_iface;
}

void PeonyApplication::initTranslation()
{
    if (is_translator_installed)
        return;

    QTranslator *trans = new QTranslator(this);
    this->installTranslator(trans);
    is_translator_installed = trans->load("peony-qt_"+QLocale::system().name(), ":/translations");
}

void PeonyApplication::parseCmd(QStringList cmd)
{
    qDebug()<<"start prase cmd";
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption quitOption(QStringList()<<"q"<<"quit", tr("Close all peony-qt windows and quit"));
    parser.addOption(quitOption);

    QCommandLineOption deamonOption(QStringList()<<"d"<<"deamon", tr("Run application as a 'Deamon', this option only effect primary application"));
    parser.addOption(deamonOption);

    parser.addPositionalArgument("files", tr("Files or directories to open"), tr("[FILE1, FILE2,...]"));

    if (this->isPrimary()) {
        parser.process(cmd);
        if (parser.isSet(quitOption))
            qApp->quit();
        else if (parser.isSet(deamonOption)) {
            this->setQuitOnLastWindowClosed(false);
            if (m_dbus_iface == nullptr) {
                qDebug()<<"tring to register dbus service";
                m_dbus_iface = new DBusInterface;
            }
        } else {
            if (!parser.positionalArguments().isEmpty()) {
                qDebug()<<parser.positionalArguments();
                for (QString path : parser.positionalArguments()) {
                    bool isDir = false;
                    Fm::FilePath target_path = Fm::FilePath::fromPathStr(path.toUtf8());
                    if (target_path.isValid()) {
                        isDir = true;
                    } else {
                        target_path = Fm::FilePath::fromUri(path.toUtf8());
                        if (target_path.isValid())
                            isDir = true;
                    }
                    GFile *file = target_path.gfile().get();
                    if (g_file_query_file_type (file, G_FILE_QUERY_INFO_NONE, nullptr) != G_FILE_TYPE_DIRECTORY)
                        isDir = false;
                    if (!isDir) {
                        QMessageBox *msgBox = new QMessageBox;//(tr("Error"), tr("Can not find directory"));
                        msgBox->setWindowTitle(tr("ERROR"));
                        msgBox->setWindowIcon(QIcon::fromTheme("gtk-error"));
                        msgBox->setText(tr("Can not find directory: ") + path);
                        msgBox->show();
                        continue;
                    }
                    PeonyNavigationWindow *w = new PeonyNavigationWindow(target_path, nullptr);
                    w->resize(1000, 618);
                    w->show();
                }
            } else {
                //go home
                //in libfmqt desgin, cache model is a static model shared by all window now, it is a serious problem.
                //it is shared by every path bar, every folder view, that means our signal will change all window location in our desgin.
                //use Fm::FolderModel directly, rather than cache model.
                PeonyNavigationWindow *w1 = new PeonyNavigationWindow(nullptr);
                w1->resize(1000,618);
                w1->show();
            }
        }
    } else {
        parser.process(cmd);
    }
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
