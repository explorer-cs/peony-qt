#ifndef PEONYTOOLBAR_H
#define PEONYTOOLBAR_H

#include <QWidget>
#include <libfm-qt/folderview.h>
#include <libfm-qt/foldermodel.h>

class PeonyToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit PeonyToolBar(QWidget *parent = nullptr);
    void createToolBar();

Q_SIGNALS:
    void createFolderRequest();
    void newWindowRequest();
    void openInTerminalRequest();

    void changeViewModeRequest(Fm::FolderView::ViewMode viewmode);
    void changeSortModeRequest(Fm::FolderModel::ColumnId sortmode);

    //maybe use clipboard provided by libfm-qt is better.
    void copyToClipboardRequest();
    void pasteFromClipboardRequest();
    void cutToClipboradRequest();

    void deleteSelectionRequest();

    void shareSelectionRequest();
    void burnRequest();
    void archiveSeletionRequest();

    void searchRequest(QString key);

    //more

private:
    bool is_tool_bar_created = false;
};

#endif // PEONYTOOLBAR_H
