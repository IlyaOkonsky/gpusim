#pragma once

#include "../IExperimentGenerator.h"
#include "MatrixMultiplyExperimentGeneratorSettings.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CMatrixMultiplyExperimentGenerator : public IExperimentGenerator
    {
    public:
        CMatrixMultiplyExperimentGenerator(const Settings::CMatrixMultiplyExperimentGeneratorSettings &settings,
            quint64 minMatrixSize, quint64 maxMatrixSize, quint64 matrixSizeIncrement,
            quint64 blockSize);

        virtual CExperiment generate();

    private:
        // Shorts large matrix size with add k postfix
        static QString getMatrixSizeString(quint64 matrixSize);

        CSimulation createSim(quint32 simNumber, quint64 matrixSize, quint64 blockSize);
        GridSimConfig::CGridSimConfig createConfig(const QString &name, quint64 matrixSize, quint64 blockSize);
        GridSimConfig::CGridSimGridletConfig createGridlet(quint32 id, quint64 matrixSize, quint64 blockSize,
            quint64 count = 1);

    private:
        Settings::CMatrixMultiplyExperimentGeneratorSettings m_settings;

        quint64 m_minMatrixSize;
        quint64 m_maxMatrixSize;
        quint64 m_matrixSizeIncrement;
        quint64 m_blockSize;

    private:
        static const QString c_genName;
        static const QString c_expDirNameFormat;
        static const QString c_expNameFormat;
        static const QString c_simNameFormat;
        static const QString c_configPostfix;
    };
}