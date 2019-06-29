#ifndef PEONYNAVIGATIONWINDOW_H
#define PEONYNAVIGATIONWINDOW_H

#include <QWidget>
#include <QMainWindow>

#include <libfm-qt/foldermodel.h>
#include <libfm-qt/folderview.h>

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
    void initSignal();

    void goToUri(const QUrl &uri);
    void goToUri(const QString &uri);
    void goToUri(const char* uri);
    void goToPath(const QString &path);
    void goToPath(const char* path);
    void goToPath(Fm::FilePath path);

    Fm::FileInfoList getSelection();
    Fm::Folder getWindowFolder();

public Q_SLOTS:
    /*
    void createFolder();
    void openInTerminal();
    void openInNewWindow();

    void changeViewMode(int viewmode);

    void changeSortMode(int sortmode);

    void copyToClipboard();
    void pasteFromClipboard();
    void cutToClipboard();//deal with in paste?

    void deleteSelection();

    void shareSelection();
    void burn();//
    void archiveSelection();

    void searchByKey(QString key);//cancellable

    virtual void openWithAdmin();
    virtual void openInNewTab();//tab page doesn't support yet.

    void goToFolder(Fm::Folder *folder, bool createNewWindow);
    void goBack();
    void goForward();
    */
    void cdUp();
    void updateLocationBarString(QString);

private:
    void createToolBars();
    void createStatusBar();
    void createFolderView();

    BorderLayout *m_layout = nullptr;
    QSplitter *m_view_splitter = nullptr;

    Fm::FolderView *m_folder_view = nullptr;
    PeonyToolBar *m_tool_bar = nullptr;
    PeonyLocationSearchBar *m_location_bar = nullptr;
    PeonySideBar *m_side_bar = nullptr;
    PeonyStatusBar *m_status_bar = nullptr;
    PeonyPreviewPane *m_preview_pane = nullptr;
};

#endif // PEONYNAVIGATIONWINDOW_H
