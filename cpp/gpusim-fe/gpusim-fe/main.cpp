#include <QtGui/QApplication>

#include "Core/MetaTypes.h"

#include "Model/ModelThread.h"
#include "Model/Model.h"
#include "View/MainWindow/Mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Core::registerMetaTypes();

    CModelThread modelThread;
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
        SIGNAL(performExperiment(quint64, quint64, quint64, quint64)),
        modelThread.getModel().data(),
        SLOT(performExperiment(quint64, quint64, quint64, quint64)),
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

    // After user closed application we must close model, but closing must be async, so call close slot from model
    // thread and wait for  model thread ends.
    //
    QEventLoop eventLoop;
    eventLoop.connect(&modelThread, SIGNAL(finished()), SLOT(quit()), Qt::QueuedConnection);
    QMetaObject::invokeMethod(modelThread.getModel().data(), "close", Qt::QueuedConnection);
    eventLoop.exec();
    return iRes;
}