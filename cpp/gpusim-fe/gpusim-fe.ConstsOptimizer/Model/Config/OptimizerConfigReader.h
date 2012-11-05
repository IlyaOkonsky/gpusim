#pragma once

#include "OptimizerConfig.h"

namespace Model
{
    class COptimizerConfigReader
    {
    public:
        static bool readConfig(COptimizerConfig &config);

    private:
        COptimizerConfigReader() {}
    };
}