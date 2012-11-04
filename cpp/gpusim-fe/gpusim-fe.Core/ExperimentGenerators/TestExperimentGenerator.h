#pragma once

#include "../IExperimentGenerator.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CTestExperimentGenerator : public IExperimentGenerator
    {
    public:
        CTestExperimentGenerator();

        virtual CExperiment generate();

    private:
        static const QString c_genName;
        static const QString c_expDirName;
        static const QString c_expName;
        static const QString c_simNameFormat;
    };
}