#pragma once

#include <QThread>

class QThreadEx : public QThread
{
    Q_OBJECT
public:
    explicit QThreadEx(QObject *pParent = nullptr);

protected:
    void run();
};