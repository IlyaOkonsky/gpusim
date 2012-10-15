#include "Model.h"
#include "Core/ExperimentGenerators/MatrixMultiplyExperimentGenerator.h"

#include <QThread>
#include <QDebug>

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 

#pragma region Private constants
const QString CModel::c_simulatorJarPath      = QString("../Simulator/gpusim-runtime.jar");
const QString CModel::c_experimentsWorkingDir = QString("../Experiments");
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
    Q_ASSERT(!m_closing);
    if (m_closing)
    {
        qWarning() << "[CModel::close()] Already canceling";
        return;
    }

    if (m_experimenter.isExecuting())
    {
        m_closing = true;
        cancelExperiment();
        return;
    }

    disconnect(&m_experimenter);
    thread()->quit();
}

void CModel::performExperiment(quint64 matrixSize, quint64 minBlockSize, quint64 maxBlockSize,
    quint64 blockSizeIncrement)
{
    CMatrixMultiplyExperimentGenerator gen(matrixSize, minBlockSize, maxBlockSize, blockSizeIncrement);
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