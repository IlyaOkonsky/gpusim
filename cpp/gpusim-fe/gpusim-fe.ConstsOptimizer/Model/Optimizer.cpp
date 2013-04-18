#include "Optimizer.h"

#include "../../gpusim-fe.Core/Originals/OriginalsReader.h"
#include "../../gpusim-fe.Core/Originals/OriginalsHelpers.h"

#include "NBEGSettingsGenerator/NBEGSettigsGenerator.h"
#include "../../gpusim-fe.Core/ExperimentGenerators/NBodyExperimentGenerator.h"

#include "../../QLogger/QLog"

#include <QCoreApplication>

using namespace Model;
using Core::CNBodyExperimentGenerator;
using Core::CExperiment;
using Core::COriginal;
using Core::COriginalsReader;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString COptimizer::c_simulatorJarPath       = QString("../Simulator/gpusim-runtime.jar");
const QString COptimizer::c_experimentsWorkingDir  = QString("../Experiments-co");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

COptimizer::COptimizer(const QString &configFilePath, const QString &originalsFilePath, const QString &outputFilePath,
    QObject *pParent /*= nullptr*/)
    :QObject(pParent), m_configFilePath(configFilePath), m_originalsFilePath(originalsFilePath),
    m_outputFilePath(outputFilePath), m_settingsSetTotalSize(0), m_bestDifference(0.0f), m_bestAverangeError(0.0f),
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
    if (!readConfig())
    {
        qLog_CriticalMsg() << "Failed to read config.";
        finishOptimization(true);
        return;
    }

    if (!readOriginals())
    {
        qLog_CriticalMsg() << "Failed to read originals.";
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

    double averangeError = 0.0f;
    double diff = Core::calculateDifference(m_originals, pExperimenter->getExperimentRef(),
        &averangeError);
    if (diff == -1.0f)
    {
        qLog_CriticalMsg() << "Failed to calculate difference between originals and simulation.";
        finishOptimization(true);
        return;
    }

    if (checkDiff(diff, averangeError))
        saveBestSettings();

    processNextSettings();
}

//////////////////////////////////////////////////////////////////////////

bool COptimizer::readOriginals()
{
    qLog_DebugMsg() << "Reading originals from " << m_originalsFilePath << " file...";
    COriginalsReader reader(m_originals, m_config.m_originalsMinN, m_config.m_originalsMaxN,
        m_config.m_originalsThreadsPerBlock);
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

    CNBEGSettingsList settingsSet = CNBEGSettingsGenerator::generate(m_config);
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

    writeSettingsToLog(m_currentSettings);

    CNBodyExperimentGenerator gen(m_currentSettings, 1024, 1024, 0, 1024, 1024, 0);
    m_experimenter.execute(gen.generateFromOriginals(m_originals, false));
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
    qLog_DebugMsg() << "Best difference value " << m_bestDifference << " with avernage error " << m_bestAverangeError <<
        "% was achived. Best settings values:";
    writeSettingsToLog(m_bestSettings);
    qApp->quit();
}

bool COptimizer::checkDiff(double currentDifference, double currentAverangeError)
{
    qLog_DebugMsg() << "Difference value: " << currentDifference << "; Averange error: " << currentAverangeError << "%.";
    
    double currentCompareValue = (m_config.m_cm == COptimizerConfig::CM_AbsoluteDifference)
        ? currentDifference
        : currentAverangeError;
    double currentBestValue = (m_config.m_cm == COptimizerConfig::CM_AbsoluteDifference)
        ? m_bestDifference
        : m_bestAverangeError;

    if ((currentBestValue != 0.0f) && (currentCompareValue >= currentBestValue))
        return false;

    m_bestDifference = currentDifference;
    m_bestAverangeError = currentAverangeError;
    m_bestSettings = m_currentSettings;

    qLog_DebugMsg() << "New best difference value " << m_bestDifference << " with averange error " <<
        m_bestAverangeError << "% found.";
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

void COptimizer::writeSettingsToLog(const CNBodyExperimentGeneratorSettings &s)
{
    qLog_DebugMsg() << "NBEGSettings values: ";
    qLog_DebugMsg() << "..gpuCoreRating:                   " << s.getGPUCoreRating();
    qLog_DebugMsg() << "..resourceBaudRate:                " << s.getResourceBaudRate();
    qLog_DebugMsg() << "..resourceCostPerSec:              " << s.getResourceCostPerSec();
    qLog_DebugMsg() << "..linkBaudRate:                    " << s.getLinkBaudRate();
    qLog_DebugMsg() << "..limitationsDivider:              " << s.getLimitationsDivider();
    qLog_DebugMsg() << "..smallTPBPenaltyWeight:           " << s.getSmallTPBPenaltyWeight();
    qLog_DebugMsg() << "..largeTPBPenaltyWeight:           " << s.getLargeTPBPenaltyWeight();
    qLog_DebugMsg() << "..multiplicativeLengthScaleFactor: " << s.getMultiplicativeLengthScaleFactor();
    qLog_DebugMsg() << "..additiveLengthScaleFactor:       " << s.getAdditiveLengthScaleFactor();
}