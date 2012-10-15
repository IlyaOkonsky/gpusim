#include "GridSimResourceConfig.h"
#include "../Serialization/JavaXMLHelper.h"
#include "../Serialization/JavaXMLListSerialize.hpp"

#include <QRegExp>

using namespace Core::GridSimConfig;
using namespace Core::Serialization;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const double  CGridSimResourceConfig::c_minCostPerSec  = 0.0f;
const double  CGridSimResourceConfig::c_minTimeZone    = -12.0f;
const double  CGridSimResourceConfig::c_maxTimeZone    = 13.0f;
const quint32 CGridSimResourceConfig::c_minAllocPolicy = 0;
const quint32 CGridSimResourceConfig::c_maxAllocPolicy = 1;
const double  CGridSimResourceConfig::c_minBaudRate    = 0.0f;

const QString CGridSimResourceConfig::c_defaultName        = QString("Resource_0");
const QString CGridSimResourceConfig::c_defaultArch        = QString("Unnamed Architecture");
const QString CGridSimResourceConfig::c_defaultOS          = QString("Unnamed OS");
const double  CGridSimResourceConfig::c_defaultCostPerSec  = c_minCostPerSec;
const double  CGridSimResourceConfig::c_defaultTimeZone    = 0.0f;
const quint32 CGridSimResourceConfig::c_defaultAllocPolicy = c_minAllocPolicy;
const double  CGridSimResourceConfig::c_defaultBaudRate    = 1.0f;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CGridSimResourceConfig::c_nameRegExp = QString("Resource_\\d+");

const QString CGridSimResourceConfig::c_className           = QString("gpusim.config.GridSimResourceConfig");
const QString CGridSimResourceConfig::c_namePropName        = QString("name");
const QString CGridSimResourceConfig::c_archPropName        = QString("arch");
const QString CGridSimResourceConfig::c_osPropName          = QString("OS");
const QString CGridSimResourceConfig::c_costPerSecPropName  = QString("costPerSec");
const QString CGridSimResourceConfig::c_timeZonePropName    = QString("timeZone");
const QString CGridSimResourceConfig::c_allocPolicyPropName = QString("allocPolicy");
const QString CGridSimResourceConfig::c_baudRatePropName    = QString("baudRate");
const QString CGridSimResourceConfig::c_machinesPropName    = QString("machines");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
//////////////////////////////////////////////////////////////////////////

#pragma region Constructor and equality comparision support
CGridSimResourceConfig::CGridSimResourceConfig(
    const QString &name /*= c_defaultName*/,
    const QString &arch /*= c_defaultArch*/,
    const QString &os   /*= c_defaultOS*/,
    double costPerSec   /*= c_defaultCostPerSec*/,
    double timeZone     /*= c_defaultTimeZone*/,
    qint32 allocPolicy  /*= c_defaultAllocPolicy*/,
    double baudRate     /*= c_defaultBaudRate*/,
    const CGridSimMachinesConfig &machines /*= CGridSimMachinesConfig()*/)
    :IJavaXMLSerialize(c_className), m_name(c_defaultName), m_arch(c_defaultArch), m_os(c_defaultOS),
    m_costPerSec(c_defaultCostPerSec), m_timeZone(c_defaultTimeZone), m_allocPolicy(c_defaultAllocPolicy),
    m_baudRate(c_defaultBaudRate)
{
    setName(name);
    setArch(arch);
    setOS(os);
    setCostPerSec(costPerSec);
    setTimeZone(timeZone);
    setAllocPolicy(allocPolicy);
    setBaudRate(baudRate);
    setMachines(machines);
}

bool CGridSimResourceConfig::operator==(const CGridSimResourceConfig& other) const
{
    return
        (m_name        == other.m_name       ) &&
        (m_arch        == other.m_arch       ) &&
        (m_os          == other.m_os         ) &&
        (m_costPerSec  == other.m_costPerSec ) &&
        (m_timeZone    == other.m_timeZone   ) &&
        (m_allocPolicy == other.m_allocPolicy) &&
        (m_baudRate    == other.m_baudRate   ) &&
        (m_machines    == other.m_machines   );
}

