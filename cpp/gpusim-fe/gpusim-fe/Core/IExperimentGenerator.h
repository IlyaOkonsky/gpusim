#pragma once

#include "Experiment.h"

namespace Core
{
    class IExperimentGenerator
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