#include "MatrixMultiplyExperimentGeneratorSettings.h"
#include "../Serialization/JavaXMLHelper.h"

using namespace Core::Settings;
using namespace Core::Serialization;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const quint32 CMatrixMultiplyExperimentGeneratorSettings::c_defaultCPUMachinePECount  = 8;
const quint32 CMatrixMultiplyExperimentGeneratorSettings::c_defaultCPUMachinePERating = 100;
const quint32 CMatrixMultiplyExperimentGeneratorSettings::c_defaultGPUMachinePECount  = 128;
const quint32 CMatrixMultiplyExperimentGeneratorSettings::c_defaultGPUMachinePERating = 20;

const QString CMatrixMultiplyExperimentGeneratorSettings::c_defaultResourceArch       = QString("Matrix Multiply Experiment Arch");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_defaultResourceOS         = QString("Matrix Multiply Experiment OS");
const double  CMatrixMultiplyExperimentGeneratorSettings::c_defaultResourceBaudRate   = 10000000000.0f; // (10 Gbps)
const double  CMatrixMultiplyExperimentGeneratorSettings::c_defaultResourceCostPerSec = 1.0f; // default cost?
const double  CMatrixMultiplyExperimentGeneratorSettings::c_defaultLinkBaudRate       = 10000000000.0f; // (10 Gbps)

const double  CMatrixMultiplyExperimentGeneratorSettings::c_defaultLoadOperationCost  = 10.0f;
const double  CMatrixMultiplyExperimentGeneratorSettings::c_defaultSaveOperationCost  = 100.0f;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CMatrixMultiplyExperimentGeneratorSettings::c_className                  = QString("gpusim-fe.MMEG-settings");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_cpuMachinePECountPropName  = QString("cpuMachineCount");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_cpuMachinePERatingPropName = QString("cpuMachineRating");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_gpuMachinePECountPropName  = QString("gpuMachineCount");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_gpuMachinePERatingPropName = QString("gpuMachineRating");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_resourceArchPropName       = QString("resourceArch");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_resourceOSPropName         = QString("resourceOS");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_resourceBaudRatePropName   = QString("resourceBaudRate");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_resourceCostPerSecPropName = QString("resourceCostPerSec");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_linkBaudRatePropName       = QString("linkBaudRate");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_loadOperationCostPropName  = QString("loadOperationCost");
const QString CMatrixMultiplyExperimentGeneratorSettings::c_saveOperationCostPropName  = QString("saveOperationCost");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor and comparision support
CMatrixMultiplyExperimentGeneratorSettings::CMatrixMultiplyExperimentGeneratorSettings()
    :IJavaXMLSerialize(c_className),
    m_cpuMachinePECount(c_defaultCPUMachinePECount), m_cpuMachinePERating(c_defaultCPUMachinePERating),
    m_gpuMachinePECount(c_defaultGPUMachinePECount), m_gpuMachinePERating(c_defaultGPUMachinePERating),
    m_resourceArch(c_defaultResourceArch), m_resourceOS(c_defaultResourceOS),
    m_resourceBaudRate(c_defaultResourceBaudRate), m_resourceCostPerSec(c_defaultResourceCostPerSec),
    m_linkBaudRate(c_defaultLinkBaudRate), m_loadOperationCost(c_defaultLoadOperationCost),
    m_saveOperationCost(c_defaultSaveOperationCost)
{

}

bool CMatrixMultiplyExperimentGeneratorSettings::operator==(const CMatrixMultiplyExperimentGeneratorSettings& other) const
{
    return
        (m_cpuMachinePECount  == other.m_cpuMachinePECount ) &&
        (m_cpuMachinePERating == other.m_cpuMachinePERating) &&
        (m_gpuMachinePECount  == other.m_gpuMachinePECount ) &&
        (m_gpuMachinePERating == other.m_gpuMachinePERating) &&
        (m_resourceArch       == other.m_resourceArch      ) &&
        (m_resourceOS         == other.m_resourceOS        ) &&
        (m_resourceBaudRate   == other.m_resourceBaudRate  ) &&
        (m_resourceCostPerSec == other.m_resourceCostPerSec) &&
        (m_linkBaudRate       == other.m_linkBaudRate      ) &&
        (m_loadOperationCost  == other.m_loadOperationCost ) &&
        (m_saveOperationCost  == other.m_saveOperationCost );
}

bool CMatrixMultiplyExperimentGeneratorSettings::operator!=(const CMatrixMultiplyExperimentGeneratorSettings& other) const
{
    return !(*this == other);
}

