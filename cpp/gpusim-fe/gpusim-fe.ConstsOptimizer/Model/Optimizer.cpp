#include "Optimizer.h"

#include "Originals/OriginalsReader.h"
#include "MMEGSettingsGenerator/MMEGSettigsGenerator.h"
#include "../../gpusim-fe.Core/ExperimentGenerators/MatrixMultiplyExperimentGenerator.h"

#include "../../QLogger/QLog"

#include <QCoreApplication>
#include <QMath.h>

using namespace Model;
using Core::CMatrixMultiplyExperimentGenerator;
using Core::CExperiment;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString COptimizer::c_simulatorJarPath      = QString("../Simulator/gpusim-runtime.jar");
const QString COptimizer::c_experimentsWorkingDir = QString("../Experiments-co");
const quint32 COptimizer::c_mmegBlockSize         = 16;
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

COptimizer::COptimizer(const QString &originalsFilePath, const QString &configFilePath, const QString &outputFilePath,
    QObject *pParent /*= nullptr*/)
    :QObject(pParent), m_originalsFilePath(originalsFilePath), m_configFilePath(configFilePath),
    m_outputFilePath(outputFilePath), m_settingsSetTotalSize(0), m_bestDifference(0.0f),
    m_experimenter(c_simulatorJarPath, c_experimentsWorkingDir, CExperiment(), this)
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

void COptimizer::optimize()
{
    if (!readOriginals())
    {
        qLog_CriticalMsg() << "Failed to read originals.";
        finishOptimization(true);
        return;
    }

    m_mmegMinMatrixSize = m_originals.front().getMatrixSize();
    m_mmegMaxMatrixSize = m_originals.back().getMatrixSize();
    m_mmegMatrixSizeIncrement = (m_originals.back().getMatrixSize() - m_originals.front().getMatrixSize()) /
        (m_originals.size() - 1);

    if (!readConfig())
    {
        qLog_CriticalMsg() << "Failed to read config.";
        finishOptimization(true);
        return;
    }

    generateSettingsSet();
    qLog_DebugMsg() << "Starting experiments execution...";
    processNextSettings();
}

//////////////////////////////////////////////////////////////////////////

void COptimizer::onExperimeterProgress(Core::CExperimenter *pExperimenter, quint8 val)
{
    // NOTE: Currently nothing are here. But in future some code will be added.
}

void COptimizer::onExperimeterExecuted(Core::CExperimenter *pExperimenter, Core::CExitCode ec)
{
    qLog_DebugMsg() << "Experiment #" << m_settingsSetTotalSize - m_settingsSet.size() << 
        " of " << m_settingsSetTotalSize << " executed with exit code " << exitCodeToString(ec);

    auto progress = ((m_settingsSetTotalSize - m_settingsSet.size()) * 100) / m_settingsSetTotalSize;
    qLog_DebugMsg() << "Overal progress: " << progress << "%";

    if (ec == Core::EC_Error)
    {
        qLog_CriticalMsg() << "Last experiment executed with error exit code.";
        finishOptimization(true);
        return;
    }

    double diff = calculateDiff(pExperimenter->getExperimentRef());
    if (diff == -1.0f)
    {
        qLog_CriticalMsg() << "Failed to calculate difference between originals and simulation.";
        finishOptimization(true);
        return;
    }

    if (checkDiff(diff))
        saveBestSettings();

    processNextSettings();
}

//////////////////////////////////////////////////////////////////////////

bool COptimizer::readOriginals()
{
    qLog_DebugMsg() << "Reading originals from " << m_originalsFilePath << " file...";
    COriginalsReader reader(m_originals);
    bool res = reader.readOriginals(m_originalsFilePath);
    qLog_DebugMsg() << "..." << res;
    return res;
}

bool COptimizer::readConfig()
{
    qLog_DebugMsg() << "Reading configuration from " << m_configFilePath << " file...";
    bool res = COptimizerConfig::readFromFile(m_configFilePath, m_config);
    qLog_DebugMsg() << "..." << res;
    return res;
}

