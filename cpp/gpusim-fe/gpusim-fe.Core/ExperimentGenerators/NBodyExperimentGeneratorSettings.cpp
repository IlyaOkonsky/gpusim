#include "NBodyExperimentGeneratorSettings.h"
#include "../Serialization/JavaXMLHelper.h"

using namespace Core::Settings;
using namespace Core::Serialization;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const quint32 CNBodyExperimentGeneratorSettings::c_defaultCPUMachinePECount  = 8;
const quint32 CNBodyExperimentGeneratorSettings::c_defaultCPUMachinePERating = 100;
const quint32 CNBodyExperimentGeneratorSettings::c_defaultGPUMachinePECount  = 128;
const quint32 CNBodyExperimentGeneratorSettings::c_defaultGPUMachinePERating = 20;

const QString CNBodyExperimentGeneratorSettings::c_defaultResourceArch       = QString("Matrix Multiply Experiment Arch");
const QString CNBodyExperimentGeneratorSettings::c_defaultResourceOS         = QString("Matrix Multiply Experiment OS");
const double  CNBodyExperimentGeneratorSettings::c_defaultResourceBaudRate   = 10000000000.0f; // (10 Gbps)
const double  CNBodyExperimentGeneratorSettings::c_defaultResourceCostPerSec = 1.0f; // default cost?
const double  CNBodyExperimentGeneratorSettings::c_defaultLinkBaudRate       = 10000000000.0f; // (10 Gbps)

const double  CNBodyExperimentGeneratorSettings::c_defaultLoadOperationCost  = 10.0f;
const double  CNBodyExperimentGeneratorSettings::c_defaultSaveOperationCost  = 100.0f;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CNBodyExperimentGeneratorSettings::c_className                  = QString("gpusim-fe.MMEG-settings");
const QString CNBodyExperimentGeneratorSettings::c_cpuMachinePECountPropName  = QString("cpuMachineCount");
const QString CNBodyExperimentGeneratorSettings::c_cpuMachinePERatingPropName = QString("cpuMachineRating");
const QString CNBodyExperimentGeneratorSettings::c_gpuMachinePECountPropName  = QString("gpuMachineCount");
const QString CNBodyExperimentGeneratorSettings::c_gpuMachinePERatingPropName = QString("gpuMachineRating");
const QString CNBodyExperimentGeneratorSettings::c_resourceArchPropName       = QString("resourceArch");
const QString CNBodyExperimentGeneratorSettings::c_resourceOSPropName         = QString("resourceOS");
const QString CNBodyExperimentGeneratorSettings::c_resourceBaudRatePropName   = QString("resourceBaudRate");
const QString CNBodyExperimentGeneratorSettings::c_resourceCostPerSecPropName = QString("resourceCostPerSec");
const QString CNBodyExperimentGeneratorSettings::c_linkBaudRatePropName       = QString("linkBaudRate");
const QString CNBodyExperimentGeneratorSettings::c_loadOperationCostPropName  = QString("loadOperationCost");
const QString CNBodyExperimentGeneratorSettings::c_saveOperationCostPropName  = QString("saveOperationCost");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor and comparision support
CNBodyExperimentGeneratorSettings::CNBodyExperimentGeneratorSettings()
    :IJavaXMLSerialize(c_className),
    m_cpuMachinePECount(c_defaultCPUMachinePECount), m_cpuMachinePERating(c_defaultCPUMachinePERating),
    m_gpuMachinePECount(c_defaultGPUMachinePECount), m_gpuMachinePERating(c_defaultGPUMachinePERating),
    m_resourceArch(c_defaultResourceArch), m_resourceOS(c_defaultResourceOS),
    m_resourceBaudRate(c_defaultResourceBaudRate), m_resourceCostPerSec(c_defaultResourceCostPerSec),
    m_linkBaudRate(c_defaultLinkBaudRate), m_loadOperationCost(c_defaultLoadOperationCost),
    m_saveOperationCost(c_defaultSaveOperationCost)
{

}

bool CNBodyExperimentGeneratorSettings::operator==(const CNBodyExperimentGeneratorSettings& other) const
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

bool CNBodyExperimentGeneratorSettings::operator!=(const CNBodyExperimentGeneratorSettings& other) const
{
    return !(*this == other);
}

bool CNBodyExperimentGeneratorSettings::operator<(const CNBodyExperimentGeneratorSettings& other) const
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
quint32 CNBodyExperimentGeneratorSettings::getCPUMachinePECount() const
{
    return m_cpuMachinePECount;
}

