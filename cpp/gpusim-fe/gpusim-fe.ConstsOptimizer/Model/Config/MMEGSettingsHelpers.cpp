#include "MMEGSettingsHelpers.h"

void Model::setMMEGSProp(CMMEGSettingsProps prop, CMatrixMultiplyExperimentGeneratorSettings &s, quint32 val)
{
    switch(prop)
    {
    case MMEGSProp_cpuMachinePECount:
        s.setCPUMachinePECount(val);
        break;

    case MMEGSProp_cpuMachinePERating:
        s.setCPUMachinePERating(val);
        break;

    case MMEGSProp_gpuMachinePECount:
        s.setGPUMachinePECount(val);
        break;

    case MMEGSProp_gpuMachinePERating:
        s.setGPUMachinePERating(val);
        break;

    default:
        Q_ASSERT(0 && "Wrong property");
        break;
    }
}

void Model::setMMEGSProp(CMMEGSettingsProps prop, CMatrixMultiplyExperimentGeneratorSettings &s, double val)
{
    switch(prop)
    {
    case MMEGSProp_resourceBaudRate:
        s.setResourceBaudRate(val);
        break;

    case MMEGSProp_resourceCostPerSec:
        s.setResourceCostPerSec(val);
        break;

    case MMEGSProp_linkBaudRate:
        s.setLinkBaudRate(val);
        break;

    case MMEGSProp_loadOperationCost:
        s.setLoadOperationCost(val);
        break;

    case MMEGSProp_saveOperationCost:
        s.setSaveOperationCost(val);
        break;

    default:
        Q_ASSERT(0 && "Wrong property");
        break;
    }
}

QString Model::getMMEGSPropName(CMMEGSettingsProps prop)
{
    switch(prop)
    {
    case MMEGSProp_cpuMachinePECount:
        return QString("cpuMachinePECount");

    case MMEGSProp_cpuMachinePERating:
        return QString("cpuMachinePERating");

    case MMEGSProp_gpuMachinePECount:
        return QString("gpuMachinePECount");

    case MMEGSProp_gpuMachinePERating:
        return QString("gpuMachinePERating");

    case MMEGSProp_resourceBaudRate:
        return QString("resourceBaudRate");

    case MMEGSProp_resourceCostPerSec:
        return QString("resourceCostPerSec");

    case MMEGSProp_linkBaudRate:
        return QString("linkBaudRate");

    case MMEGSProp_loadOperationCost:
        return QString("loadOperationCost");

    case MMEGSProp_saveOperationCost:
        return QString("saveOperationCost");
    }

    return QString();
}

Model::CMMEGSettingsProps Model::getNextPropToVary(CMMEGSettingsProps prop)
{
    switch(prop)
    {
    case MMEGSProp_None:
        return MMEGSProp_cpuMachinePECount;

    case MMEGSProp_cpuMachinePECount:
        return MMEGSProp_cpuMachinePERating;

    case MMEGSProp_cpuMachinePERating:
        return MMEGSProp_gpuMachinePECount;

    case MMEGSProp_gpuMachinePECount:
        return MMEGSProp_gpuMachinePERating;

    case MMEGSProp_gpuMachinePERating:
        return MMEGSProp_resourceBaudRate;

    case MMEGSProp_resourceBaudRate:
        return MMEGSProp_resourceCostPerSec;

    case MMEGSProp_resourceCostPerSec:
        return MMEGSProp_linkBaudRate;

    case MMEGSProp_linkBaudRate:
        return MMEGSProp_loadOperationCost;

    case MMEGSProp_loadOperationCost:
        return MMEGSProp_saveOperationCost;

    case MMEGSProp_saveOperationCost:
        return MMEGSProp_None;
    }

    return MMEGSProp_None;
}