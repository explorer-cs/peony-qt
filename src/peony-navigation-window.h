#ifndef PEONYNAVIGATIONWINDOW_H
#define PEONYNAVIGATIONWINDOW_H

#include <QWidget>
#include <QMainWindow>

#include <libfm-qt/foldermodel.h>
#include <libfm-qt/folderview.h>
#include <libfm-qt/proxyfoldermodel.h>

class BorderLayout;

class PeonyToolBar;
class PeonyLocationBar;
class PeonyStatusBar;
class PeonySideBar;

class PeonyFolderView;
class PeonyPreviewPage;

class QSplitter;

enum LastFileOpType {
    Copy,
    Cut,
    Other
};

class PeonyNavigationWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PeonyNavigationWindow(QWidget *parent = nullptr);
    explicit PeonyNavigationWindow(Fm::FilePath target_dir = Fm::FilePath::homeDir() ,QWidget *parent = nullptr);

    void initLayout();
    void initSignal();

    void goToUri(const QUrl &uri);
    void goToUri(const QString &uri);
    void goToUri(const char* uri);
    void goToPath(const QString &path);
    void goToPath(const char* path);
    void goToPath(const Fm::FilePath &path, bool addHistory);

    Fm::FileInfoList getSelection();
    Fm::Folder getWindowFolder();

public Q_SLOTS:
    /*
    void createFolder();
    void openInTerminal();
    void openInNewWindow();

    void changeViewMode(int viewmode);

    void changeSortMode(int sortmode);

    void shareSelection();
    void burn();//
    void archiveSelection();

    void searchByKey(QString key);//cancellable

    virtual void openWithAdmin();
    virtual void openInNewTab();//tab page doesn't support yet.

    void goToFolder(Fm::Folder *folder, bool createNewWindow);
    */
    void copyToClipboard();
    void pasteFromClipboard();
    void cutToClipboard();//deal with in paste?

    void deleteSelection();

    void goBack();
    void goForward();
    void showHistoryMenu(QAction *listHistoryAction, QMenu *historyMenu);

    void cdUp();
    void updateLocationBarString(QString);
    //when a folder clicked a folder view, it aslo need update path bar.
    //I real think it wasn't a good desgin... there must be a better way.
    //this desgin is unfriendly for history
    void updateLocationBarPath(Fm::FilePath path);

private:
    void createToolBars();
    void createStatusBar();
    void createFolderView();

    BorderLayout *m_layout = nullptr;
    QSplitter *m_view_splitter = nullptr;

    PeonyFolderView *m_folder_view = nullptr;
    PeonyToolBar *m_tool_bar = nullptr;
    PeonyLocationBar *m_location_bar = nullptr;
    PeonySideBar *m_side_bar = nullptr;
    PeonyStatusBar *m_status_bar = nullptr;
    PeonyPreviewPage *m_preview_page = nullptr;

    Fm::FilePathList m_forward_list;
    Fm::FilePathList m_backward_list;

    Fm::FilePathList m_clipborad_list;
    LastFileOpType last_file_op_type = Other;

    Fm::ProxyFolderModelFilter *m_filter = nullptr;
};

#endif // PEONYNAVIGATIONWINDOW_H
