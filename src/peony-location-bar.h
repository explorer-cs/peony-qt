#ifndef PEONYLOCATIONSEARCHBAR_H
#define PEONYLOCATIONSEARCHBAR_H

#include <QWidget>

class QLineEdit;

class PeonyLocationSearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit PeonyLocationSearchBar(QWidget *parent = nullptr);
    void createLocationSearchBar();

Q_SIGNALS:
    void backRequest();
    void forwardRequest();
    void goToRequest(QString location);//a Fm::FilePath? a QString?
    void cdUpRequest();

    void searchByKeyRequest(QString key);

public Q_SLOTS:
    void updateLocatonBarStatus(QString location);

private:
    bool is_location_search_bar_created = false;
    QLineEdit *m_location_edit_line = nullptr;
};

#endif // PEONYLOCATIONSEARCHBAR_H
