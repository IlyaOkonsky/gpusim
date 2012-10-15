#pragma once

#include "Simulation.h"

namespace Core
{
    class CExperiment
    {
    public:
        static const QString c_defaultName;
        static const QString c_defaultDirName;

    public:
        CExperiment(const QString &dirName = c_defaultDirName, const QString &name = c_defaultName,
            const CSimulationsList &simulations = CSimulationsList());

        bool operator == (const CExperiment& other) const;
        bool operator != (const CExperiment& other) const;

        QString getDirName() const;
        void setDirName(const QString &dirName);

        QString getName() const;
        void setName(const QString &name);

        CSimulationsList getSimulations() const;
        const CSimulationsList &getSimulationsRef() const;
        void setSimulations(const CSimulationsList &simulations);

    private:
        QString m_dirName;
        QString m_name;
        CSimulationsList m_simulations;
    };
}

Q_DECLARE_METATYPE(Core::CExperiment);