#include "NBodyExperimentGenerator.h"

using namespace Core;
using namespace Core::GridSimConfig;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Private constants
const QString CNBodyExperimentGenerator::c_genName           = QString("N-Body Experiment Generator");
const QString CNBodyExperimentGenerator::c_expDirNameFormat  = QString("NBody_[%1; %2; %3]_[%4; %5; %6]");
const QString CNBodyExperimentGenerator::c_expNameFormat     = QString("N-Body Experiment (N [%1; %2; %3], ThreadsPerBlock [%4; %5; %6])");
const QString CNBodyExperimentGenerator::c_simNameFormat     = QString("Sim #%1(N %2, TPB %3)");
const QString CNBodyExperimentGenerator::c_configPostfix     = QString(".config");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor
CNBodyExperimentGenerator::CNBodyExperimentGenerator(const Settings::CNBodyExperimentGeneratorSettings &settings,
    quint64 minN, quint64 maxN, quint64 nIncrement, quint64 minThreadsPerBlock, quint64 maxThreadsPerBlock,
    quint64 threadsPerBlockIncrement)
    : IExperimentGenerator(c_genName), m_settings(settings), m_minN(minN), m_maxN(maxN), m_nIncrement(nIncrement),
    m_minThreadsPerBlock(minThreadsPerBlock), m_maxThreadsPerBlock(maxThreadsPerBlock),
    m_threadsPerBlockIncrement(threadsPerBlockIncrement)
{
    Q_ASSERT(m_minN >= 1024);
    Q_ASSERT(m_maxN <= 1024*1024);
    Q_ASSERT(m_minN <= m_maxN);

    Q_ASSERT(m_minThreadsPerBlock >= 1);
    Q_ASSERT(m_maxThreadsPerBlock <= 1024);
    Q_ASSERT(m_minThreadsPerBlock <= m_maxThreadsPerBlock);

    Q_ASSERT((m_threadsPerBlockIncrement % 2) == 0);
}

#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Experiment generation
CExperiment CNBodyExperimentGenerator::generate()
{
    // Build experiment name and directory name
    //
    QString expDirName = c_expDirNameFormat.arg(QString::number(m_minN), QString::number(m_maxN),
        QString::number(m_nIncrement), QString::number(m_minThreadsPerBlock), QString::number(m_maxThreadsPerBlock),
        QString::number(m_threadsPerBlockIncrement));
    QString expName = c_expNameFormat.arg(QString::number(m_minN), QString::number(m_maxN),
        QString::number(m_nIncrement), QString::number(m_minThreadsPerBlock), QString::number(m_maxThreadsPerBlock),
        QString::number(m_threadsPerBlockIncrement));

    // Generate simulations
    //
    CSimulationsList sims;
    quint32 simNumber = 0;
    
    quint64 currentN = m_minN;
    quint64 currentThreadsPerBlock = m_minThreadsPerBlock;
    do
    {
        do 
        {
            sims.append(createSim(simNumber, currentN, currentThreadsPerBlock));
            ++simNumber;

            if (!m_threadsPerBlockIncrement)
                break;
            currentThreadsPerBlock += m_threadsPerBlockIncrement;

        } while (currentThreadsPerBlock <= m_maxThreadsPerBlock);

        if (!m_nIncrement)
            break;
        currentN += m_nIncrement;

    } while (currentN <= m_maxN);

    // Create and return experiment
    return CExperiment(expDirName, expName, sims);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Tools
CSimulation CNBodyExperimentGenerator::createSim(quint32 simNumber, quint64 n, quint64 threadsPerBlock)
{
    QString simName = c_simNameFormat.arg(QString::number(simNumber), QString::number(n),
        QString::number(threadsPerBlock));
    QString configName = simName + c_configPostfix;
    return CSimulation(simNumber, simName, createConfig(configName, n, threadsPerBlock));
}

CGridSimConfig CNBodyExperimentGenerator::createConfig(const QString &name, quint64 n, quint64 threadsPerBlock)
{
    // Machines list (Machines count = N / threadsPerBlock; PECount = threadsPerBlock;)
    //
    CGridSimMachinesConfig machines;

    quint32 machinesCount = n / threadsPerBlock;
    for (quint32 i = 0; i < machinesCount; ++i)
        machines.append(CGridSimMachineConfig(i, threadsPerBlock, 1000));

    //
    // Resources list with only one resource
    //
    CGridSimResourceConfig resource;
    resource.setArch(m_settings.getResourceArch());
    resource.setOS(m_settings.getResourceOS());
    resource.setBaudRate(m_settings.getResourceBaudRate());
    resource.setCostPerSec(m_settings.getResourceCostPerSec());
    resource.setAllocPolicy(1);
    resource.setMachines(machines);

    CGridSimResourcesConfig resources;
    resources.append(resource);

    // Gridlets list
    //
    CGridSimGridletsConfig gridltes;
    gridltes.append(createGridlet(n));

    // And finally config
    //
    return CGridSimConfig(name, m_settings.getLinkBaudRate(), resources, gridltes);
}

CGridSimGridletConfig CNBodyExperimentGenerator::createGridlet(quint64 n)
{
    // NOTE: Formulas:
    //   length = length_const;
    //   in_size = in_size_const;
    //   out_size = out_size_const;
    //   count = n^2;

    double length = 5;
    quint64 inputSize = 20;
    quint64 outputSize = 12;
    quint64 modifiedN = n / 200;
    quint64 count = modifiedN * modifiedN;

    return CGridSimGridletConfig(0, length, inputSize, outputSize, count);
}
#pragma endregion