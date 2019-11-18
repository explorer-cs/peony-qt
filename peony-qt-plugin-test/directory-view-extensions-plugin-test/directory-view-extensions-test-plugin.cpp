#include "directory-view-extensions-test-plugin.h"
#include "directory-view-widget.h"

#include <QLabel>

#include <QVBoxLayout>

using namespace Peony;

DirectoryViewExtensionsTestPlugin::DirectoryViewExtensionsTestPlugin(QObject *parent) : QObject(parent)
{

}

void DirectoryViewExtensionsTestPlugin::fillDirectoryView(DirectoryViewWidget *view)
{
    auto layout = new QVBoxLayout(view);
    layout->addWidget(new QLabel("test icon view", nullptr));
    view->setLayout(layout);
}