bool CMatrixMultiplyExperimentGeneratorSettings::operator<(const CMatrixMultiplyExperimentGeneratorSettings& other) const
{
    if (m_cpuMachinePECount != other.m_cpuMachinePECount)
        return m_cpuMachinePECount < other.m_cpuMachinePECount;

    if (m_cpuMachinePERating != other.m_cpuMachinePERating)
        return m_cpuMachinePERating < other.m_cpuMachinePERating;

    if (m_gpuMachinePECount != other.m_gpuMachinePECount)
        return m_gpuMachinePECount < other.m_gpuMachinePECount;

    if (m_gpuMachinePERating != other.m_gpuMachinePERating)
        return m_gpuMachinePERating < other.m_gpuMachinePERating;

    if (m_resourceBaudRate != other.m_resourceBaudRate)
        return m_resourceBaudRate < other.m_resourceBaudRate;

    if (m_resourceCostPerSec != other.m_resourceCostPerSec)
        return m_resourceCostPerSec < other.m_resourceCostPerSec;

    if (m_linkBaudRate != other.m_linkBaudRate)
        return m_linkBaudRate < other.m_linkBaudRate;

    if (m_loadOperationCost != other.m_loadOperationCost)
        return m_loadOperationCost < other.m_loadOperationCost;

    return m_saveOperationCost < other.m_saveOperationCost;
}

#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region CPUMachinePECount property
quint32 CMatrixMultiplyExperimentGeneratorSettings::getCPUMachinePECount() const
{
    return m_cpuMachinePECount;
}

void CMatrixMultiplyExperimentGeneratorSettings::setCPUMachinePECount(quint32 cpuMachineCount)
{
    m_cpuMachinePECount = cpuMachineCount;
}
#pragma endregion

#pragma region CPUMachinePERating property
quint32 CMatrixMultiplyExperimentGeneratorSettings::getCPUMachinePERating() const
{
    return m_cpuMachinePERating;
}

void CMatrixMultiplyExperimentGeneratorSettings::setCPUMachinePERating(quint32 cpuMachineRating)
{
    m_cpuMachinePERating = cpuMachineRating;
}
#pragma endregion

#pragma region GPUMachinePECount property
quint32 CMatrixMultiplyExperimentGeneratorSettings::getGPUMachinePECount() const
{
    return m_cpuMachinePECount;
}

void CMatrixMultiplyExperimentGeneratorSettings::setGPUMachinePECount(quint32 gpuMachineCount)
{
    m_gpuMachinePECount = gpuMachineCount;
}
#pragma endregion

#pragma region GPUMachinePERating property
quint32 CMatrixMultiplyExperimentGeneratorSettings::getGPUMachinePERating() const
{
    return m_cpuMachinePERating;
}

void CMatrixMultiplyExperimentGeneratorSettings::setGPUMachinePERating(quint32 gpuMachineRating)
{
    m_gpuMachinePERating = gpuMachineRating;
}
#pragma endregion

#pragma region ResourceArch property
QString CMatrixMultiplyExperimentGeneratorSettings::getResourceArch() const
{
    return m_resourceArch;
}

void CMatrixMultiplyExperimentGeneratorSettings::setResourceArch(const QString &resourceArch)
{
    m_resourceArch = resourceArch;
}
#pragma endregion

#pragma region ResourceOS property
QString CMatrixMultiplyExperimentGeneratorSettings::getResourceOS() const
{
    return m_resourceOS;
}

void CMatrixMultiplyExperimentGeneratorSettings::setResourceOS(const QString &resourceOS)
{
    m_resourceOS = resourceOS;
}
#pragma endregion

#pragma region ResourceBaudRate property
double CMatrixMultiplyExperimentGeneratorSettings::getResourceBaudRate() const
{
    return m_resourceBaudRate;
}

void CMatrixMultiplyExperimentGeneratorSettings::setResourceBaudRate(double resourceBaudRate)
{
    m_resourceBaudRate = resourceBaudRate;
}
#pragma endregion

#pragma region ResourceCostPerSec property
double CMatrixMultiplyExperimentGeneratorSettings::getResourceCostPerSec() const
{
    return m_resourceCostPerSec;
}

void CMatrixMultiplyExperimentGeneratorSettings::setResourceCostPerSec(double resourceCostPerSec)
{
    m_resourceCostPerSec = resourceCostPerSec;
}
#pragma endregion

#pragma region LinkBaudRate property
double CMatrixMultiplyExperimentGeneratorSettings::getLinkBaudRate() const
{
    return m_linkBaudRate;
}

void CMatrixMultiplyExperimentGeneratorSettings::setLinkBaudRate(double linkBaudRate)
{
    m_linkBaudRate = linkBaudRate;
}
#pragma endregion

#pragma region LoadOperationCost property
double CMatrixMultiplyExperimentGeneratorSettings::getLoadOperationCost() const
{
    return m_loadOperationCost;
}

