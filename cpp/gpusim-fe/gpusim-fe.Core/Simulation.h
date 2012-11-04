#pragma once

#include "GridSimConfig/GridSimConfig.h"
#include "GridSimConfig/GridSimOutput.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CSimulation
    {
    public:
        static const quint32 c_defaultNumber;
        static const QString c_defaultName;

    public:
        CSimulation(quint32 number = c_defaultNumber, const QString &name = c_defaultName,
            const GridSimConfig::CGridSimConfig &config = GridSimConfig::CGridSimConfig());

        bool operator == (const CSimulation& other) const;
        bool operator != (const CSimulation& other) const;

        // Compares by Number property value (returns m_number < right.m_number)
        bool operator <  (const CSimulation& right) const;


        quint32 getNumber() const;
        void setNumber(quint32 number);

        QString getName() const;
        void setName(const QString &name);

        GridSimConfig::CGridSimConfig getConfig() const;
        const GridSimConfig::CGridSimConfig &getConfigRef() const;
        void setConfig(const GridSimConfig::CGridSimConfig &config);

        GridSimConfig::CGridSimOutput getOutput() const;
        const GridSimConfig::CGridSimOutput &getOutputRef() const;
        void setOutput(const GridSimConfig::CGridSimOutput &output);

    private:
        quint32 m_number;
        QString m_name;
        GridSimConfig::CGridSimConfig m_config;
        GridSimConfig::CGridSimOutput m_output;
    };

    typedef QVector<CSimulation> CSimulationsList;
}

Q_DECLARE_METATYPE(Core::CSimulation);
Q_DECLARE_METATYPE(Core::CSimulationsList);