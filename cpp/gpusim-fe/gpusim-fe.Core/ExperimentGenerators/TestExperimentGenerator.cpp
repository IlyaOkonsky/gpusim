#include "TestExperimentGenerator.h"

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Private constants
const QString CTestExperimentGenerator::c_genName       = QString("Test Experiment Generator");
const QString CTestExperimentGenerator::c_expDirName    = QString("TestExperiment");
const QString CTestExperimentGenerator::c_expName       = QString("Test Experiment");
const QString CTestExperimentGenerator::c_simNameFormat = QString("Simulation_#%1");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

CTestExperimentGenerator::CTestExperimentGenerator()
    :IExperimentGenerator(c_genName)
{

}

CExperiment Core::CTestExperimentGenerator::generate()
{
    CSimulationsList sims;
    for (int i = 0; i < 5; ++i)
    {
        sims.push_back(CSimulation(i, QString(c_simNameFormat).arg(i), i,
            GridSimConfig::CGridSimConfig::createTestConfig()));
    }

    return CExperiment(c_expName, c_expDirName, sims);
}