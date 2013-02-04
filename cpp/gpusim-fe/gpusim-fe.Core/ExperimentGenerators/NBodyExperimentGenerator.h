#pragma once

#include "../IExperimentGenerator.h"
#include "NBodyExperimentGeneratorSettings.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CNBodyExperimentGenerator : public IExperimentGenerator
    {
    public:
        CNBodyExperimentGenerator(const Settings::CNBodyExperimentGeneratorSettings &settings,
            quint64 minN, quint64 maxN, quint64 nIncrement,
            quint64 minThreadsPerBlock, quint64 maxThreadsPerBlock, quint64 threadsPerBlockIncrement);

        virtual CExperiment generate();

    private:
        CSimulation createSim(quint32 simNumber, quint64 n, quint64 threadsPerBlock);
        GridSimConfig::CGridSimConfig createConfig(const QString &name, quint64 n, quint64 threadsPerBlock);
        GridSimConfig::CGridSimGridletConfig createGridlet(quint64 n);

    private:
        Settings::CNBodyExperimentGeneratorSettings m_settings;

        quint64 m_minN;
        quint64 m_maxN;
        quint64 m_nIncrement;

        quint64 m_minThreadsPerBlock;
        quint64 m_maxThreadsPerBlock;
        quint64 m_threadsPerBlockIncrement;

    private:
        static const QString c_genName;
        static const QString c_expDirNameFormat;
        static const QString c_expNameFormat;
        static const QString c_simNameFormat;
        static const QString c_configPostfix;
    };
}