#include "Model.h"
#include "../../gpusim-fe.Core/ExperimentGenerators/MatrixMultiplyExperimentGenerator.h"
#include "../../gpusim-fe.Core/ExperimentGenerators/NBodyExperimentGenerator.h"

#include "../QLogger/QLog"

#include <QThread>

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 

#pragma region Private constants
const QString CModel::c_genSettingsFilePath       = QString("NBEGSettings.xml");
const QString CModel::c_simulatorJarPath          = QString("../Simulator/gpusim-runtime.jar");
const QString CModel::c_experimentsWorkingDir     = QString("../Experiments");

#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

CModel::CModel(QObject *pParent /*= nullptr*/)
    :QObject(pParent), m_state(State_Ready), m_closing(false), m_experimenter(c_simulatorJarPath,
    c_experimentsWorkingDir, CExperiment(), this)
{
    connect(&m_experimenter,
        SIGNAL(progress(Core::CExperimenter *, quint8)),
        SLOT(onExperimeterProgress(Core::CExperimenter *, quint8)),
        Qt::QueuedConnection);

    connect(&m_experimenter,
        SIGNAL(executed(Core::CExperimenter *, Core::CExitCode)),
        SLOT(onExperimeterExecuted(Core::CExperimenter *, Core::CExitCode)),
        Qt::QueuedConnection);
}

void CModel::close()
{
    if (m_experimenter.isExecuting())
    {
        m_closing = true;
        cancelExperiment();
        return;
    }

    disconnect(&m_experimenter);
    thread()->quit();
}

void CModel::performExperiment(quint32 minN, quint32 maxN, quint32 minThreadsPerBlock, quint32 maxThreadsPerBlock)
{
    if ((minN != maxN) && (minThreadsPerBlock != maxThreadsPerBlock))
    {
        qLog_WarningMsg() << "Only one parameter (N or threads per block can be changed, not both).";
        emit experimentProgress(100);
        emit experimentResult(EC_Error);
    }

    Settings::CNBodyExperimentGeneratorSettings settings;
    if (!settings.loadFromFile(c_genSettingsFilePath))
    {
        qLog_WarningMsg() << "Failed to load NBEG settings.";
        emit experimentProgress(100);
        emit experimentResult(EC_Error);
    }

    CNBodyExperimentGenerator gen(settings, minN, maxN, 2, minThreadsPerBlock, maxThreadsPerBlock, 2);
    m_experimenter.execute(gen.generate());
}

void CModel::cancelExperiment()
{
    m_experimenter.cancel();
}

//////////////////////////////////////////////////////////////////////////

void CModel::onExperimeterProgress(Core::CExperimenter *pExperimenter, quint8 val)
{
    emit experimentProgress(val);
}

void CModel::onExperimeterExecuted(Core::CExperimenter *pExperimenter, Core::CExitCode ec)
{
    if (m_closing)
    {
        close();
        return;
    }

    if (ec != EC_Ok)
    {
        emit experimentResult(ec);
        return;
    }

    emit experimentResult(ec, pExperimenter->getExperimentRef());
}