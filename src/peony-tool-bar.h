#ifndef PEONYTOOLBAR_H
#define PEONYTOOLBAR_H

#include <QWidget>

class PeonyToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit PeonyToolBar(QWidget *parent = nullptr);
    void createToolBar();

private:
    bool is_tool_bar_created = false;
};

#endif // PEONYTOOLBAR_H
