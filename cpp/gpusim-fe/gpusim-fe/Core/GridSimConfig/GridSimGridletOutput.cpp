#include "GridSimGridletOutput.h"
#include "../Serialization/JavaXMLHelper.h"

using namespace Core::GridSimConfig;
using namespace Core::Serialization;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const double  CGridSimGridletOutput::c_minTime               = 0.0f;
const double  CGridSimGridletOutput::c_minProcessingCost     = 0.0f;

const quint32 CGridSimGridletOutput::c_defaultID             = 0;
const double  CGridSimGridletOutput::c_defaultExecStartTime  = c_minTime;
const double  CGridSimGridletOutput::c_defaultExecFinishTime = c_minTime;
const double  CGridSimGridletOutput::c_defaultProcessingCost = c_minProcessingCost;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CGridSimGridletOutput::c_className              = QString("gpusim.config.GridSimGridletOutput");
const QString CGridSimGridletOutput::c_idPropName             = QString("ID");
const QString CGridSimGridletOutput::c_execStartTimePropName  = QString("execStartTime");
const QString CGridSimGridletOutput::c_execFinishTimePropName = QString("execFinishTime");
const QString CGridSimGridletOutput::c_processingCostPropName = QString("processingCost");
const QString CGridSimGridletOutput::c_HistoryPropName        = QString("history");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
//////////////////////////////////////////////////////////////////////////

#pragma region Constructor and equality comparision support
CGridSimGridletOutput::CGridSimGridletOutput(quint32 id /*= c_defaultID*/,
    double execStartTime /*= c_defaultExecStartTime*/, double execFinishTime /*= c_defaultExecFinishTime*/,
    double processingCost /*= c_defaultProcessingCost*/, const QString &history /*= QString()*/)
    :IJavaXMLSerialize(c_className), m_id(c_defaultID), m_execStartTime(c_defaultExecStartTime),
    m_execFinishTime(c_defaultExecFinishTime), m_processingCost(c_defaultProcessingCost)
{
    setID(id);
    setExecStartTime(execStartTime);
    setExecFinishTime(execFinishTime);
    setProcessingCost(processingCost);
    setHistory(history);
}

bool CGridSimGridletOutput::operator==(const CGridSimGridletOutput& other) const
{
    return
        (m_id             == other.m_id            ) &&
        (m_execStartTime  == other.m_execStartTime ) &&
        (m_execFinishTime == other.m_execFinishTime) &&
        (m_processingCost == other.m_processingCost) &&
        (m_history        == other.m_history       );
}

bool CGridSimGridletOutput::operator!=(const CGridSimGridletOutput& other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region ID property
quint32 CGridSimGridletOutput::getID() const
{
    return m_id;
}

void CGridSimGridletOutput::setID(quint32 id)
{
    if (m_id == id)
        return;

    m_id = id;
}
#pragma endregion

#pragma region ExecStartTime property
double CGridSimGridletOutput::getExecStartTime() const
{
    return m_execStartTime;
}

void CGridSimGridletOutput::setExecStartTime(double execStartTime)
{
    if (m_execStartTime == execStartTime)
        return;

    Q_ASSERT(execStartTime >= c_minTime);
    m_execStartTime = execStartTime;
}
#pragma endregion

#pragma region ExecFinishTime property
double CGridSimGridletOutput::getExecFinishTime() const
{
    return m_execFinishTime;
}

void CGridSimGridletOutput::setExecFinishTime(double execFinishTime)
{
    if (m_execFinishTime == execFinishTime)
        return;

    Q_ASSERT(execFinishTime >= c_minTime);
    m_execFinishTime = execFinishTime;
}
#pragma endregion

#pragma region ProcessingCost property
double CGridSimGridletOutput::getProcessingCost() const
{
    return m_processingCost;
}

void CGridSimGridletOutput::setProcessingCost(double processingCost)
{
    if (m_processingCost == processingCost)
        return;

    Q_ASSERT(processingCost >= c_minProcessingCost);
    m_processingCost = processingCost;
}
#pragma endregion

#pragma region History property
QString CGridSimGridletOutput::getHistory() const
{
    return m_history;
}

void CGridSimGridletOutput::setHistory(const QString &history)
{
    if (m_history == history)
        return;

    m_history = history;
}
#pragma endregion
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Serialization support
bool CGridSimGridletOutput::isValid() const
{
    return (m_execStartTime >= c_minTime) && (m_execFinishTime>= c_minTime) &&
        (m_processingCost >= c_minProcessingCost);
}

void CGridSimGridletOutput::saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const
{
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_idPropName,             getID()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_execStartTimePropName,  getExecStartTime()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_execFinishTimePropName, getExecFinishTime()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_processingCostPropName, getProcessingCost()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_HistoryPropName,        getHistory()));
}

void CGridSimGridletOutput::loadDataFromXMLNode(const QDomElement &elem)
{
    quint32 id;
    double execStartTime;
    double execFinishTime;
    double processingCost;
    QString history;

    CJavaXMLHelper::getPropertyValue(elem, c_idPropName,             id,             c_defaultID);
    CJavaXMLHelper::getPropertyValue(elem, c_execStartTimePropName,  execStartTime,  c_defaultExecStartTime);
    CJavaXMLHelper::getPropertyValue(elem, c_execFinishTimePropName, execFinishTime, c_defaultExecFinishTime);
    CJavaXMLHelper::getPropertyValue(elem, c_processingCostPropName, processingCost, c_defaultProcessingCost);
    CJavaXMLHelper::getPropertyValue(elem, c_HistoryPropName,        history);

    setID(id);
    setExecStartTime(execStartTime);
    setExecFinishTime(execFinishTime);
    setProcessingCost(processingCost);
    setHistory(history);
}

IJavaXMLSerializePtr CGridSimGridletOutput::create() const
{
    return IJavaXMLSerializePtr(new CGridSimGridletOutput());
}

void CGridSimGridletOutput::copy(const IJavaXMLSerializePtr &from)
{
    auto ptr = from.staticCast<CGridSimGridletOutput>();
    *this = *ptr;
}
#pragma endregion