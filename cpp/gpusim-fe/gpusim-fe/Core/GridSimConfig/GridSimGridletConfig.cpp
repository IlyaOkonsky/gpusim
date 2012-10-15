#include "GridSimGridletConfig.h"
#include "../Serialization/JavaXMLHelper.h"

using namespace Core::GridSimConfig;
using namespace Core::Serialization;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const double  CGridSimGridletConfig::c_minLength     = 0.0f;
const quint64 CGridSimGridletConfig::c_minInputSize  = 1;
const quint64 CGridSimGridletConfig::c_minOutputSize = 1;

const quint32 CGridSimGridletConfig::c_defaultID         = 0;
const double  CGridSimGridletConfig::c_defaultLength     = c_minLength;
const quint64 CGridSimGridletConfig::c_defaultInputSize  = c_minInputSize;
const quint64 CGridSimGridletConfig::c_defaultOutputSize = c_minOutputSize;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CGridSimGridletConfig::c_className          = QString("gpusim.config.GridSimGridletConfig");
const QString CGridSimGridletConfig::c_idPropName         = QString("ID");
const QString CGridSimGridletConfig::c_lengthPropName     = QString("length");
const QString CGridSimGridletConfig::c_inputSizePropName  = QString("inputSize");
const QString CGridSimGridletConfig::c_outputSizePropName = QString("outputSize");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
//////////////////////////////////////////////////////////////////////////

#pragma region Constructor and equality comparision support
CGridSimGridletConfig::CGridSimGridletConfig(quint32 id /*= c_defaultID*/, double length /*= c_defaultLength*/,
    quint64 inputSize /*= c_defaultInputSize*/, quint64 outputSize /*= c_defaultOutputSize*/)
    : IJavaXMLSerialize(c_className), m_id(c_defaultID), m_length(c_defaultLength), m_inputSize(c_defaultInputSize),
    m_outputSize(c_defaultOutputSize)
{
    setID(id);
    setLength(length);
    setInputSize(inputSize);
    setOutputSize(outputSize);
}

bool CGridSimGridletConfig::operator==(const CGridSimGridletConfig& other) const
{
    return
        (m_id         == other.m_id        ) &&
        (m_length     == other.m_length    ) &&
        (m_inputSize  == other.m_inputSize ) &&
        (m_outputSize == other.m_outputSize);
}

bool CGridSimGridletConfig::operator!=(const CGridSimGridletConfig& other) const
{
    return !(*this == other);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region ID property
quint32 CGridSimGridletConfig::getID() const
{
    return m_id;
}

void CGridSimGridletConfig::setID(quint32 id)
{
    if (m_id == id)
        return;

    m_id = id;
}
#pragma endregion

#pragma region Length property
double CGridSimGridletConfig::getLength() const
{
    return m_length;
}

void CGridSimGridletConfig::setLength(double length)
{
    if (m_length == length)
        return;

    Q_ASSERT(length >= c_minLength);
    m_length = length;
}
#pragma endregion

#pragma region InputSize property
quint64 CGridSimGridletConfig::getInputSize() const
{
    return m_inputSize;
}

void CGridSimGridletConfig::setInputSize(quint64 inputSize)
{
    if (m_inputSize == inputSize)
        return;

    Q_ASSERT(inputSize >= c_minInputSize);
    m_inputSize = inputSize;
}
#pragma endregion

#pragma region OutputSize property
quint64 CGridSimGridletConfig::getOutputSize() const
{
    return m_outputSize;
}

void CGridSimGridletConfig::setOutputSize(quint64 outputSize)
{
    if (m_outputSize == outputSize)
        return;

    Q_ASSERT(outputSize >= c_minOutputSize);
    m_outputSize = outputSize;
}
#pragma endregion
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Serialization support
bool CGridSimGridletConfig::isValid() const
{
    return (m_length >= c_minLength) && (m_inputSize >= c_minInputSize) && (m_outputSize >= c_minOutputSize);
}

void CGridSimGridletConfig::saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const
{
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_idPropName,         getID()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_lengthPropName,     getLength()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_inputSizePropName,  getInputSize()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_outputSizePropName, getOutputSize()));
}

void CGridSimGridletConfig::loadDataFromXMLNode(const QDomElement &elem)
{
    quint32 id;
    double length;
    quint64 inputSize;
    quint64 outputSize;

    CJavaXMLHelper::getPropertyValue(elem, c_idPropName,         id,         c_defaultID);
    CJavaXMLHelper::getPropertyValue(elem, c_lengthPropName,     length,     c_defaultLength);
    CJavaXMLHelper::getPropertyValue(elem, c_inputSizePropName,  inputSize,  c_defaultInputSize);
    CJavaXMLHelper::getPropertyValue(elem, c_outputSizePropName, outputSize, c_defaultOutputSize);

    setID(id);
    setLength(length);
    setInputSize(inputSize);
    setOutputSize(outputSize);
}

IJavaXMLSerializePtr CGridSimGridletConfig::create() const
{
    return IJavaXMLSerializePtr(new CGridSimGridletConfig());
}

void CGridSimGridletConfig::copy(const IJavaXMLSerializePtr &from)
{
    auto ptr = from.staticCast<CGridSimGridletConfig>();
    *this = *ptr;
}
#pragma endregion