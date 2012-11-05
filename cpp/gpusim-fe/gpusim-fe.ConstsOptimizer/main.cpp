#include "../gpusim-fe.Core/MetaTypes.h"
#include "Model/Optimizer.h"
#include <QtCore/QCoreApplication>

#include <../QLogger/QLogger>

void installLogger()
{
    QLogger::instance()->setLogDestination(QLogger::CLogDestination(
        QLogger::LogDest_Default | QLogger::LogDest_File));
    QLogger::instance()->setLogFormat("[%COL(SEV):5%]:[%COL(TIME):25%]:"
        "[%COL(MOD):20%]:[%COL(FUNC):40%]> %COL(MSG):0%");

    QLogger::instance()->setLogFileName("gpusim-fe.ConstsOptimizer.log");
    QLogger::instance()->setLogLevel(QLogger::LogLevel_Trace);
    QLogger::instance()->setLogsDir(QDir::current());
    QLogger::instance()->writeStartMessage();
}

void deinstallLogger()
{
    QLogger::instance()->writeStopMessage();
    QLogger::instance()->writeEndl(2);
    QLogger::instance()->deinstall();
}

//////////////////////////////////////////////////////////////////////////

int process(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Core::registerMetaTypes();
    Model::COptimizer optimizer("matrmul.txt", "gpusim-fe.ConstsOptimizer.cfg", QString());
    QMetaObject::invokeMethod(&optimizer, "optimize", Qt::QueuedConnection);
    return app.exec();
}

int main(int argc, char *argv[])
{
    installLogger();
    int iRes = process(argc, argv);
    deinstallLogger();
    system("pause");
    return iRes;
}