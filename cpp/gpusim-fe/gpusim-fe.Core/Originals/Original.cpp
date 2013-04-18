#include "Original.h"

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const double  COriginal::c_minSimulationTime      = 0.0f;
const quint32 COriginal::c_defaultN               = 0;
const quint32 COriginal::c_defaultThreadsPerBlock = 0;
const double  COriginal::c_defaultSimulationTime  = c_minSimulationTime;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor and Comparision support
COriginal::COriginal(quint32 n /*= c_defaultN*/, quint32 threadsPerBlock /*= c_defaultThreadsPerBlock*/,
    double simulationTime /*= c_defaultSimulationTime*/)
    : m_n(c_defaultN), m_threadsPerBlock(c_defaultThreadsPerBlock), m_simulationTime(c_defaultSimulationTime)
{
    setN(n);
    setThreadsPerBlock(threadsPerBlock);
    setSimulationTime(simulationTime);
}

bool COriginal::operator==(const COriginal& other) const
{
    return
        (m_n               == other.m_n              ) &&
        (m_threadsPerBlock == other.m_threadsPerBlock) &&
        (m_simulationTime  == other.m_simulationTime );
}

bool COriginal::operator!=(const COriginal& other) const
{
    return !(*this == other);
}

bool COriginal::operator<(const COriginal& other) const
{
    return (m_n != other.m_n)? (m_n < other.m_n) : (m_threadsPerBlock < other.m_threadsPerBlock);
}
#pragma endregion

#pragma region Properties
#pragma region N property
quint32 COriginal::getN() const
{
    return m_n;
}

void COriginal::setN(quint32 n)
{
    if (m_n == n)
        return;

    m_n = n;
}
#pragma endregion

#pragma region ThreadsPerBlock property
quint32 COriginal::getThreadsPerBlock() const
{
    return m_threadsPerBlock;
}

void COriginal::setThreadsPerBlock(quint32 threadsPerBlock)
{
    if (m_threadsPerBlock == threadsPerBlock)
        return;

    m_threadsPerBlock = threadsPerBlock;
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