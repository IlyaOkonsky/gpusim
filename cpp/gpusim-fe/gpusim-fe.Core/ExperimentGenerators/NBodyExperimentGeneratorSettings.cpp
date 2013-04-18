#include "NBodyExperimentGeneratorSettings.h"
#include "../Serialization/JavaXMLHelper.h"

using namespace Core::Settings;
using namespace Core::Serialization;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const quint32 CNBodyExperimentGeneratorSettings::c_defaultGPUCoreRating      = 1000;

const QString CNBodyExperimentGeneratorSettings::c_defaultResourceArch       = QString("N-Body Experiment Arch");
const QString CNBodyExperimentGeneratorSettings::c_defaultResourceOS         = QString("N-Body Experiment OS");
const double  CNBodyExperimentGeneratorSettings::c_defaultResourceBaudRate   = 10000000000.0f; // (10 Gbps)
const double  CNBodyExperimentGeneratorSettings::c_defaultResourceCostPerSec = 1.0f; // default cost?
const double  CNBodyExperimentGeneratorSettings::c_defaultLinkBaudRate       = 10000000000.0f; // (10 Gbps)

const double  CNBodyExperimentGeneratorSettings::c_defaultLimitationsDivider               = 128.0f;
const double  CNBodyExperimentGeneratorSettings::c_defaultSmallTPBPenaltyWeight            = 1.0f;
const double  CNBodyExperimentGeneratorSettings::c_defaultLargeTPBPenaltyWeight            = 0.5f;
const double  CNBodyExperimentGeneratorSettings::c_defaultMultiplicativeLengthScaleFactor  = 0.111f;
const double  CNBodyExperimentGeneratorSettings::c_defaultAdditiveLengthScaleFactor        = 0.0f;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString CNBodyExperimentGeneratorSettings::c_className                               = QString("gpusim-fe.NBEG-settings");
const QString CNBodyExperimentGeneratorSettings::c_gpuCoreRatingPropName                   = QString("gpuCoreRating");
const QString CNBodyExperimentGeneratorSettings::c_resourceArchPropName                    = QString("resourceArch");
const QString CNBodyExperimentGeneratorSettings::c_resourceOSPropName                      = QString("resourceOS");
const QString CNBodyExperimentGeneratorSettings::c_resourceBaudRatePropName                = QString("resourceBaudRate");
const QString CNBodyExperimentGeneratorSettings::c_resourceCostPerSecPropName              = QString("resourceCostPerSec");
const QString CNBodyExperimentGeneratorSettings::c_linkBaudRatePropName                    = QString("linkBaudRate");
const QString CNBodyExperimentGeneratorSettings::c_limitationsDividerPropName              = QString("limitationsDivider");
const QString CNBodyExperimentGeneratorSettings::c_smallTPBPenaltyWeightPropName           = QString("smallTPBPenaltyWeight");
const QString CNBodyExperimentGeneratorSettings::c_largeTPBPenaltyWeightPropName           = QString("largeTPBPenaltyWeight");
const QString CNBodyExperimentGeneratorSettings::c_multiplicativeLengthScaleFactorPropName = QString("multiplicativeLengthScaleFactor");
const QString CNBodyExperimentGeneratorSettings::c_additiveLengthScaleFactorPropName       = QString("additiveLengthScale");

#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor and comparision support
CNBodyExperimentGeneratorSettings::CNBodyExperimentGeneratorSettings()
    :IJavaXMLSerialize(c_className),
    m_gpuCoreRating(c_defaultGPUCoreRating), m_resourceArch(c_defaultResourceArch), m_resourceOS(c_defaultResourceOS),
    m_resourceBaudRate(c_defaultResourceBaudRate), m_resourceCostPerSec(c_defaultResourceCostPerSec),
    m_linkBaudRate(c_defaultLinkBaudRate), m_limitationsDivider(c_defaultLimitationsDivider),
    m_smallTPBPenaltyWeight(c_defaultSmallTPBPenaltyWeight), m_largeTPBPenaltyWeight(c_defaultLargeTPBPenaltyWeight),
    m_multiplicativeLengthScaleFactor(c_defaultMultiplicativeLengthScaleFactor),
    m_additiveLengthScaleFactor(c_defaultAdditiveLengthScaleFactor)
{

}

