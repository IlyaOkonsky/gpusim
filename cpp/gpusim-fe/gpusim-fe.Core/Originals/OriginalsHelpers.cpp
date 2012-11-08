#include "OriginalsHelpers.h"

#include "../../QLogger/QLog"

#include <QMath.h>

using namespace Core;

double Core::calculateDifference(const COriginalsList &originals, const Core::CExperiment &experiment,
    double *pAverangeError /*= nullptr*/)
{
    if (experiment.getSimulationsRef().size() != originals.size())
    {
        qLog_CriticalMsg() << "Originals and simulations count in experiment are no equal.";
        return -1.0f;
    }

    double diff = 0.0f;
    double error = 0.0f;
    auto itOriginals = originals.constBegin();
    auto itOriginalsEnd = originals.constEnd();
    auto itSimulation = experiment.getSimulationsRef().constBegin();
    for(; itOriginals != itOriginalsEnd; ++itOriginals, ++itSimulation)
    {
        double distance = qAbs(itOriginals->getSimulationTime() -
            itSimulation->getOutputRef().getTotalSimulationTime());
        double e = (distance *100)/ itOriginals->getSimulationTime();
        error += e;
        diff += qPow(distance, 2.0f);

        qLog_DebugMsg() << "Distance: " << distance << "; Error = " << e << "%.";
    }

    error /= (double)originals.size();
    if (pAverangeError)
        *pAverangeError = error;

    return diff;
}