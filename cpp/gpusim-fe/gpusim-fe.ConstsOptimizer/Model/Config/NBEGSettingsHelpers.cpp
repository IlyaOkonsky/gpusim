#include "NBEGSettingsHelpers.h"

void Model::setNBEGSProp(CNBEGSettingsProps prop, CNBodyExperimentGeneratorSettings &s, quint32 val)
{
    switch(prop)
    {
    case NBEGSProp_gpuCoreRating:
        s.setGPUCoreRating(val);
        break;

    default:
        Q_ASSERT(0 && "Wrong property");
        break;
    }
}

void Model::setNBEGSProp(CNBEGSettingsProps prop, CNBodyExperimentGeneratorSettings &s, double val)
{
    switch(prop)
    {
    case NBEGSProp_resourceBaudRate:
        s.setResourceBaudRate(val);
        break;

    case NBEGSProp_resourceCostPerSec:
        s.setResourceCostPerSec(val);
        break;

    case NBEGSProp_linkBaudRate:
        s.setLinkBaudRate(val);
        break;

    case NBEGSProp_limitationsDivider:
        s.setLimitationsDivider(val);
        break;

    case NBEGSProp_smallTPBPenaltyWeight:
        s.setSmallTPBPenaltyWeight(val);
        break;

    case NBEGSProp_largeTPBPenaltyWeight:
        s.setLargeTPBPenaltyWeight(val);
        break;
    case NBEGSProp_multiplicativeLengthScaleFactor:
        s.setMultiplicativeLengthScaleFactor(val);
        break;

    case NBEGSProp_additiveLengthScaleFactor:
        s.setAdditiveLengthScaleFactor(val);
        break;

    default:
        Q_ASSERT(0 && "Wrong property");
        break;
    }
}

QString Model::getNBEGSPropName(CNBEGSettingsProps prop)
{
    switch(prop)
    {
    case NBEGSProp_gpuCoreRating:
        return QString("gpuCoreRating");

    case NBEGSProp_resourceBaudRate:
        return QString("resourceBaudRate");

    case NBEGSProp_resourceCostPerSec:
        return QString("resourceCostPerSec");

    case NBEGSProp_linkBaudRate:
        return QString("linkBaudRate");

    case NBEGSProp_limitationsDivider:
        return QString("limitationsDivider");

    case NBEGSProp_smallTPBPenaltyWeight:
        return QString("smallTPBPenaltyWeight");

    case NBEGSProp_largeTPBPenaltyWeight:
        return QString("largeTPBPenaltyWeight");

    case NBEGSProp_multiplicativeLengthScaleFactor:
        return QString("multiplicativeLengthScaleFactor");

    case NBEGSProp_additiveLengthScaleFactor:
        return QString("additiveLengthScaleFactor");
    }

    return QString();
}

Model::CNBEGSettingsProps Model::getNextPropToVary(CNBEGSettingsProps prop)
{
    switch(prop)
    {
    case NBEGSProp_None:
        return NBEGSProp_gpuCoreRating;

    case NBEGSProp_gpuCoreRating:
        return NBEGSProp_resourceBaudRate;

    case NBEGSProp_resourceBaudRate:
        return NBEGSProp_resourceCostPerSec;

    case NBEGSProp_resourceCostPerSec:
        return NBEGSProp_linkBaudRate;

    case NBEGSProp_linkBaudRate:
        return NBEGSProp_limitationsDivider;    
    
    case NBEGSProp_limitationsDivider:
        return NBEGSProp_smallTPBPenaltyWeight;

    case NBEGSProp_smallTPBPenaltyWeight:
        return NBEGSProp_largeTPBPenaltyWeight;

    case NBEGSProp_largeTPBPenaltyWeight:
        return NBEGSProp_multiplicativeLengthScaleFactor;

    case NBEGSProp_multiplicativeLengthScaleFactor:
        return NBEGSProp_additiveLengthScaleFactor;

    case NBEGSProp_additiveLengthScaleFactor:
        return NBEGSProp_None;
    }

    return NBEGSProp_None;
}