void CMatrixMultiplyExperimentGeneratorSettings::setLoadOperationCost(double loadOperationCost)
{
    m_loadOperationCost = loadOperationCost;
}
#pragma endregion

#pragma region SaveOperationCost property
double CMatrixMultiplyExperimentGeneratorSettings::getSaveOperationCost() const
{
    return m_saveOperationCost;
}

void CMatrixMultiplyExperimentGeneratorSettings::setSaveOperationCost(double saveOperationCost)
{
    m_saveOperationCost = saveOperationCost;
}
#pragma endregion
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Serialization support
bool CMatrixMultiplyExperimentGeneratorSettings::isValid() const
{
    // WARNING: empty validation check (stub always returns true);
    return true;
}

void CMatrixMultiplyExperimentGeneratorSettings::saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const
{
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_cpuMachinePECountPropName,  getCPUMachinePECount()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_cpuMachinePERatingPropName, getCPUMachinePERating()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_gpuMachinePECountPropName,  getGPUMachinePECount()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_gpuMachinePERatingPropName, getGPUMachinePERating()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_resourceArchPropName,       getResourceArch()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_resourceOSPropName,         getResourceOS()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_resourceBaudRatePropName,   getResourceBaudRate()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_resourceCostPerSecPropName, getResourceCostPerSec()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_linkBaudRatePropName,       getLinkBaudRate()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_loadOperationCostPropName,  getLoadOperationCost()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_saveOperationCostPropName,  getSaveOperationCost()));
}

void CMatrixMultiplyExperimentGeneratorSettings::loadDataFromXMLNode(const QDomElement &elem)
{
    quint32 cpuMachinePECount;
    quint32 cpuMachinePERating;
    quint32 gpuMachinePECount;
    quint32 gpuMachinePERating;

    QString resourceArch;
    QString resourceOS;
    double resourceBaudRate;
    double resourceCostPerSec;
    double linkBaudRate;

    double loadOperationCost;
    double saveOperationCost;

    CJavaXMLHelper::getPropertyValue(elem, c_cpuMachinePECountPropName,  cpuMachinePECount,   c_defaultCPUMachinePECount );
    CJavaXMLHelper::getPropertyValue(elem, c_cpuMachinePERatingPropName, cpuMachinePERating,  c_defaultCPUMachinePERating);
    CJavaXMLHelper::getPropertyValue(elem, c_gpuMachinePECountPropName,  gpuMachinePECount,   c_defaultGPUMachinePECount );
    CJavaXMLHelper::getPropertyValue(elem, c_gpuMachinePERatingPropName, gpuMachinePERating,  c_defaultGPUMachinePERating);
    CJavaXMLHelper::getPropertyValue(elem, c_resourceArchPropName,       resourceArch,        c_defaultResourceArch      );
    CJavaXMLHelper::getPropertyValue(elem, c_resourceOSPropName,         resourceOS,          c_defaultResourceOS        );
    CJavaXMLHelper::getPropertyValue(elem, c_resourceBaudRatePropName,   resourceBaudRate,    c_defaultResourceBaudRate  );
    CJavaXMLHelper::getPropertyValue(elem, c_resourceCostPerSecPropName, resourceCostPerSec,  c_defaultResourceCostPerSec);
    CJavaXMLHelper::getPropertyValue(elem, c_linkBaudRatePropName,       linkBaudRate,        c_defaultLinkBaudRate      );
    CJavaXMLHelper::getPropertyValue(elem, c_loadOperationCostPropName,  loadOperationCost,   c_defaultLoadOperationCost );
    CJavaXMLHelper::getPropertyValue(elem, c_saveOperationCostPropName,  saveOperationCost,   c_defaultSaveOperationCost );

    setCPUMachinePECount(cpuMachinePECount);
    setCPUMachinePERating(cpuMachinePERating);
    setGPUMachinePECount(gpuMachinePECount);
    setGPUMachinePERating(gpuMachinePERating);
    setResourceArch(resourceArch);
    setResourceOS(resourceOS);
    setResourceBaudRate(resourceBaudRate);
    setResourceCostPerSec(resourceCostPerSec);
    setLinkBaudRate(linkBaudRate);
    setLoadOperationCost(loadOperationCost);
    setSaveOperationCost(saveOperationCost);
}

IJavaXMLSerializePtr CMatrixMultiplyExperimentGeneratorSettings::create() const
{
    return IJavaXMLSerializePtr(new CMatrixMultiplyExperimentGeneratorSettings());
}

void CMatrixMultiplyExperimentGeneratorSettings::copy(const IJavaXMLSerializePtr &from)
{
    auto ptr = from.staticCast<CMatrixMultiplyExperimentGeneratorSettings>();
    *this = *ptr;
}
#pragma endregion