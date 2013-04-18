#pragma once

#include "../Core_global.h"

#include <QVector>
#include <QMetaType>

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT COriginal
    {
    public:
        static const double  c_minSimulationTime;

        static const quint32 c_defaultMatrixSize;
        static const quint32 c_defaultN;
        static const quint32 c_defaultThreadsPerBlock;
        static const double  c_defaultSimulationTime;

    public:
        COriginal(quint32 n = c_defaultN, quint32 threadsPerBlock = c_defaultThreadsPerBlock,
            double simulationTime = c_defaultSimulationTime);

        bool operator == (const COriginal& other) const;
        bool operator != (const COriginal& other) const;
        bool operator <  (const COriginal& other) const;

        quint32 getN() const;
        void setN(quint32 n);

        quint32 getThreadsPerBlock() const;
        void setThreadsPerBlock(quint32 threadsPerBlock);

        double getSimulationTime() const;
        void setSimulationTime(double simulationTime);

    private:
        quint32 m_n;
        quint32 m_threadsPerBlock;
        double m_simulationTime;
    };

    typedef QVector<COriginal> COriginalsList;
}

Q_DECLARE_METATYPE(Core::COriginal);
Q_DECLARE_METATYPE(Core::COriginalsList);