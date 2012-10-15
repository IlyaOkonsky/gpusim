#include "GridSimOutput.h"
#include "../Serialization/JavaXMLHelper.h"
#include "../Serialization/JavaXMLListSerialize.hpp"

using namespace Core::GridSimConfig;
using namespace Core::Serialization;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const quint32 CGridSimOutput::c_minVersion                 = 1;
const double  CGridSimOutput::c_minTotalSimulationTime     = 0.0f;
const quint32 CGridSimOutput::c_currentOutputVersion       = 1;
const QString CGridSimOutput::c_defaultName                = QString("Unnamed GridSim Output");
const double  CGridSimOutput::c_defaultTotalSimulationTime = c_minTotalSimulationTime;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CGridSimOutput::c_className                   = QString("gpusim.config.GridSimOutput");
const QString CGridSimOutput::c_versionPropName             = QString("version");
const QString CGridSimOutput::c_namePropName                = QString("name");
const QString CGridSimOutput::c_totalSimulationTimePropName = QString("totalSimulationTime");
const QString CGridSimOutput::c_gridletsPropName            = QString("gridlets");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor and equality comparision support
CGridSimOutput::CGridSimOutput(const QString &name /*= c_defaultName*/,
    double totalSimulationTime /*= c_defaultTotalSimulationTime*/,
    const CGridSimGridletsOutput &gridlets /*= CGridSimGridletsOutput()*/)
    :IJavaXMLSerialize(c_className), m_version(c_currentOutputVersion), m_name(c_defaultName),
    m_totalSimulationTime(c_defaultTotalSimulationTime)
{
    setName(name);
    setTotalSimulationTime(totalSimulationTime);
    setGridlets(gridlets);
}

bool CGridSimOutput::operator==(const CGridSimOutput& other) const
{
    return
        (m_name                == other.m_name               ) &&
        (m_version             == other.m_version            ) &&
        (m_totalSimulationTime == other.m_totalSimulationTime) &&
        (m_gridlets            == other.m_gridlets           );
}

bool CGridSimOutput::operator!=(const CGridSimOutput& other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region Version property
quint32 CGridSimOutput::getVersion() const
{
    return m_version;
}

void CGridSimOutput::setVersion(quint32 version)
{
    if (m_version == version)
        return;

    Q_ASSERT(version >= c_minVersion);
    m_version = version;
}
#pragma endregion

#pragma region Name property
QString CGridSimOutput::getName() const
{
    return m_name;
}

void CGridSimOutput::setName(const QString &name)
{
    if (m_name == name)
        return;

    Q_ASSERT(!name.isEmpty());
    m_name = name;
}
#pragma endregion

#pragma region TotalSimulationTime property
double CGridSimOutput::getTotalSimulationTime() const
{
    return m_totalSimulationTime;
}

void CGridSimOutput::setTotalSimulationTime(double totalSimulationTime)
{
    if (m_totalSimulationTime == totalSimulationTime)
        return;

    Q_ASSERT(totalSimulationTime >= c_minTotalSimulationTime);
    m_totalSimulationTime = totalSimulationTime;
}
#pragma endregion

#pragma region Gridlets property
CGridSimGridletsOutput CGridSimOutput::getGridlets() const
{
    return getGridletsRef();
}

const CGridSimGridletsOutput &CGridSimOutput::getGridletsRef() const
{
    return m_gridlets;
}

void CGridSimOutput::setGridlets(const CGridSimGridletsOutput &gridlets)
{
    if (m_gridlets == gridlets)
        return;

    Q_ASSERT(!gridlets.isEmpty());
    m_gridlets = gridlets;
}
#pragma endregion
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Serialization support
bool CGridSimOutput::isValid() const
{
    return (m_version >= c_minVersion) && (m_totalSimulationTime >= c_minTotalSimulationTime) &&
        (!(m_name.isEmpty() || m_gridlets.isEmpty()));
}

void CGridSimOutput::saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const
{
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_versionPropName,             getVersion()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_namePropName,                getName()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_totalSimulationTimePropName, getTotalSimulationTime()));

    elem.appendChild(createListPropertyElement(doc, c_gridletsPropName, getGridletsRef().constBegin(),
        getGridletsRef().constEnd()));
}

void CGridSimOutput::loadDataFromXMLNode(const QDomElement &elem)
{
    quint32 version;
    QString name;
    double totalSimulationTime;
    CGridSimGridletsOutput gridlets;

    CJavaXMLHelper::getPropertyValue(elem, c_versionPropName, version, c_currentOutputVersion);
    CJavaXMLHelper::getPropertyValue(elem, c_namePropName,    name,    c_defaultName);
    CJavaXMLHelper::getPropertyValue(elem, c_totalSimulationTimePropName, totalSimulationTime,
        c_defaultTotalSimulationTime);

    getListPropertyValue(elem, c_gridletsPropName, std::back_inserter(gridlets));

    setVersion(version);
    setName(name);
    setTotalSimulationTime(totalSimulationTime);
    setGridlets(gridlets);
}

IJavaXMLSerializePtr CGridSimOutput::create() const
{
    return IJavaXMLSerializePtr(new CGridSimOutput());
}

void CGridSimOutput::copy(const IJavaXMLSerializePtr &from)
{
    auto ptr = from.staticCast<CGridSimOutput>();
    *this = *ptr;
}
#pragma endregion