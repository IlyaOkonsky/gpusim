#include "GridSimMachineConfig.h"
#include "../Serialization/JavaXMLHelper.h"

using namespace Core::GridSimConfig;
using namespace Core::Serialization;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const quint32 CGridSimMachineConfig::c_minPECount  = 1;
const quint32 CGridSimMachineConfig::c_minPERating = 1;

const quint32 CGridSimMachineConfig::c_defaultlID      = 0;
const quint32 CGridSimMachineConfig::c_defaultPECount  = c_minPECount;
const quint32 CGridSimMachineConfig::c_defaultPERating = c_minPERating;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CGridSimMachineConfig::c_className        = QString("gpusim.config.GridSimMachineConfig");
const QString CGridSimMachineConfig::c_idPropName       = QString("ID");
const QString CGridSimMachineConfig::c_peCountPropName  = QString("PECount");
const QString CGridSimMachineConfig::c_peRatingPropName = QString("PERating");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
//////////////////////////////////////////////////////////////////////////

#pragma region Constructor and equality comparision support
CGridSimMachineConfig::CGridSimMachineConfig(quint32 id /*= c_defaultlID*/, quint32 peCount /*= c_defaultPECount*/,
    quint32 peRating /*= c_defaultPERating*/)
    :IJavaXMLSerialize(c_className), m_id(c_defaultlID), m_peCount(c_defaultPECount), m_peRating(c_defaultPERating)
{
    setID(id);
    setPECount(peCount);
    setPERating(peRating);
}

bool CGridSimMachineConfig::operator==(const CGridSimMachineConfig& other) const
{
    return
        (m_id       == other.m_id      ) &&
        (m_peCount  == other.m_peCount ) &&
        (m_peRating == other.m_peRating);
}

bool CGridSimMachineConfig::operator!=(const CGridSimMachineConfig& other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region ID property
quint32 CGridSimMachineConfig::getID() const
{
    return m_id;
}

void CGridSimMachineConfig::setID(quint32 id)
{
    if (m_id == id)
        return;

    m_id = id;
}
#pragma endregion

#pragma region PECount property
quint32 CGridSimMachineConfig::getPECount() const
{
    return m_peCount;
}

void CGridSimMachineConfig::setPECount(quint32 peCount)
{
    if (m_peCount == peCount)
        return;

    Q_ASSERT(peCount >= c_minPECount);
    m_peCount = peCount;
}
#pragma endregion

#pragma region PERating property
quint32 CGridSimMachineConfig::getPERating() const
{
    return m_peRating;
}

void CGridSimMachineConfig::setPERating(quint32 peRating)
{
    if (m_peRating == peRating)
        return;

    Q_ASSERT(peRating >= c_minPERating);
    m_peRating = peRating;
}
#pragma endregion
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Serialization support
bool CGridSimMachineConfig::isValid() const
{
    return (m_peCount>= c_minPECount) && (m_peRating >= c_minPERating);
}

void CGridSimMachineConfig::saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const
{
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_idPropName,       getID()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_peCountPropName,  getPECount()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_peRatingPropName, getPERating()));
}

void CGridSimMachineConfig::loadDataFromXMLNode(const QDomElement &elem)
{
    quint32 id;
    quint32 peCount;
    quint32 peRating;

    CJavaXMLHelper::getPropertyValue(elem, c_idPropName,       id,       c_defaultlID);
    CJavaXMLHelper::getPropertyValue(elem, c_peCountPropName,  peCount,  c_defaultPECount);
    CJavaXMLHelper::getPropertyValue(elem, c_peRatingPropName, peRating, c_defaultPERating);

    setID(id);
    setPECount(peCount);
    setPERating(peRating);
}

IJavaXMLSerializePtr CGridSimMachineConfig::create() const
{
    return IJavaXMLSerializePtr(new CGridSimMachineConfig());
}

void CGridSimMachineConfig::copy(const IJavaXMLSerializePtr &from)
{
    auto ptr = from.staticCast<CGridSimMachineConfig>();
    *this = *ptr;
}
#pragma endregion