#pragma once

#include "Experiment.h"
#include "Originals/Original.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT IExperimentGenerator
    {
    public:
        QString getName() const;

        virtual CExperiment generate() = 0;

    protected:
        IExperimentGenerator(const QString &name = QString());

    private:
        QString m_name;
    };
}