#include "Original.h"

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const double  COriginal::c_minSimulationTime     = 0.0f;
const quint32 COriginal::c_defaultMatrixSize     = 0;
const double  COriginal::c_defaultSimulationTime = c_minSimulationTime;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor and Comparision support
COriginal::COriginal(quint32 matrixSize /*= c_defaultMatrixSize*/,
    double simulationTime /*= c_defaultSimulationTimef*/)
    : m_matrixSize(c_defaultMatrixSize), m_simulationTime(c_defaultSimulationTime)
{
    setMatrixSize(matrixSize);
    setSimulationTime(simulationTime);
}

bool COriginal::operator==(const COriginal& other) const
{
    return (m_matrixSize == other.m_matrixSize) && (m_simulationTime == other.m_simulationTime);
}

bool COriginal::operator!=(const COriginal& other) const
{
    return !(*this == other);
}

bool COriginal::operator<(const COriginal& other) const
{
    return (m_matrixSize < other.m_matrixSize);
}
#pragma endregion

#pragma region Properties
#pragma region MatrixSize property
quint32 COriginal::getMatrixSize() const
{
    return m_matrixSize;
}

void COriginal::setMatrixSize(quint32 matrixSize)
{
    if (m_matrixSize == matrixSize)
        return;

    m_matrixSize = matrixSize;
}
#pragma endregion

#pragma region SimulationTime property
double COriginal::getSimulationTime() const
{
    return m_simulationTime;
}

void COriginal::setSimulationTime(double simulationTime)
{
    if (m_simulationTime == simulationTime)
        return;

    Q_ASSERT(simulationTime >= c_minSimulationTime);
    m_simulationTime = simulationTime;
}
#pragma endregion
#pragma endregion