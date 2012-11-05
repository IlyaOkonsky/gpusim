#include "VaryHelper.h"

using namespace Model;

//////////////////////////////////////////////////////////////////////////

template <class T>
void varyMMEGProperty(const CMatrixMultiplyExperimentGeneratorSettings &initial,
    CMMEGSettingsProps prop, T startValue, T endValue, T increment, T noVaryIncrementValue, CMMEGSettingsList &l)
{
    // If no variations for current property is avaliable - the only one, initial value is the result of variation.
    // 
    if (increment == noVaryIncrementValue)
    {
        l.push_back(initial);
        return;
    }

    for (T val = startValue; val < endValue; val += increment)
    {
        CMatrixMultiplyExperimentGeneratorSettings s(initial);
        setMMEGSProp(prop, s, val);
        l.push_back(s);
    }
}

//////////////////////////////////////////////////////////////////////////

void Model::varyProp(const CMatrixMultiplyExperimentGeneratorSettings &initial, const COptimizerConfig&cfg,
    CMMEGSettingsProps prop, CMMEGSettingsList &l)
{
    switch(prop)
    {
    case MMEGSProp_cpuMachinePECount:
        varyMMEGProperty(initial, prop,
            cfg.m_cpuMachinePECountS, cfg.m_cpuMachinePECountE, cfg.m_cpuMachinePECountI, quint32(0), l);
        break;

    case MMEGSProp_cpuMachinePERating:
        varyMMEGProperty(initial, prop,
            cfg.m_cpuMachinePERatingS, cfg.m_cpuMachinePERatingE, cfg.m_cpuMachinePERatingI, quint32(0), l);
        break;

    case MMEGSProp_gpuMachinePECount:
        varyMMEGProperty(initial, prop,
            cfg.m_gpuMachinePECountS, cfg.m_gpuMachinePECountE, cfg.m_gpuMachinePECountI, quint32(0), l);
        break;

    case MMEGSProp_gpuMachinePERating:
        varyMMEGProperty(initial, prop,
            cfg.m_gpuMachinePERatingS, cfg.m_gpuMachinePERatingE, cfg.m_gpuMachinePERatingI, quint32(0), l);
        break;

    case MMEGSProp_resourceBaudRate:
        varyMMEGProperty(initial, prop,
            cfg.m_resourceBaudRateS, cfg.m_resourceBaudRateE, cfg.m_resourceBaudRateI, double(0.0f), l);
        break;

    case MMEGSProp_resourceCostPerSec:
        varyMMEGProperty(initial, prop,
            cfg.m_resourceCostPerSecS, cfg.m_resourceCostPerSecE, cfg.m_resourceCostPerSecI, double(0.0f), l);
        break;

    case MMEGSProp_linkBaudRate:
        varyMMEGProperty(initial, prop,
            cfg.m_linkBaudRateS, cfg.m_linkBaudRateE, cfg.m_linkBaudRateI, double(0.0f), l);
        break;

    case MMEGSProp_loadOperationCost:
        varyMMEGProperty(initial, prop,
            cfg.m_loadOperationCostS, cfg.m_loadOperationCostE, cfg.m_loadOperationCostI, double(0.0f), l);
        break;

    case MMEGSProp_saveOperationCost:
        varyMMEGProperty(initial, prop,
            cfg.m_saveOperationCostS, cfg.m_saveOperationCostE, cfg.m_saveOperationCostI, double(0.0f), l);
        break;

    default:
        Q_ASSERT(0 && "Wrong property");
    }
}