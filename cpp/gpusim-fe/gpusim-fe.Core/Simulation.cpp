#include "Simulation.h"

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const quint32 CSimulation::c_defaultNumber     = 1;
const QString CSimulation::c_defaultName       = QString("Unnamed Simulation");
const double  CSimulation::c_defaultXAsisValue = 1.0f;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor and comparision support
CSimulation::CSimulation(quint32 number /*= c_defaultNumber*/, const QString &name /*= c_defaultName*/,
    double xAsisValue /*= c_defaultXAsisValue*/,
    const GridSimConfig::CGridSimConfig &config /*= GridSimConfig::CGridSimConfig()*/)
    : m_number(c_defaultNumber), m_name(c_defaultName), m_xAsisValue(c_defaultXAsisValue)
{
    setNumber(number);
    setName(name);
    setXAsisValue(xAsisValue);
    setConfig(config);
}

bool CSimulation::operator==(const CSimulation& other) const
{
    return
        (m_number     == other.m_number    ) &&
        (m_name       == other.m_name      ) &&
        (m_xAsisValue == other.m_xAsisValue) &&
        (m_config     == other.m_config    ) &&
        (m_output     == other.m_output    );
}

bool CSimulation::operator!=(const CSimulation& other) const
{
    return !(*this == other);
}

bool CSimulation::operator<(const CSimulation& right) const
{
    return (m_xAsisValue != right.m_xAsisValue) ? (m_xAsisValue < right.m_xAsisValue) : (m_number < right.m_number);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region Number property
quint32 CSimulation::getNumber() const
{
    return m_number;
}

void CSimulation::setNumber(quint32 number)
{
    if (m_number == number)
        return;

    m_number = number;
}
#pragma endregion

#pragma region Name property
QString CSimulation::getName() const
{
    return m_name;
}

void CSimulation::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
}
#pragma endregion

#pragma region XAsisValue property
double CSimulation::getXAsisValue() const
{
    return m_xAsisValue;
}

void CSimulation::setXAsisValue(double xAsisValue)
{
    if (m_xAsisValue == xAsisValue)
        return;

    m_xAsisValue = xAsisValue;
}

#pragma endregion
#pragma region Config property
GridSimConfig::CGridSimConfig CSimulation::getConfig() const
{
    return getConfigRef();
}

const GridSimConfig::CGridSimConfig & CSimulation::getConfigRef() const
{
    return m_config;
}

void CSimulation::setConfig(const GridSimConfig::CGridSimConfig &config)
{
    if (m_config == config)
        return;

    m_config = config;
}
#pragma endregion

#pragma region Output property
GridSimConfig::CGridSimOutput CSimulation::getOutput() const
{
    return getOutputRef();
}

const GridSimConfig::CGridSimOutput & CSimulation::getOutputRef() const
{
    return m_output;
}

void CSimulation::setOutput(const GridSimConfig::CGridSimOutput &output)
{
    if (m_output == output)
        return;

    m_output = output;
}
#pragma endregion
#pragma endregion