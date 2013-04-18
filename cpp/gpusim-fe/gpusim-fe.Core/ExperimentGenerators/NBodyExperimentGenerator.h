#pragma once

#include "../IExperimentGenerator.h"
#include "NBodyExperimentGeneratorSettings.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CNBodyExperimentGenerator : public IExperimentGenerator
    {
    public:
        CNBodyExperimentGenerator(const Settings::CNBodyExperimentGeneratorSettings &settings,
            quint32 minN, quint32 maxN, quint32 nIncrement,
            quint32 minThreadsPerBlock, quint32 maxThreadsPerBlock, quint32 threadsPerBlockIncrement);

        virtual CExperiment generate();
        CExperiment generateFromOriginals(const COriginalsList& originals, bool nIsAsisValue);

    private:
        CSimulation createSim(quint32 simNumber, quint32 n, quint32 threadsPerBlock, double xAsisValue);
        GridSimConfig::CGridSimConfig createConfig(const QString &name, quint32 n, quint32 threadsPerBlock);
        GridSimConfig::CGridSimResourcesConfig createResources(quint32 threadsPerBlock);
        GridSimConfig::CGridSimGridletConfig createGridlet(quint32 n, quint32 threadsPerBlock);

    private:
        Settings::CNBodyExperimentGeneratorSettings m_settings;

        quint32 m_minN;
        quint32 m_maxN;
        quint32 m_nIncrement;

        quint32 m_minThreadsPerBlock;
        quint32 m_maxThreadsPerBlock;
        quint32 m_threadsPerBlockIncrement;

    private:
        static const QString c_genName;
        static const QString c_expDirNameFormat;
        static const QString c_expNameFormat;
        static const QString c_simNameFormat;
        static const QString c_configPostfix;
    };
}