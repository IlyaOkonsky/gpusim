#include "MMEGSettigsGenerator.h"
#include "../Config/VaryHelper.h"

#include <QQueue>
#include <QStack>

using namespace Model;

//////////////////////////////////////////////////////////////////////////

CMMEGSettingsList CMMEGSettingsGenerator::generate(const COptimizerConfig&cfg)
{
    CMMEGSettingsList l;
    CMatrixMultiplyExperimentGeneratorSettings initial = createInitialSettings(cfg);
    if (cfg.m_om == COptimizerConfig::OM_Sequential)
        l = genSequentual(cfg, initial);
    else
        l = genRecursive(cfg, initial);

    std::sort(l.begin(), l.end());
    l.erase(std::unique(l.begin(), l.end()), l.end());
    return l;
}

//////////////////////////////////////////////////////////////////////////

CMatrixMultiplyExperimentGeneratorSettings CMMEGSettingsGenerator::createInitialSettings(
    const COptimizerConfig&cfg)
{
    CMatrixMultiplyExperimentGeneratorSettings s;

    double m_resourceBaudRate;
    double m_resourceCostPerSec;
    double m_linkBaudRate;

    double m_loadOperationCost;
    double m_saveOperationCost;

    s.setCPUMachinePECount(cfg.m_cpuMachinePECountS);
    s.setCPUMachinePERating(cfg.m_cpuMachinePERatingS);
    s.setGPUMachinePECount(cfg.m_gpuMachinePECountS);
    s.setGPUMachinePERating(cfg.m_gpuMachinePERatingS);
    s.setResourceArch("gpusim-fe.Consts Optimizer Arch");
    s.setResourceOS("gpusim-fe.Consts Optimizer OS");
    s.setResourceBaudRate(cfg.m_resourceBaudRateS);
    s.setResourceCostPerSec(cfg.m_resourceCostPerSecS);
    s.setLinkBaudRate(cfg.m_linkBaudRateS);
    s.setLoadOperationCost(cfg.m_loadOperationCostS);
    s.setSaveOperationCost(cfg.m_saveOperationCostS);

    return s;
}

CMMEGSettingsList CMMEGSettingsGenerator::genSequentual(const COptimizerConfig&cfg,
    const CMatrixMultiplyExperimentGeneratorSettings &initial)
{
    CMMEGSettingsList l;

    // Vary sequentually all parameters
    //
    varyProp(initial, cfg, MMEGSProp_cpuMachinePECount,  l);
    varyProp(initial, cfg, MMEGSProp_cpuMachinePERating, l);
    varyProp(initial, cfg, MMEGSProp_gpuMachinePECount,  l);
    varyProp(initial, cfg, MMEGSProp_gpuMachinePERating, l);
    varyProp(initial, cfg, MMEGSProp_resourceBaudRate,   l);
    varyProp(initial, cfg, MMEGSProp_resourceCostPerSec, l);
    varyProp(initial, cfg, MMEGSProp_linkBaudRate,       l);
    varyProp(initial, cfg, MMEGSProp_loadOperationCost,  l);
    varyProp(initial, cfg, MMEGSProp_saveOperationCost,  l);

    return l;
}

//////////////////////////////////////////////////////////////////////////

struct CMMEGSStackElem
{
    CMMEGSStackElem(
        const CMatrixMultiplyExperimentGeneratorSettings &settings = CMatrixMultiplyExperimentGeneratorSettings(),
        CMMEGSettingsProps prop = MMEGSProp_None)
        : m_settings(settings), m_prop(prop)
    {
    }

    CMatrixMultiplyExperimentGeneratorSettings m_settings;
    CMMEGSettingsProps m_prop;
};

CMMEGSettingsList CMMEGSettingsGenerator::genRecursive(const COptimizerConfig&cfg,
    const CMatrixMultiplyExperimentGeneratorSettings &initial)
{
    CMMEGSettingsList l;

    QStack<CMMEGSStackElem> stack;
    stack.push(CMMEGSStackElem(initial, MMEGSProp_cpuMachinePECount));

    while(!stack.isEmpty())
    {
        CMMEGSStackElem e = stack.pop();
        if (e.m_prop == MMEGSProp_None)
        {
            l.push_back(e.m_settings);
            continue;
        }

        // Vary by current prop and push to stack with next prop value
        //
        CMMEGSettingsList variated;
        varyProp(e.m_settings, cfg, e.m_prop, variated);
        std::for_each(variated.constBegin(), variated.constEnd(),
            [&stack, &e] (const CMatrixMultiplyExperimentGeneratorSettings &s) {
            stack.push_back(CMMEGSStackElem(s, getNextPropToVary(e.m_prop)));
        });
    }

    return l;
}