void COptimizer::generateSettingsSet()
{
    qLog_DebugMsg() << "Generating settings set... ";

    CMMEGSettingsList settingsSet = CMMEGSettingsGenerator::generate(m_config);
    m_settingsSetTotalSize = settingsSet.size();
    std::copy(settingsSet.constBegin(), settingsSet.constEnd(), std::back_inserter(m_settingsSet));
    
    qLog_DebugMsg() << "..." << m_settingsSetTotalSize << " settings generated.";
}

void COptimizer::processNextSettings()
{
    if (m_settingsSet.isEmpty())
    {
        qLog_DebugMsg() << "No more settings left. Optmization finished.";
        finishOptimization(false);
        return;
    }

    m_currentSettings = m_settingsSet.dequeue();
    qLog_DebugMsg() << "Executing experiment #" << m_settingsSetTotalSize - m_settingsSet.size() << 
        " of " << m_settingsSetTotalSize << "...";

    CMatrixMultiplyExperimentGenerator gen(m_currentSettings, m_mmegMinMatrixSize, m_mmegMaxMatrixSize,
        m_mmegMatrixSizeIncrement, c_mmegBlockSize);
    m_experimenter.execute(gen.generate());
}

void COptimizer::finishOptimization(bool error)
{
    if (error)
    {
        qLog_CriticalMsg() << "Optimization finihed with error.";
        qApp->quit();
        return;
    }

    qLog_DebugMsg() << "Optimization finihed with success.";
    qLog_DebugMsg() << "Best difference value "<< m_bestDifference << " was achived. Best settings values:";
    writeSettingsToLog(m_bestSettings);
    qApp->quit();
}

double COptimizer::calculateDiff(const Core::CExperiment &e)
{
    if (e.getSimulationsRef().size() != m_originals.size())
    {
        qLog_CriticalMsg() << "Originals and simulations count in experiment are no equal.";
        return -1.0f;
    }

    double diff = 0.0f;
    auto itOriginals = m_originals.constBegin();
    auto itOriginalsEnd = m_originals.constEnd();
    auto itSimulation = e.getSimulationsRef().constBegin();
    for(; itOriginals != itOriginalsEnd; ++itOriginals, ++itSimulation)
    {
        double distance = qAbs(itOriginals->getSimulationTime() -
            itSimulation->getOutputRef().getTotalSimulationTime());
        diff += qPow(distance, 2.0f);
    }

    return diff;
}

bool COptimizer::checkDiff(double currentDifference)
{
    qLog_DebugMsg() << "Difference value: " << currentDifference;
    if ((m_bestDifference != 0.0f) && (currentDifference >= m_bestDifference))
        return false;

    m_bestDifference = currentDifference;
    m_bestSettings = m_currentSettings;

    qLog_DebugMsg() << "New best difference value " << m_bestDifference << " found.";
    writeSettingsToLog(m_bestSettings);

    return true;
}

void COptimizer::saveBestSettings()
{
    qLog_DebugMsg() << "Saving new best settings to " << m_outputFilePath << " file...";
    bool res = m_bestSettings.saveToFile(m_outputFilePath);
    qLog_DebugMsg() << "..." << res;

    if (!res)
        qLog_WarningMsg() << "Failed to save new best settings to file " << m_outputFilePath;
}

//////////////////////////////////////////////////////////////////////////

void COptimizer::writeSettingsToLog(const CMatrixMultiplyExperimentGeneratorSettings &s)
{
    qLog_DebugMsg() << "MMEGSettings values: ";
    qLog_DebugMsg() << "..cpuMachinePECount:  " << s.getCPUMachinePECount();
    qLog_DebugMsg() << "..cpuMachinePERating: " << s.getCPUMachinePERating();
    qLog_DebugMsg() << "..gpuMachinePECount:  " << s.getGPUMachinePECount();
    qLog_DebugMsg() << "..gpuMachinePERating: " << s.getGPUMachinePERating();
    qLog_DebugMsg() << "..resourceBaudRate:   " << s.getResourceBaudRate();
    qLog_DebugMsg() << "..resourceCostPerSec: " << s.getResourceCostPerSec();
    qLog_DebugMsg() << "..linkBaudRate:       " << s.getLinkBaudRate();
    qLog_DebugMsg() << "..loadOperationCost:  " << s.getLoadOperationCost();
    qLog_DebugMsg() << "..saveOperationCost:  " << s.getSaveOperationCost();
}