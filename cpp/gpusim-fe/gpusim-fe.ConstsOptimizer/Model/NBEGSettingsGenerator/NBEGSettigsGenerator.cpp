#include "NBEGSettigsGenerator.h"
#include "../Config/VaryHelper.h"

#include <QQueue>
#include <QStack>

using namespace Model;

//////////////////////////////////////////////////////////////////////////

CNBEGSettingsList CNBEGSettingsGenerator::generate(const COptimizerConfig&cfg)
{
    CNBEGSettingsList l;
    CNBodyExperimentGeneratorSettings initial = createInitialSettings(cfg);
    if (cfg.m_om == COptimizerConfig::OM_Sequential)
        l = genSequentual(cfg, initial);
    else
        l = genRecursive(cfg, initial);

    std::sort(l.begin(), l.end());
    l.erase(std::unique(l.begin(), l.end()), l.end());
    return l;
}

//////////////////////////////////////////////////////////////////////////

CNBodyExperimentGeneratorSettings CNBEGSettingsGenerator::createInitialSettings(
    const COptimizerConfig&cfg)
{
    CNBodyExperimentGeneratorSettings s;

    double m_resourceBaudRate;
    double m_resourceCostPerSec;
    double m_linkBaudRate;

    double m_loadOperationCost;
    double m_saveOperationCost;

    s.setGPUCoreRating(cfg.m_gpuCoreRatingS);
    s.setResourceArch("gpusim-fe.Consts Optimizer Arch");
    s.setResourceOS("gpusim-fe.Consts Optimizer OS");
    s.setResourceBaudRate(cfg.m_resourceBaudRateS);
    s.setResourceCostPerSec(cfg.m_resourceCostPerSecS);
    s.setLinkBaudRate(cfg.m_linkBaudRateS);
    s.setLimitationsDivider(cfg.m_limitationsDividerS);
    s.setSmallTPBPenaltyWeight(cfg.m_smallTPBPenaltyWeightS);
    s.setLargeTPBPenaltyWeight(cfg.m_largeTPBPenaltyWeightS);
    s.setMultiplicativeLengthScaleFactor(cfg.m_multiplicativeLengthScaleFactorS);
    s.setAdditiveLengthScaleFactor(cfg.m_additiveLengthScaleFactorS);

    return s;
}

CNBEGSettingsList CNBEGSettingsGenerator::genSequentual(const COptimizerConfig&cfg,
    const CNBodyExperimentGeneratorSettings &initial)
{
    CNBEGSettingsList l;

    // Vary sequentually all parameters
    //
    varyProp(initial, cfg, NBEGSProp_gpuCoreRating,  l);
    varyProp(initial, cfg, NBEGSProp_limitationsDivider, l);
    varyProp(initial, cfg, NBEGSProp_smallTPBPenaltyWeight,  l);
    varyProp(initial, cfg, NBEGSProp_largeTPBPenaltyWeight, l);
    varyProp(initial, cfg, NBEGSProp_resourceBaudRate,   l);
    varyProp(initial, cfg, NBEGSProp_resourceCostPerSec, l);
    varyProp(initial, cfg, NBEGSProp_linkBaudRate,       l);
    varyProp(initial, cfg, NBEGSProp_multiplicativeLengthScaleFactor,  l);
    varyProp(initial, cfg, NBEGSProp_additiveLengthScaleFactor,  l);

    return l;
}

//////////////////////////////////////////////////////////////////////////

struct CNBEGSStackElem
{
    CNBEGSStackElem(
        const CNBodyExperimentGeneratorSettings &settings = CNBodyExperimentGeneratorSettings(),
        CNBEGSettingsProps prop = NBEGSProp_None)
        : m_settings(settings), m_prop(prop)
    {
    }

    CNBodyExperimentGeneratorSettings m_settings;
    CNBEGSettingsProps m_prop;
};

CNBEGSettingsList CNBEGSettingsGenerator::genRecursive(const COptimizerConfig&cfg,
    const CNBodyExperimentGeneratorSettings &initial)
{
    CNBEGSettingsList l;

    QStack<CNBEGSStackElem> stack;
    stack.push(CNBEGSStackElem(initial, NBEGSProp_gpuCoreRating));

    while(!stack.isEmpty())
    {
        CNBEGSStackElem e = stack.pop();
        if (e.m_prop == NBEGSProp_None)
        {
            l.push_back(e.m_settings);
            continue;
        }

        // Vary by current prop and push to stack with next prop value
        //
        CNBEGSettingsList variated;
        varyProp(e.m_settings, cfg, e.m_prop, variated);
        std::for_each(variated.constBegin(), variated.constEnd(),
            [&stack, &e] (const CNBodyExperimentGeneratorSettings &s) {
            stack.push_back(CNBEGSStackElem(s, getNextPropToVary(e.m_prop)));
        });
    }

    return l;
}