void CNBodyExperimentGeneratorSettings::setCPUMachinePECount(quint32 cpuMachineCount)
{
    m_cpuMachinePECount = cpuMachineCount;
}
#pragma endregion

#pragma region CPUMachinePERating property
quint32 CNBodyExperimentGeneratorSettings::getCPUMachinePERating() const
{
    return m_cpuMachinePERating;
}

void CNBodyExperimentGeneratorSettings::setCPUMachinePERating(quint32 cpuMachineRating)
{
    m_cpuMachinePERating = cpuMachineRating;
}
#pragma endregion

#pragma region GPUMachinePECount property
quint32 CNBodyExperimentGeneratorSettings::getGPUMachinePECount() const
{
    return m_gpuMachinePECount;
}

void CNBodyExperimentGeneratorSettings::setGPUMachinePECount(quint32 gpuMachineCount)
{
    m_gpuMachinePECount = gpuMachineCount;
}
#pragma endregion

#pragma region GPUMachinePERating property
quint32 CNBodyExperimentGeneratorSettings::getGPUMachinePERating() const
{
    return m_gpuMachinePERating;
}

void CNBodyExperimentGeneratorSettings::setGPUMachinePERating(quint32 gpuMachineRating)
{
    m_gpuMachinePERating = gpuMachineRating;
}
#pragma endregion

#pragma region ResourceArch property
QString CNBodyExperimentGeneratorSettings::getResourceArch() const
{
    return m_resourceArch;
}

void CNBodyExperimentGeneratorSettings::setResourceArch(const QString &resourceArch)
{
    m_resourceArch = resourceArch;
}
#pragma endregion

#pragma region ResourceOS property
QString CNBodyExperimentGeneratorSettings::getResourceOS() const
{
    return m_resourceOS;
}

void CNBodyExperimentGeneratorSettings::setResourceOS(const QString &resourceOS)
{
    m_resourceOS = resourceOS;
}
#pragma endregion

#pragma region ResourceBaudRate property
double CNBodyExperimentGeneratorSettings::getResourceBaudRate() const
{
    return m_resourceBaudRate;
}

void CNBodyExperimentGeneratorSettings::setResourceBaudRate(double resourceBaudRate)
{
    m_resourceBaudRate = resourceBaudRate;
}
#pragma endregion

#pragma region ResourceCostPerSec property
double CNBodyExperimentGeneratorSettings::getResourceCostPerSec() const
{
    return m_resourceCostPerSec;
}

void CNBodyExperimentGeneratorSettings::setResourceCostPerSec(double resourceCostPerSec)
{
    m_resourceCostPerSec = resourceCostPerSec;
}
#pragma endregion

#pragma region LinkBaudRate property
double CNBodyExperimentGeneratorSettings::getLinkBaudRate() const
{
    return m_linkBaudRate;
}

void CNBodyExperimentGeneratorSettings::setLinkBaudRate(double linkBaudRate)
{
    m_linkBaudRate = linkBaudRate;
}
#pragma endregion

#pragma region LoadOperationCost property
double CNBodyExperimentGeneratorSettings::getLoadOperationCost() const
{
    return m_loadOperationCost;
}

void CNBodyExperimentGeneratorSettings::setLoadOperationCost(double loadOperationCost)
{
    m_loadOperationCost = loadOperationCost;
}
#pragma endregion

#pragma region SaveOperationCost property
double CNBodyExperimentGeneratorSettings::getSaveOperationCost() const
{
    return m_saveOperationCost;
}

void CNBodyExperimentGeneratorSettings::setSaveOperationCost(double saveOperationCost)
{
    m_saveOperationCost = saveOperationCost;
}
#pragma endregion
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Serialization support
bool CNBodyExperimentGeneratorSettings::isValid() const
{
    // WARNING: empty validation check (stub always returns true);
    return true;
}

void CNBodyExperimentGeneratorSettings::saveDataToXMLNode(QDomDocument &doc, QDomElement &elem) const
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

void CNBodyExperimentGeneratorSettings::loadDataFromXMLNode(const QDomElement &elem)
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

IJavaXMLSerializePtr CNBodyExperimentGeneratorSettings::create() const
{
    return IJavaXMLSerializePtr(new CNBodyExperimentGeneratorSettings());
}

void CNBodyExperimentGeneratorSettings::copy(const IJavaXMLSerializePtr &from)
{
    auto ptr = from.staticCast<CNBodyExperimentGeneratorSettings>();
    *this = *ptr;
}
#pragma endregion