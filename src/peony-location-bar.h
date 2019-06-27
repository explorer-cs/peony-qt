#ifndef PEONYLOCATIONSEARCHBAR_H
#define PEONYLOCATIONSEARCHBAR_H

#include <QWidget>

class PeonyLocationSearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit PeonyLocationSearchBar(QWidget *parent = nullptr);
    void createLocationSearchBar();

private:
    bool is_location_search_bar_created = false;
};

#endif // PEONYLOCATIONSEARCHBAR_H
