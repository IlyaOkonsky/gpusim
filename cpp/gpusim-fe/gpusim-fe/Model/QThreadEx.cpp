#include "qthreadex.h"

QThreadEx::QThreadEx(QObject *pParent /*= nullptr*/)
    :QThread(pParent)
{

}

void QThreadEx::run()
{
    exec();
}