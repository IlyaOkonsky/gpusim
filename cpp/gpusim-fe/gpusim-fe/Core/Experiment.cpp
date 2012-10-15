#include "Experiment.h"

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const QString CExperiment::c_defaultName   = QString("Unnamed Experiment");
const QString CExperiment::c_defaultDirName   = QString("Unnamed Experiment");
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor and Equality comparision support
CExperiment::CExperiment(const QString &dirName /*= c_defaultDirName*/, const QString &name /*= c_defaultName*/,
    const CSimulationsList &simulations /*= CSimulationsList()*/)
    : m_dirName(c_defaultDirName), m_name(c_defaultName)
{
    setDirName(dirName);
    setName(name);
    setSimulations(simulations);
}

bool CExperiment::operator==(const CExperiment& other) const
{
    return (m_dirName == other.m_dirName) && (m_name == other.m_name) && (m_simulations == other.m_simulations);
}

bool CExperiment::operator!=(const CExperiment& other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region Name property
QString CExperiment::getDirName() const
{
    return m_dirName;
}

void CExperiment::setDirName(const QString &dirName)
{
    if (m_dirName == dirName)
        return;

    // TODO: Add DirName validation.
    m_dirName = dirName;
}
#pragma endregion

#pragma region Name property
QString CExperiment::getName() const
{
    return m_name;
}

void CExperiment::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
}
#pragma endregion

#pragma region Simulations property
CSimulationsList CExperiment::getSimulations() const
{
    return getSimulationsRef();
}

const CSimulationsList & CExperiment::getSimulationsRef() const
{
    return m_simulations;
}

void CExperiment::setSimulations(const CSimulationsList &simulations)
{
    if (m_simulations == simulations)
        return;

    m_simulations = simulations;
}
#pragma endregion
#pragma endregion