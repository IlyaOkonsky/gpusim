#include "VaryHelper.h"

using namespace Model;

//////////////////////////////////////////////////////////////////////////

template <class T>
void varyNBEGProperty(const CNBodyExperimentGeneratorSettings &initial,
    CNBEGSettingsProps prop, T startValue, T endValue, T increment, COptimizerConfig::CIncrementMode im,
    T noVaryIncrementValue, CNBEGSettingsList &l)
{
    // If no variations for current property is available - the only one, initial value is the result of variation.
    // 
    if (increment == noVaryIncrementValue)
    {
        l.push_back(initial);
        return;
    }

    for (T val = startValue; val <= endValue; )
    {
        CNBodyExperimentGeneratorSettings s(initial);
        setNBEGSProp(prop, s, val);
        l.push_back(s);

        if (im == COptimizerConfig::IM_Additive)
            val += increment;
        else
            val *= increment;
    }
}

//////////////////////////////////////////////////////////////////////////

void Model::varyProp(const CNBodyExperimentGeneratorSettings &initial, const COptimizerConfig&cfg,
    CNBEGSettingsProps prop, CNBEGSettingsList &l)
{
    switch(prop)
    {
    case NBEGSProp_gpuCoreRating:
        varyNBEGProperty(initial, prop,
            cfg.m_gpuCoreRatingS, cfg.m_gpuCoreRatingE, cfg.m_gpuCoreRatingI, cfg.m_gpuCoreRatingM,
            quint32(0), l);
        break;

    case NBEGSProp_resourceBaudRate:
        varyNBEGProperty(initial, prop,
            cfg.m_resourceBaudRateS, cfg.m_resourceBaudRateE, cfg.m_resourceBaudRateI, cfg.m_resourceBaudRateM,
            double(0.0f), l);
        break;

    case NBEGSProp_resourceCostPerSec:
        varyNBEGProperty(initial, prop,
            cfg.m_resourceCostPerSecS, cfg.m_resourceCostPerSecE, cfg.m_resourceCostPerSecI, cfg.m_resourceCostPerSecM,
            double(0.0f), l);
        break;

    case NBEGSProp_linkBaudRate:
        varyNBEGProperty(initial, prop,
            cfg.m_linkBaudRateS, cfg.m_linkBaudRateE, cfg.m_linkBaudRateI, cfg.m_linkBaudRateM,
            double(0.0f), l);
        break;

    case NBEGSProp_limitationsDivider:
        varyNBEGProperty(initial, prop,
            cfg.m_limitationsDividerS, cfg.m_limitationsDividerE, cfg.m_limitationsDividerI, cfg.m_limitationsDividerM,
            double(0.0f), l);
        break;

    case NBEGSProp_smallTPBPenaltyWeight:
        varyNBEGProperty(initial, prop,
            cfg.m_smallTPBPenaltyWeightS, cfg.m_smallTPBPenaltyWeightE, cfg.m_smallTPBPenaltyWeightI, cfg.m_smallTPBPenaltyWeightM,
            double(0.0f), l);
        break;

    case NBEGSProp_largeTPBPenaltyWeight:
        varyNBEGProperty(initial, prop,
            cfg.m_largeTPBPenaltyWeightS, cfg.m_largeTPBPenaltyWeightE, cfg.m_largeTPBPenaltyWeightI, cfg.m_largeTPBPenaltyWeightM,
            double(0.0f), l);
        break;

    case NBEGSProp_multiplicativeLengthScaleFactor:
        varyNBEGProperty(initial, prop,
            cfg.m_multiplicativeLengthScaleFactorS, cfg.m_multiplicativeLengthScaleFactorE, cfg.m_multiplicativeLengthScaleFactorI, cfg.m_multiplicativeLengthScaleFactorM,
            double(0.0f), l);
        break;

    case NBEGSProp_additiveLengthScaleFactor:
        varyNBEGProperty(initial, prop,
            cfg.m_additiveLengthScaleFactorS, cfg.m_additiveLengthScaleFactorE, cfg.m_additiveLengthScaleFactorI, cfg.m_additiveLengthScaleFactorM,
            double(0.0f), l);
        break;

    default:
        Q_ASSERT(0 && "Wrong property");
    }
}