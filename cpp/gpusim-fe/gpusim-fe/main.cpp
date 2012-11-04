#include <QtGui/QApplication>

#include "../gpusim-fe.Core/MetaTypes.h"

#include "Model/Model.h"
#include "View/MainWindow/Mainwindow.h"

#include "../gpusim-fe.Tools/ModelThread.hpp"

typedef Tools::CModelThread<CModel> CModelThreadImpl;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Core::registerMetaTypes();

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

    modelThread.waitForFinish();

    return iRes;
}