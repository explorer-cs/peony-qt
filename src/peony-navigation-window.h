#ifndef PEONYNAVIGATIONWINDOW_H
#define PEONYNAVIGATIONWINDOW_H

#include <QWidget>
#include <QMainWindow>

#include <libfm-qt/foldermodel.h>

class BorderLayout;

class PeonyToolBar;
class PeonyLocationSearchBar;
class PeonyStatusBar;
class PeonySideBar;

class PeonyFolderView;
class PeonyPreviewPane;

class QSplitter;

class PeonyNavigationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PeonyNavigationWindow(QWidget *parent = nullptr);

    void initLayout();

    void goToUri(const QUrl &uri);
    void goToUri(const QString &uri);
    void goToUri(const char* uri);
    void goToPath(const QString &path);
    void goToPath(const char* path);
    void goToPath(Fm::FilePath path);

Q_SIGNALS:

public Q_SLOTS:

private:
    void createToolBars();
    void createStatusBar();
    void createFolderView();

    BorderLayout *m_layout = nullptr;
    QSplitter *m_view_splitter = nullptr;
};

#endif // PEONYNAVIGATIONWINDOW_H
