#pragma once

#include "NBEGSettingsHelpers.h"
#include "OptimizerConfig.h"

#include <QVector>

namespace Model
{
    void varyProp(const CNBodyExperimentGeneratorSettings &initial,
        const COptimizerConfig&cfg, CNBEGSettingsProps prop, CNBEGSettingsList &l);
}
