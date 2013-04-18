#pragma once

#include "../gpusim-fe.Core/ExperimentGenerators/NBodyExperimentGeneratorSettings.h"
#include <QVector>
#include <QQueue>

namespace Model
{
    using Core::Settings::CNBodyExperimentGeneratorSettings;
    typedef QVector<CNBodyExperimentGeneratorSettings> CNBEGSettingsList;
    typedef QQueue<CNBodyExperimentGeneratorSettings> CNBEGSettingsQueue;

    typedef enum _tagCNBEGSettingsProps
    {
        NBEGSProp_None,
        NBEGSProp_gpuCoreRating,

        NBEGSProp_resourceBaudRate,
        NBEGSProp_resourceCostPerSec,
        NBEGSProp_linkBaudRate,

        NBEGSProp_limitationsDivider,
        NBEGSProp_smallTPBPenaltyWeight,
        NBEGSProp_largeTPBPenaltyWeight,
        NBEGSProp_multiplicativeLengthScaleFactor,
        NBEGSProp_additiveLengthScaleFactor
    } CNBEGSettingsProps;

    void setNBEGSProp(CNBEGSettingsProps prop, CNBodyExperimentGeneratorSettings &s, quint32 val);
    void setNBEGSProp(CNBEGSettingsProps prop, CNBodyExperimentGeneratorSettings &s, double val);
    QString getNBEGSPropName(CNBEGSettingsProps prop);
    CNBEGSettingsProps getNextPropToVary(CNBEGSettingsProps prop);
}
