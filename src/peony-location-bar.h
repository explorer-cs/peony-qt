#ifndef PEONYLOCATIONSEARCHBAR_H
#define PEONYLOCATIONSEARCHBAR_H

#include <QWidget>

class QLineEdit;
class QMenu;

namespace Fm {
    class PathBar;
    class FilePath;
}

class PeonyLocationBar : public QWidget
{
    Q_OBJECT
public:
    explicit PeonyLocationBar(QWidget *parent = nullptr);
    void createLocationBar();
    Fm::PathBar *pathBar(){return m_path_bar;}

Q_SIGNALS:
    void backRequest();
    void forwardRequest();
    void historyMenuRequest(QAction *historyAction, QMenu *historyMenu);
    void goToRequest(QString location);//a Fm::FilePath? a QString?
    void cdUpRequest();

    void searchByKeyRequest(QString key);

public Q_SLOTS:
    void updateLocationBarStatus(QString location);
    void updateLocationBarStatus(const Fm::FilePath &path);

private:
    bool is_location_search_bar_created = false;
    QLineEdit *m_location_edit_line = nullptr;
    Fm::PathBar *m_path_bar = nullptr;
};

#endif // PEONYLOCATIONSEARCHBAR_H
