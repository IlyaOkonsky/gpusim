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
        static const double  c_defaultSimulationTime;

    public:
        COriginal(quint32 matrixSize = c_defaultMatrixSize, double simulationTime = c_defaultSimulationTime);

        bool operator == (const COriginal& other) const;
        bool operator != (const COriginal& other) const;
        bool operator <  (const COriginal& other) const;

        quint32 getMatrixSize() const;
        void setMatrixSize(quint32 matrixSize);

        double getSimulationTime() const;
        void setSimulationTime(double simulationTime);

    private:
        quint32 m_matrixSize;
        double m_simulationTime;
    };

    typedef QVector<COriginal> COriginalsList;
}

Q_DECLARE_METATYPE(Core::COriginal);
Q_DECLARE_METATYPE(Core::COriginalsList);