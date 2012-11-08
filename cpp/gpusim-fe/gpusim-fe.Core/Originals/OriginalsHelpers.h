#pragma once

#include "Original.h"
#include "../Experiment.h"

namespace Core
{
    // Returns difference between originals and experiment results.
    // In case of error returns -1.0f;
    // 
    double GPUSIM_FE_CORE_EXPORT calculateDifference(const COriginalsList &originals,
        const Core::CExperiment &experiment, double *pAverangeError = nullptr);
}