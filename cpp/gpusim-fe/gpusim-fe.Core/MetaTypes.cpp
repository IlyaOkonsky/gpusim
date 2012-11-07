#include "MetaTypes.h"

#include "Experiment.h"
#include "Enums.h"
#include "Originals/Original.h"

#include <QProcess>

void Core::registerMetaTypes()
{
    qRegisterMetaType<GridSimConfig::CGridSimGridletConfig>("Core::GridSimConfig::CGridSimGridletConfig");
    qRegisterMetaType<GridSimConfig::CGridSimGridletsConfig>("Core::GridSimConfig::CGridSimGridletsConfig");

    qRegisterMetaType<GridSimConfig::CGridSimGridletOutput>("Core::GridSimConfig::CGridSimGridletOutput");
    qRegisterMetaType<GridSimConfig::CGridSimGridletsOutput>("Core::GridSimConfig::CGridSimGridletsOutput");

    qRegisterMetaType<GridSimConfig::CGridSimMachineConfig>("Core::GridSimConfig::CGridSimMachineConfig");
    qRegisterMetaType<GridSimConfig::CGridSimMachinesConfig>("Core::GridSimConfig::CGridSimMachinesConfig");

    qRegisterMetaType<GridSimConfig::CGridSimResourceConfig>("Core::GridSimConfig::CGridSimResourceConfig");
    qRegisterMetaType<GridSimConfig::CGridSimResourcesConfig>("Core::GridSimConfig::CGridSimResourcesConfig");

    qRegisterMetaType<GridSimConfig::CGridSimConfig>("Core::GridSimConfig::CGridSimConfig");
    qRegisterMetaType<GridSimConfig::CGridSimOutput>("Core::GridSimConfig::CGridSimOutput");

    qRegisterMetaType<CSimulation>("Core::CSimulation");
    qRegisterMetaType<CSimulationsList>("Core::CSimulationsList");

    qRegisterMetaType<CExperiment>("Core::CExperiment");

    qRegisterMetaType<CExitCode>("Core::CExitCode");

    qRegisterMetaType<COriginal>("Core::COriginal");
    qRegisterMetaType<COriginalsList>("Core::COriginalsList");

    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
}