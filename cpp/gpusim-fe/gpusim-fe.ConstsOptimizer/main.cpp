#include "../gpusim-fe.Core/MetaTypes.h"
#include "Model/Optimizer.h"
#include <QtCore/QCoreApplication>

#include <../QLogger/QLogger>
#include <iostream>

void installLogger()
{
    QLogger::instance()->setLogDestination(QLogger::CLogDestination(
        QLogger::LogDest_Default | QLogger::LogDest_File));
    QLogger::instance()->setDateTimeFormat("hh:mm:ss.zzz");
    QLogger::instance()->setLogFormat("[%COL(SEV):5%]:[%COL(TIME):12%]:"
        "[%COL(MOD):20%]:[%COL(FUNC):40%]> %COL(MSG):0%");

    QLogger::instance()->setLogFileName("gpusim-fe.ConstsOptimizer.log");
    QLogger::instance()->setLogLevel(QLogger::LogLevel_Debug);
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

    if (app.arguments().contains("--trace"))
        QLogger::instance()->setLogLevel(QLogger::LogLevel_Trace);

    Model::COptimizer optimizer("matrmul.txt", "gpusim-fe.ConstsOptimizer.cfg", "bestSettings.xml");
    QMetaObject::invokeMethod(&optimizer, "optimize", Qt::QueuedConnection);
    return app.exec();
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cout << "Parameters not set. usage:" << std::endl;
        std::cout << "gpusim-fe.ConstsOptimizer <originals_file_path> <config_file_path> <best_settings_filePath> " <<
            "[--trace]" << std::endl;
        system("pause");
        return 0;
    }

    installLogger();
    int iRes = process(argc, argv);
    deinstallLogger();
    system("pause");
    return iRes;
}