#include "../gpusim-fe.Core/MetaTypes.h"

#include "Model/Model.h"
#include "View/MainWindow/Mainwindow.h"
#include "../gpusim-fe.Tools/ModelThread.hpp"

#include "../QLogger/QLogger.h"

#include <QtGui/QApplication>

//////////////////////////////////////////////////////////////////////////

void installLogger()
{
    QLogger::instance()->setLogDestination(QLogger::CLogDestination(
        QLogger::LogDest_Default | QLogger::LogDest_File));
    QLogger::instance()->setDateTimeFormat("hh:mm:ss.zzz");
    QLogger::instance()->setLogFormat("[%COL(SEV):5%]:[%COL(TIME):12%]:"
        "[%COL(MOD):20%]:[%COL(FUNC):40%]> %COL(MSG):0%");

    QLogger::instance()->setLogFileName("gpusim-fe.log");
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

typedef Tools::CModelThread<CModel> CModelThreadImpl;

int process(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Core::registerMetaTypes();

    if (app.arguments().contains("--trace"))
        QLogger::instance()->setLogLevel(QLogger::LogLevel_Trace);

    CModelThreadImpl modelThread;
    modelThread.start();
    modelThread.waitForModelReady();

    CMainWindow mainWindow;

    QObject::connect(
        modelThread.getModel().data(),
        SIGNAL(experimentProgress(quint8)),
        &mainWindow,
        SLOT(onExperimentProgress(quint8)),
        Qt::QueuedConnection);


    QObject::connect(modelThread.getModel().data(),
        SIGNAL(experimentResult(Core::CExitCode, const Core::CExperiment &)),
        &mainWindow,
        SLOT(onExperimentResult(Core::CExitCode, const Core::CExperiment &)),
        Qt::QueuedConnection);

    QObject::connect(&mainWindow,
        SIGNAL(performExperiment(quint32, quint32, quint32, quint32)),
        modelThread.getModel().data(),
        SLOT(performExperiment(quint32, quint32, quint32, quint32)),
        Qt::QueuedConnection);
    QObject::connect(&mainWindow,
        SIGNAL(cancelExperiment()),
        modelThread.getModel().data(),
        SLOT(cancelExperiment()),
        Qt::QueuedConnection);

    mainWindow.show();
    int iRes = app.exec();

    mainWindow.disconnect(modelThread.getModel().data());
    modelThread.getModel().data()->disconnect(&mainWindow);

    modelThread.waitForFinish();

    return iRes;
}

int main(int argc, char *argv[])
{
    installLogger();
    int iRes = process(argc, argv);
    deinstallLogger();
    return iRes;
}