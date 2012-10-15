#include "Experimenter.h"

#include <QDebug>

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

CExperimenter::CExperimenter(const QString &simulatorJarPath, const QString &workingDir /*= QDir::currentPath()*/,
    const CExperiment &experiment /*= CExperiment()*/, QObject *pParent /*= nullptr*/)
    :QObject(pParent), m_state(State_Ready), m_simulatorJarPath(simulatorJarPath), m_workingDir(workingDir),
    m_pSimulator(nullptr)
{
    setExperiment(experiment);
}

//////////////////////////////////////////////////////////////////////////

CExperiment CExperimenter::getExperiment() const
{
    return getExperimentRef();
}

const CExperiment & CExperimenter::getExperimentRef() const
{
    return m_experiment;
}

void CExperimenter::setExperiment(const CExperiment &experiment)
{
    Q_ASSERT(m_state == State_Ready);
    if (m_state != State_Ready)
    {
        qWarning() << "[CExperimenter::setExperiment] wrong state.";
        return;
    }

    if (m_experiment == experiment)
        return;

    m_experiment = experiment;

    //
    // Apply new experiment
    //
    m_simulatorsWorkingDir = QDir(m_workingDir).absoluteFilePath(m_experiment.getDirName());
    if (!QDir().mkpath(m_simulatorsWorkingDir))
    {
        qWarning() << "[CExperimenter::setExperiment] failed to create simulators working dir";
        return;
    }
    
    m_simulations = m_experiment.getSimulationsRef();
}

//////////////////////////////////////////////////////////////////////////

bool CExperimenter::isReady() const
{
    return (m_state == State_Ready);
}

bool CExperimenter::isExecuting() const
{
    return (m_state == State_Executing);
}

bool CExperimenter::isCanceling() const
{
    return (m_state == State_Canceling);
}

//////////////////////////////////////////////////////////////////////////

void CExperimenter::execute()
{
    Q_ASSERT(m_state == State_Ready);
    if (m_state != State_Ready)
    {
        qWarning() << "[CSimulator::execute]. Wrong state.";
        processExecuted(EC_Error);
        return;
    }

    emitProgress(0);
    m_state = State_Executing;

    m_itCurrentSim = m_simulations.begin();
    processCurrentSim();
}

void CExperimenter::execute(const CExperiment &experiment)
{
    setExperiment(experiment);
    execute();
}

void CExperimenter::cancel()
{
    if (m_state != State_Executing)
    {
        qWarning() << "[CExperimenter::cancel]: Wrong state";
        return;
    }

    m_state = State_Canceling;
    if (m_pSimulator)
    {
        m_pSimulator->cancel();
        return;
    }

    processExecuted(EC_Canceled);
}

//////////////////////////////////////////////////////////////////////////

void CExperimenter::onSimulatorProgress(Core::CSimulator *pSim, quint8 val)
{
    // total progress items = experiments count * 100
    // current items = passed_experiments count*100 + val
    
    size_t totalSimulationsCount = m_simulations.size();
    size_t passedSimulationsCount = std::distance(m_simulations.begin(), m_itCurrentSim);
    size_t p = ((passedSimulationsCount * 100 + val) * 100) / (totalSimulationsCount * 100);
    emitProgress(p);
}

void CExperimenter::onSimulatorExecuted(Core::CSimulator *pSim, Core::CExitCode ec)
{
    // 1) Update results
    *m_itCurrentSim = pSim->getSimulationRef();

    // 2) Clear current simulator
    //
    disconnect(m_pSimulator);
    delete m_pSimulator;
    m_pSimulator = nullptr;

    if (ec == EC_Ok)
    {
        m_itCurrentSim++;
        processCurrentSim();
        return;
    }

    // Here can be only error or cancel error codes
    //
    processExecuted(ec);
}

//////////////////////////////////////////////////////////////////////////

void CExperimenter::processCurrentSim()
{
    if (m_itCurrentSim == m_simulations.end())
    {
        // Simulation end with success.
        processExecuted(EC_Ok);
        return;
    }

    createSimulator(*m_itCurrentSim);

    qDebug() << "[CExperimenter::processCurrentSim]: Executing simulation " << m_itCurrentSim->getName();
    m_pSimulator->execute();
}


void CExperimenter::emitProgress(quint8 val)
{
    emit progress(this, val);
}

void CExperimenter::processExecuted(CExitCode ec)
{
    m_experiment.setSimulations(m_simulations);
    
    m_state = State_Ready;

    emitProgress(100);
    emit executed(this, ec);
}

void CExperimenter::processCancel()
{

}

void CExperimenter::createSimulator(const CSimulation &sim)
{
    if (m_pSimulator)
    {
        qWarning() << "[CExperimenter::createSimulator]: Simulator already created";
        return;
    }

    m_pSimulator = new CSimulator(m_simulatorJarPath, m_simulatorsWorkingDir, sim, this);
    connect(m_pSimulator,
        SIGNAL(progress(Core::CSimulator *, quint8)),
        SLOT(onSimulatorProgress(Core::CSimulator *, quint8)),
        Qt::QueuedConnection);
    connect(m_pSimulator,
        SIGNAL(executed(Core::CSimulator *, Core::CExitCode)),
        SLOT(onSimulatorExecuted(Core::CSimulator *, Core::CExitCode)),
        Qt::QueuedConnection);
}