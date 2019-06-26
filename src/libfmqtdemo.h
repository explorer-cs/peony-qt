#ifndef LIBFMQTDEMO_H
#define LIBFMQTDEMO_H

#include <QObject>

class LibFmQtDemo : public QObject
{
    Q_OBJECT
public:
    explicit LibFmQtDemo(QObject *parent = nullptr);

Q_SIGNALS:

public Q_SLOTS:
};

#endif // LIBFMQTDEMO_H
