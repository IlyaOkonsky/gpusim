#pragma once

#include "../IExperimentGenerator.h"

namespace Core
{
    class CMatrixMultiplyExperimentGenerator : public IExperimentGenerator
    {
    public:
        CMatrixMultiplyExperimentGenerator(quint64 matrixSize, quint64 minBlockSize, quint64 maxBlockSize,
            quint64 blockSizeIncrement = 1);

        virtual CExperiment generate();

    private:
        static CSimulation createSim(quint32 simNumber, quint64 matrixSize, quint64 blockSize);
        static GridSimConfig::CGridSimConfig createConfig(const QString &name, quint64 matrixSize, quint64 blockSize);
        static GridSimConfig::CGridSimGridletConfig createGridlet(quint32 id, quint64 matrixSize, quint64 blockSize);

    private:
        quint64 m_matrixSize;
        quint64 m_minBlockSize;
        quint64 m_maxBlockSize;
        quint64 m_blockSizeIncrement;

    private:
        static const QString c_genName;
        static const QString c_expDirNameFormat;
        static const QString c_expNameFormat;
        static const QString c_simNameFormat;
        static const QString c_configPostfix;

        static const quint32 c_cpuMachinePECount;
        static const quint32 c_cpuMachinePERating;
        static const quint32 c_gpuMachinePECount;
        static const quint32 c_gpuMachinePERating;

        static const QString c_resourceArch;
        static const QString c_resourceOS;
        static const double  c_resourceBaudRate;
        static const double  c_resourceCostPerSec;

        static const double  c_loadOperationCost;
        static const double  c_saveOperationCost;
    };
}