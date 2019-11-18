#include "directoryviewextensionplugin1test.h"

#include "icon-view.h"

using namespace Peony;

DirectoryViewExtensionPlugin1Test::DirectoryViewExtensionPlugin1Test(QObject *parent) : QObject(parent)
{

}

DirectoryViewIface *DirectoryViewExtensionPlugin1Test::create()
{
    return new DirectoryView::IconView;
}
