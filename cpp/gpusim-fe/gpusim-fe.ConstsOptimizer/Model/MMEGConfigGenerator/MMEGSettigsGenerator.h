#pragma once

#include "../Config/MMEGSettingsHelpers.h"
#include "../Config/OptimizerConfig.h"

namespace Model
{
    class CMMEGSettingsGenerator
    {
    public:
        static CMMEGSettingsList generate(const COptimizerConfig&cfg);

    private:
        static CMatrixMultiplyExperimentGeneratorSettings createInitialSettings(const COptimizerConfig&cfg);

        static CMMEGSettingsList genSequentual(const COptimizerConfig&cfg,
            const CMatrixMultiplyExperimentGeneratorSettings &initial);
        static CMMEGSettingsList genRecursive(const COptimizerConfig&cfg,
            const CMatrixMultiplyExperimentGeneratorSettings &initial);

    private:
        CMMEGSettingsGenerator() { }
    };
}