bool CNBodyExperimentGeneratorSettings::operator==(const CNBodyExperimentGeneratorSettings& other) const
{
    return
        (m_gpuCoreRating                   == other.m_gpuCoreRating                  ) &&
        (m_resourceArch                    == other.m_resourceArch                   ) &&
        (m_resourceOS                      == other.m_resourceOS                     ) &&
        (m_resourceBaudRate                == other.m_resourceBaudRate               ) &&
        (m_resourceCostPerSec              == other.m_resourceCostPerSec             ) &&
        (m_linkBaudRate                    == other.m_linkBaudRate                   ) &&
        (m_limitationsDivider              == other.m_limitationsDivider             ) &&
        (m_smallTPBPenaltyWeight           == other.m_smallTPBPenaltyWeight          ) &&
        (m_largeTPBPenaltyWeight           == other.m_largeTPBPenaltyWeight          ) &&
        (m_multiplicativeLengthScaleFactor == other.m_multiplicativeLengthScaleFactor) &&
        (m_additiveLengthScaleFactor       == other.m_additiveLengthScaleFactor      );
}

bool CNBodyExperimentGeneratorSettings::operator!=(const CNBodyExperimentGeneratorSettings& other) const
{
    return !(*this == other);
}

bool CNBodyExperimentGeneratorSettings::operator<(const CNBodyExperimentGeneratorSettings& other) const
{
    if (m_gpuCoreRating != other.m_gpuCoreRating)
        return m_gpuCoreRating < other.m_gpuCoreRating;

    if (m_resourceBaudRate != other.m_resourceBaudRate)
        return m_resourceBaudRate < other.m_resourceBaudRate;

    if (m_resourceCostPerSec != other.m_resourceCostPerSec)
        return m_resourceCostPerSec < other.m_resourceCostPerSec;

    if (m_linkBaudRate != other.m_linkBaudRate)
        return m_linkBaudRate < other.m_linkBaudRate;


    if (m_limitationsDivider != other.m_limitationsDivider)
        return m_limitationsDivider < other.m_limitationsDivider;

    if (m_smallTPBPenaltyWeight != other.m_smallTPBPenaltyWeight)
        return m_smallTPBPenaltyWeight < other.m_smallTPBPenaltyWeight;

    if (m_largeTPBPenaltyWeight != other.m_largeTPBPenaltyWeight)
        return m_largeTPBPenaltyWeight < other.m_largeTPBPenaltyWeight;

    if (m_multiplicativeLengthScaleFactor != other.m_multiplicativeLengthScaleFactor)
        return m_multiplicativeLengthScaleFactor < other.m_multiplicativeLengthScaleFactor;

    return m_additiveLengthScaleFactor < other.m_additiveLengthScaleFactor;
}

#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Properties
#pragma region GPUCoreRating property
quint32 CNBodyExperimentGeneratorSettings::getGPUCoreRating() const
{
    return m_gpuCoreRating;
}

void CNBodyExperimentGeneratorSettings::setGPUCoreRating(quint32 gpuCoreRating)
{
    m_gpuCoreRating= gpuCoreRating;
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

#pragma region LimitationsDivider property
double CNBodyExperimentGeneratorSettings::getLimitationsDivider() const
{
    return m_limitationsDivider;
}

void CNBodyExperimentGeneratorSettings::setLimitationsDivider(double limitationsDivider)
{
    m_limitationsDivider = limitationsDivider;
}
#pragma endregion

#pragma region SmallTPBPenaltyWeight property
double CNBodyExperimentGeneratorSettings::getSmallTPBPenaltyWeight() const
{
    return m_smallTPBPenaltyWeight;
}

void CNBodyExperimentGeneratorSettings::setSmallTPBPenaltyWeight(double smallTPBPenaltyWeight)
{
    m_smallTPBPenaltyWeight = smallTPBPenaltyWeight;
}
#pragma endregion

#pragma region LargeTPBPenaltyWeight property
double CNBodyExperimentGeneratorSettings::getLargeTPBPenaltyWeight() const
{
    return m_largeTPBPenaltyWeight;
}

void CNBodyExperimentGeneratorSettings::setLargeTPBPenaltyWeight(double largeTPBPenaltyWeight)
{
    m_largeTPBPenaltyWeight = largeTPBPenaltyWeight;
}
#pragma endregion

#pragma region MultiplicativeLengthScaleFactor property
double CNBodyExperimentGeneratorSettings::getMultiplicativeLengthScaleFactor() const
{
    return m_multiplicativeLengthScaleFactor;
}

void CNBodyExperimentGeneratorSettings::setMultiplicativeLengthScaleFactor(double multiplicativeLengthScaleFactor)
{
    m_multiplicativeLengthScaleFactor = multiplicativeLengthScaleFactor;
}
#pragma endregion

#pragma region AdditiveLengthScaleFactor property
double CNBodyExperimentGeneratorSettings::getAdditiveLengthScaleFactor() const
{
    return m_additiveLengthScaleFactor;
}

void CNBodyExperimentGeneratorSettings::setAdditiveLengthScaleFactor(double additiveLengthScaleFactor)
{
    m_additiveLengthScaleFactor = additiveLengthScaleFactor;
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
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_gpuCoreRatingPropName,                   getGPUCoreRating()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_resourceArchPropName,                    getResourceArch()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_resourceOSPropName,                      getResourceOS()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_resourceBaudRatePropName,                getResourceBaudRate()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_resourceCostPerSecPropName,              getResourceCostPerSec()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_linkBaudRatePropName,                    getLinkBaudRate()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_limitationsDividerPropName,              getLimitationsDivider()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_smallTPBPenaltyWeightPropName,           getSmallTPBPenaltyWeight()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_largeTPBPenaltyWeightPropName,           getLargeTPBPenaltyWeight()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_multiplicativeLengthScaleFactorPropName, getMultiplicativeLengthScaleFactor()));
    elem.appendChild(CJavaXMLHelper::createPropertyElement(doc, c_additiveLengthScaleFactorPropName,       getAdditiveLengthScaleFactor()));
}

