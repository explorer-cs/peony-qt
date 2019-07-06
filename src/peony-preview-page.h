#ifndef PEONYPREVIEWPAGE_H
#define PEONYPREVIEWPAGE_H

#include <QWidget>

class QHBoxLayout;

class PeonyPreviewPage : public QWidget
{
    Q_OBJECT

    enum Type {
        Text,
        PDF,
        Picture,
        OfficeDoc,
        Html,
        Other
    };

public:
    explicit PeonyPreviewPage(QWidget *parent = nullptr);

Q_SIGNALS:
    void requestEdit(QString filepath);
    //I defined a intermediary signal,
    //because i need to dynamic connect and disconnect the selection signal,
    //which controlled by preview bar show hide
    //this signal is aslo used by status bar.
    void selChanged();

public Q_SLOTS:
    void preview(QString filepath, PeonyPreviewPage::Type type = Other);
    void setDefault();

private:
    QString fileInPreview = nullptr;
    QHBoxLayout *m_layout;
    QWidget *defaultView = nullptr;
    //Text View
    //PDF View,
    //Picture View,
    //Office View,
    //Web View
};

#endif // PEONYPREVIEWPAGE_H
