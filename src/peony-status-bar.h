#ifndef PEONYSTATUSBAR_H
#define PEONYSTATUSBAR_H

#include <QWidget>
class QLabel;

class PeonyStatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit PeonyStatusBar(QWidget *parent = nullptr);
    void createStatusBar();

public Q_SLOTS:
    void updateStatusBarStatus(QString status);

private:
    bool is_status_bar_created = false;
    QLabel *m_label = nullptr;
};

#endif // PEONYSTATUSBAR_H