void CNBodyExperimentGeneratorSettings::loadDataFromXMLNode(const QDomElement &elem)
{
    quint32 gpuCoreRating;

    QString resourceArch;
    QString resourceOS;
    double resourceBaudRate;
    double resourceCostPerSec;
    double linkBaudRate;

    double limitationsDivider;
    double smallTPBPenaltyWeight;
    double largeTPBPenaltyWeight;
    double multiplicativeLengthScaleFactor;
    double additiveLengthScaleFactor;

    CJavaXMLHelper::getPropertyValue(elem, c_gpuCoreRatingPropName,                   gpuCoreRating,                   c_defaultGPUCoreRating                  );
    CJavaXMLHelper::getPropertyValue(elem, c_resourceArchPropName,                    resourceArch,                    c_defaultResourceArch                   );
    CJavaXMLHelper::getPropertyValue(elem, c_resourceOSPropName,                      resourceOS,                      c_defaultResourceOS                     );
    CJavaXMLHelper::getPropertyValue(elem, c_resourceBaudRatePropName,                resourceBaudRate,                c_defaultResourceBaudRate               );
    CJavaXMLHelper::getPropertyValue(elem, c_resourceCostPerSecPropName,              resourceCostPerSec,              c_defaultResourceCostPerSec             );
    CJavaXMLHelper::getPropertyValue(elem, c_linkBaudRatePropName,                    linkBaudRate,                    c_defaultLinkBaudRate                   );
    CJavaXMLHelper::getPropertyValue(elem, c_limitationsDividerPropName,              limitationsDivider,              c_defaultLimitationsDivider             );
    CJavaXMLHelper::getPropertyValue(elem, c_smallTPBPenaltyWeightPropName,           smallTPBPenaltyWeight,           c_defaultSmallTPBPenaltyWeight          );
    CJavaXMLHelper::getPropertyValue(elem, c_largeTPBPenaltyWeightPropName,           largeTPBPenaltyWeight,           c_defaultLargeTPBPenaltyWeight          );
    CJavaXMLHelper::getPropertyValue(elem, c_multiplicativeLengthScaleFactorPropName, multiplicativeLengthScaleFactor, c_defaultMultiplicativeLengthScaleFactor);
    CJavaXMLHelper::getPropertyValue(elem, c_additiveLengthScaleFactorPropName,       additiveLengthScaleFactor,       c_defaultAdditiveLengthScaleFactor      );
    

    setGPUCoreRating(gpuCoreRating);
    setResourceArch(resourceArch);
    setResourceOS(resourceOS);
    setResourceBaudRate(resourceBaudRate);
    setResourceCostPerSec(resourceCostPerSec);
    setLinkBaudRate(linkBaudRate);
    setLimitationsDivider(limitationsDivider);
    setSmallTPBPenaltyWeight(smallTPBPenaltyWeight);
    setLargeTPBPenaltyWeight(largeTPBPenaltyWeight);
    setAdditiveLengthScaleFactor(additiveLengthScaleFactor);
    setMultiplicativeLengthScaleFactor(multiplicativeLengthScaleFactor);
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