bool CGridSimResourceConfig::operator!=(const CGridSimResourceConfig& other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region Name property
QString CGridSimResourceConfig::getName() const
{
    return m_name;
}

void CGridSimResourceConfig::setName(const QString &name)
{
    if (m_name == name)
        return;

    Q_ASSERT(checkName(name));
    m_name = name;
}
#pragma endregion

#pragma region Arch property
QString CGridSimResourceConfig::getArch() const
{
    return m_arch;
}

void CGridSimResourceConfig::setArch(const QString &arch)
{
    if (m_arch == arch)
        return;

    Q_ASSERT(!arch.isEmpty());
    m_arch = arch;
}
#pragma endregion

#pragma region OS property
QString CGridSimResourceConfig::getOS() const
{
    return m_os;
}

void CGridSimResourceConfig::setOS(const QString &os)
{
    if (m_os == os)
        return;

    Q_ASSERT(!os.isEmpty());
    m_os = os;
}
#pragma endregion

#pragma region CostPerSec property
double CGridSimResourceConfig::getCostPerSec() const
{
    return m_costPerSec;
}

void CGridSimResourceConfig::setCostPerSec(double costPerSec)
{
    if (m_costPerSec == costPerSec)
        return;

    Q_ASSERT(costPerSec >= c_minCostPerSec);
    m_costPerSec = costPerSec;
}
#pragma endregion

#pragma region TimeZone property
double CGridSimResourceConfig::getTimeZone() const
{
    return m_timeZone;
}

void CGridSimResourceConfig::setTimeZone(double timeZone)
{
    if (m_timeZone == timeZone)
        return;

    Q_ASSERT((timeZone >= c_minTimeZone) && (timeZone <= c_maxTimeZone));
    m_timeZone = timeZone;
}
#pragma endregion

#pragma region AllocPolicy property
qint32 CGridSimResourceConfig::getAllocPolicy() const
{
    return m_allocPolicy;
}

void CGridSimResourceConfig::setAllocPolicy(qint32 allocPolicy)
{
    if (m_allocPolicy == allocPolicy)
        return;

    Q_ASSERT((allocPolicy >= c_minAllocPolicy) && (allocPolicy <= c_maxAllocPolicy));
    m_allocPolicy = allocPolicy;
}
#pragma endregion

#pragma region BaudRate property
double CGridSimResourceConfig::getBaudRate() const
{
    return m_baudRate;
}

void CGridSimResourceConfig::setBaudRate(double baudRate)
{
    if (m_baudRate == baudRate)
        return;

    Q_ASSERT(baudRate > c_minBaudRate);
    m_baudRate = baudRate;
}
#pragma endregion

#pragma region Machines property
CGridSimMachinesConfig CGridSimResourceConfig::getMachines() const
{
    return getMachinesRef();
}

const CGridSimMachinesConfig & CGridSimResourceConfig::getMachinesRef() const
{
    return m_machines;
}

void CGridSimResourceConfig::setMachines(const CGridSimMachinesConfig &machines)
{
    if (m_machines == machines)
        return;

    Q_ASSERT(!machines.isEmpty());
    m_machines = machines;
}
#pragma endregion
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Serialization support
bool CGridSimResourceConfig::isValid() const
{
    if (!checkName(m_name))
        return false;

    if (m_arch.isEmpty() || m_os.isEmpty())
        return false;

    if (m_costPerSec < c_minCostPerSec)
        return false;

    if ((m_timeZone < c_minTimeZone) || (m_timeZone > c_maxTimeZone))
        return false;

    if ((m_allocPolicy < c_minAllocPolicy) || (m_allocPolicy > c_maxAllocPolicy))
        return false;

    if (m_baudRate <= c_minBaudRate)
        return false;

    return !m_machines.isEmpty();
}

void CGridSimResourceConfig::saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const
{
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_namePropName,        getName()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_archPropName,        getArch()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_osPropName,          getOS()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_costPerSecPropName,  getCostPerSec()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_timeZonePropName,    getTimeZone()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_allocPolicyPropName, getAllocPolicy()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_baudRatePropName,    getBaudRate()));
    elem.appendChild(Serialization::createListPropertyElement(doc, c_machinesPropName, getMachinesRef().constBegin(),
        getMachinesRef().constEnd()));
}

void CGridSimResourceConfig::loadDataFromXMLNode(const QDomElement &elem)
{
    QString name;
    QString arch;
    QString os;
    double costPerSec;
    double timeZone;
    qint32 allocPolicy;
    double baudRate;
    CGridSimMachinesConfig machines;

    CJavaXMLHelper::getPropertyValue(elem, c_namePropName,        name,        c_defaultName);
    CJavaXMLHelper::getPropertyValue(elem, c_archPropName,        arch,        c_defaultArch);
    CJavaXMLHelper::getPropertyValue(elem, c_osPropName,          os,          c_defaultOS);
    CJavaXMLHelper::getPropertyValue(elem, c_costPerSecPropName,  costPerSec,  c_defaultCostPerSec);
    CJavaXMLHelper::getPropertyValue(elem, c_timeZonePropName,    timeZone,    c_defaultTimeZone);
    CJavaXMLHelper::getPropertyValue(elem, c_allocPolicyPropName, allocPolicy, c_defaultAllocPolicy);
    CJavaXMLHelper::getPropertyValue(elem, c_baudRatePropName,    baudRate,    c_defaultBaudRate);

    getListPropertyValue(elem, c_machinesPropName, std::back_inserter(machines));

    setName(name);
    setArch(arch);
    setOS(os);
    setCostPerSec(costPerSec);
    setTimeZone(timeZone);
    setAllocPolicy(allocPolicy);
    setBaudRate(baudRate);
    setMachines(machines);
}

IJavaXMLSerializePtr CGridSimResourceConfig::create() const
{
    return IJavaXMLSerializePtr(new CGridSimResourceConfig());
}

void CGridSimResourceConfig::copy(const IJavaXMLSerializePtr &from)
{
    auto ptr = from.staticCast<CGridSimResourceConfig>();
    *this = *ptr;
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Tools
bool CGridSimResourceConfig::checkName(const QString &name)
{
    // Check name for format "Resource_<resource_id_start_from_0>
    //
    QRegExp nameRegExp(c_nameRegExp);
    nameRegExp.setCaseSensitivity(Qt::CaseSensitive);
    return nameRegExp.exactMatch(name);
}
#pragma endregion