#include "../gpusim-fe.Core/MetaTypes.h"

#include <QtCore/QCoreApplication>

int main(int argc, char *argv[])
{
    Core::registerMetaTypes();

    QCoreApplication a(argc, argv);

    return a.exec();
}