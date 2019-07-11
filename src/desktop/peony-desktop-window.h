#ifndef PEONYDESKTOPWINDOW_H
#define PEONYDESKTOPWINDOW_H

#include <QListWidget>

class QSettings;

namespace Fm {
    class FolderModel;
    class ProxyFolderModel;
}

class PeonyDesktopWindow : public QListView
{
    Q_OBJECT
public:
    explicit PeonyDesktopWindow(QWidget *parent = nullptr);
    ~PeonyDesktopWindow() override;

    //void setBackgroundPath(QString path);

protected:
    //void paintEvent(QPaintEvent *e) override;
    //void dropEvent(QDropEvent *e) override;

private:
    QSettings *m_settings = nullptr;

    QString bg_path = nullptr;
    QString new_bg_path = nullptr;

    //FolderModel need to be replaced by a dirved class.
    //at least, we need insert "trash, computer and personal directory"
    //to Model.
    Fm::FolderModel *m_model = nullptr;
    Fm::ProxyFolderModel *m_proxy_model = nullptr;
};

#endif
