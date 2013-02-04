#include "Simulator.h"

#include "../QLogger/QLog"

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CSimulator::c_configFileNameFormat = QString("Simulation_%1.config.xml");
const QString CSimulator::c_outputFileNameFormat = QString("Simulation_%1.output.xml");

//java -jar "<jar-file-path>" "<config-file-path>" "<output-file-path>"
const QString CSimulator::c_javaProcessExecFormat = QString("java -Xms1024m -Xmx1024m -jar \"%1\" \"%2\" \"%3\"");

#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

CSimulator::CSimulator(const QString &simulatorJarPath, const QString &workingDir /*= QDir::currentPath()*/,
    const CSimulation &simulation /*= CSimulation()*/, QObject *pParent /*= nullptr*/)
    :QObject(pParent), m_state(State_Ready), m_simulatorJarPath(simulatorJarPath), m_workingDir(workingDir),
    m_pJavaProcess(nullptr)
{
    setSimulation(simulation);
}

CSimulation CSimulator::getSimulation() const
{
    return m_simulation;
}

const CSimulation & CSimulator::getSimulationRef() const
{
    return m_simulation;
}

void CSimulator::setSimulation(const CSimulation &simulation)
{
    Q_ASSERT(m_state == State_Ready);
    if (m_state != State_Ready)
    {
        qLog_WarningMsg() << "Wrong state.";
        return;
    }

    if (m_simulation == simulation)
        return;

    m_simulation = simulation;

    //
    // Apply new simulation
    //
    m_configFilePath = buildFullSimualtionFilePath(m_workingDir, c_configFileNameFormat, m_simulation.getNumber());
    m_outputFilePath = buildFullSimualtionFilePath(m_workingDir, c_outputFileNameFormat, m_simulation.getNumber());

    m_pJavaProcess = new QProcess(this);
    connect(m_pJavaProcess,
        SIGNAL(error(QProcess::ProcessError)),
        SLOT(onJavaProcessError(QProcess::ProcessError)),
        Qt::QueuedConnection);
    connect(m_pJavaProcess,
        SIGNAL(started()),
        SLOT(onJavaProcessStarted()),
        Qt::QueuedConnection);
    connect(m_pJavaProcess,
        SIGNAL(finished(int, QProcess::ExitStatus)),
        SLOT(onJavaProcessFinished(int, QProcess::ExitStatus)),
        Qt::QueuedConnection);
}

void CSimulator::execute()
{
    Q_ASSERT(m_state == State_Ready);
    if (m_state != State_Ready)
    {
        qLog_WarningMsg() << "Wrong state.";
        processExecuted(EC_Error);
        return;
    }

    emitProgress(0);
    if (!m_simulation.getConfigRef().saveToFile(m_configFilePath))
    {
        qLog_WarningMsg() << "Failed to save configuration to file.";
        processExecuted(EC_Error);
        return;
    }

    QString execString = c_javaProcessExecFormat.arg(m_simulatorJarPath, m_configFilePath, m_outputFilePath);
    m_pJavaProcess->start(execString);

    m_state = State_StartingJavaProcess;
    emitProgress(10);
}

void CSimulator::execute(const CSimulation &simulation)
{
    setSimulation(simulation);
    execute();
}

void CSimulator::cancel()
{
    if ((m_state == State_Ready) || (m_state == State_Canceling))
    {
        qLog_WarningMsg() << "Wrong state.";
        return;
    }

    CState prevState = m_state;
    m_state = State_Canceling;

    // Cancellation can be done after process will be started
    //
    if (prevState == State_StartingJavaProcess)
        return;

    processCancel();
}

//////////////////////////////////////////////////////////////////////////

void CSimulator::onJavaProcessError(QProcess::ProcessError error)
{
    // Ignore errors when cancel. Process process termination in onJavaProcessFinished method
    //
    if (m_state == State_Canceling)
        return;

    qLog_WarningMsg() << "Process error: " << error;
    processExecuted(EC_Error);
}

void CSimulator::onJavaProcessStarted()
{
    if (m_state == State_Canceling)
    {
        processCancel();
        return;
    }

    m_state = State_WaitJavaProcessExecResult;
    emitProgress(20);
}

void CSimulator::onJavaProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    emitProgress(90);

    if (m_state == State_Canceling)
    {
        processExecuted(EC_Canceled);
        return;
    }

    if (exitCode)
    {
        qLog_DebugMsg() << "Java process exit error code: " << exitCode;
        processExecuted(EC_Error);
        return;
    }

    GridSimConfig::CGridSimOutput output;
    if (!output.loadFromFile(m_outputFilePath))
    {
        qLog_WarningMsg() << "Failed to load output from file.";
        processExecuted(EC_Error);
    }
    m_simulation.setOutput(output);

    processExecuted(EC_Ok);
}

//////////////////////////////////////////////////////////////////////////
#pragma region Tools
QString CSimulator::buildFullSimualtionFilePath(const QString &workingDir, const QString &fileNameFormat,
    quint32 number)
{
    return QDir(workingDir).absoluteFilePath(fileNameFormat.arg(number));
}

void CSimulator::emitProgress(quint8 val)
{
    emit progress(this, val);
}

void CSimulator::processExecuted(CExitCode ec)
{
    disconnect(m_pJavaProcess);
    delete m_pJavaProcess;
    m_pJavaProcess = nullptr;

    m_configFilePath.clear();
    m_outputFilePath.clear();

    m_state = State_Ready;

    emitProgress(100);
    emit executed(this, ec);
}

void CSimulator::processCancel()
{
    m_pJavaProcess->kill();
}
#pragma endregion