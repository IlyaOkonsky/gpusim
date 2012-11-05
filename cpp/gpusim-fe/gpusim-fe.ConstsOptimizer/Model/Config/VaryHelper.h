#pragma once

#include "MMEGSettingsHelpers.h"
#include "OptimizerConfig.h"

#include <QVector>

namespace Model
{
    void varyProp(const CMatrixMultiplyExperimentGeneratorSettings &initial,
        const COptimizerConfig&cfg, CMMEGSettingsProps prop, CMMEGSettingsList &l);
}
