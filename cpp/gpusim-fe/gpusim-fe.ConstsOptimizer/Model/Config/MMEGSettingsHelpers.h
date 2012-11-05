#pragma once

#include "../gpusim-fe.Core/ExperimentGenerators/MatrixMultiplyExperimentGeneratorSettings.h"
#include <QVector>

namespace Model
{
    using Core::Settings::CMatrixMultiplyExperimentGeneratorSettings;
    typedef QVector<CMatrixMultiplyExperimentGeneratorSettings> CMMEGSettingsList;

    typedef enum _tagCMMEGSettingsProps
    {
        MMEGSProp_None,
        MMEGSProp_cpuMachinePECount,
        MMEGSProp_cpuMachinePERating,
        MMEGSProp_gpuMachinePECount,
        MMEGSProp_gpuMachinePERating,
        MMEGSProp_resourceBaudRate,
        MMEGSProp_resourceCostPerSec,
        MMEGSProp_linkBaudRate,
        MMEGSProp_loadOperationCost,
        MMEGSProp_saveOperationCost
    } CMMEGSettingsProps;

    void setMMEGSProp(CMMEGSettingsProps prop, CMatrixMultiplyExperimentGeneratorSettings &s, quint32 val);
    void setMMEGSProp(CMMEGSettingsProps prop, CMatrixMultiplyExperimentGeneratorSettings &s, double val);
    QString getMMEGSPropName(CMMEGSettingsProps prop);
    CMMEGSettingsProps getNextPropToVary(CMMEGSettingsProps prop);
}
