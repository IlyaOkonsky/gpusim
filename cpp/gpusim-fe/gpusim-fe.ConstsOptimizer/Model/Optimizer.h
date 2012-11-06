#pragma once

#include "Originals/Original.h"
#include "Config/OptimizerConfig.h"
#include "Config/MMEGSettingsHelpers.h"
#include "../../gpusim-fe.Core/Experimenter.h"

#include <QObject>

namespace Model
{
    class COptimizer : public QObject
    {
        Q_OBJECT;

    public:
        COptimizer(const QString &originalsFilePath, const QString &configFilePath,
            const QString &outputFilePath, QObject *pParent = nullptr);

    public slots:
        void optimize();

    private slots:
        void onExperimeterProgress(Core::CExperimenter *pExperimenter, quint8 val);
        void onExperimeterExecuted(Core::CExperimenter *pExperimenter, Core::CExitCode ec);

    private:
        void processOptimize();

        bool readOriginals();
        bool readConfig();
        void generateSettingsSet();

        void processNextSettings();
        void finishOptimization(bool error);

        // Returns difference between originals and experiment result.
        // In case of error returns -1.0f;
        // 
        double calculateDiff(const Core::CExperiment &e);

        bool checkDiff(double currentDifference);
        void saveBestSettings();

        static void writeSettingsToLog(const CMatrixMultiplyExperimentGeneratorSettings &s);

    private:
        QString m_originalsFilePath;
        QString m_configFilePath;
        QString m_outputFilePath;

        COriginalsList m_originals;
        COptimizerConfig m_config;

        quint32 m_settingsSetTotalSize;
        CMMEGSettingsQueue m_settingsSet;

        CMatrixMultiplyExperimentGeneratorSettings m_currentSettings;
        CMatrixMultiplyExperimentGeneratorSettings m_bestSettings;
        double m_bestDifference;

        quint32 m_mmegMinMatrixSize;
        quint32 m_mmegMaxMatrixSize;
        quint32 m_mmegMatrixSizeIncrement;
        Core::CExperimenter m_experimenter;

    private:
        static const QString c_simulatorJarPath;
        static const QString c_experimentsWorkingDir;
        static const quint32 c_mmegBlockSize;
    };
}