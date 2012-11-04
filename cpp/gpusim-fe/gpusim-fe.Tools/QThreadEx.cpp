#include "QThreadEx.h"

using namespace Tools;

QThreadEx::QThreadEx(QObject *pParent /*= nullptr*/)
    :QThread(pParent)
{

}

void QThreadEx::run()
{
    exec();
}