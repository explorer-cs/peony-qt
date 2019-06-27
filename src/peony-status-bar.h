#ifndef PEONYSTATUSBAR_H
#define PEONYSTATUSBAR_H

#include <QWidget>

class PeonyStatusBar : public QWidget
{
    Q_OBJECT
public:
    explicit PeonyStatusBar(QWidget *parent = nullptr);
    void createStatusBar();

private:
    bool is_status_bar_created = false;
};

#endif // PEONYSTATUSBAR_H
