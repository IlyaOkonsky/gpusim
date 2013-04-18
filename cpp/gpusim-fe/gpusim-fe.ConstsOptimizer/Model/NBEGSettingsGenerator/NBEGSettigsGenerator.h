#pragma once

#include "../Config/NBEGSettingsHelpers.h"
#include "../Config/OptimizerConfig.h"

namespace Model
{
    class CNBEGSettingsGenerator
    {
    public:
        static CNBEGSettingsList generate(const COptimizerConfig&cfg);

    private:
        static CNBodyExperimentGeneratorSettings createInitialSettings(const COptimizerConfig&cfg);

        static CNBEGSettingsList genSequentual(const COptimizerConfig&cfg,
            const CNBodyExperimentGeneratorSettings &initial);
        static CNBEGSettingsList genRecursive(const COptimizerConfig&cfg,
            const CNBodyExperimentGeneratorSettings &initial);

    private:
        CNBEGSettingsGenerator() { }
    };
}