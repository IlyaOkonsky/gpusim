#include "NBodyExperimentGenerator.h"

#include <qmath.h>
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
    do
    {
        quint64 currentThreadsPerBlock = m_minThreadsPerBlock;
        do 
        {
            sims.append(createSim(simNumber, currentN, currentThreadsPerBlock));
            ++simNumber;

            if (!m_threadsPerBlockIncrement)
                break;
            currentThreadsPerBlock *= m_threadsPerBlockIncrement;

        } while (currentThreadsPerBlock <= m_maxThreadsPerBlock);

        if (!m_nIncrement)
            break;
        currentN *= m_nIncrement;

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
    quint64 blocksCount = qMin(n / threadsPerBlock, quint64(32768));
    return CSimulation(simNumber, simName, createConfig(configName, n, threadsPerBlock));
}

CGridSimConfig CNBodyExperimentGenerator::createConfig(const QString &name, quint64 n, quint64 threadsPerBlock)
{
    auto resources = createResources(threadsPerBlock);

    // Gridlets list
    //
    CGridSimGridletsConfig gridltes;
    gridltes.append(createGridlet(n, threadsPerBlock));

    // And finally config
    //
    return CGridSimConfig(name, m_settings.getLinkBaudRate(), resources, gridltes);
}

CGridSimResourcesConfig CNBodyExperimentGenerator::createResources(quint64 threadsPerBlock)
{
    // Machines list (Only one machines with PECount = threadsPerBlock;)
    //
    CGridSimMachinesConfig machines;
    machines.append(CGridSimMachineConfig(0, 1, 1000));

    //
    // Resources list with n / threadsPerBlock (max 1024 ) resources
    //
    CGridSimResourcesConfig resources;
    CGridSimResourceConfig resource;
    //resource.setName(CGridSimResourceConfig::buildName(i));
    resource.setArch(m_settings.getResourceArch());
    resource.setOS(m_settings.getResourceOS());
    resource.setBaudRate(m_settings.getResourceBaudRate());
    resource.setCostPerSec(m_settings.getResourceCostPerSec());
    resource.setAllocPolicy(0);
    resource.setMachines(machines);
    resource.setCount(threadsPerBlock);
    resources.append(resource);

    return resources;
}


CGridSimGridletConfig CNBodyExperimentGenerator::createGridlet(quint64 n, quint64 threadsPerBlock)
{
    // NOTE: Formulas:
    //   length = length_const;
    //   in_size = in_size_const;
    //   out_size = out_size_const;
    //   count = n^2;

//    quint64 modifiedN = n / blocksCount;
    
    double r = double(threadsPerBlock) * log(double(n));
    if (n >= 512 * 1024)
        r *= threadsPerBlock;

    double r2 = n / threadsPerBlock;
    if (threadsPerBlock >= 256)
        r2 = 0;
//quint64 r = threadsPerBlock * n * log(n); 
    
    double l = (n * n) / (threadsPerBlock * threadsPerBlock);
    double length = l + r - r2;

//     if (threadsPerBlock <= 32)
//     {
//         length = n * n / qPow(2, threadsPerBlock );
//     }

    // !!! double length = log(double(n * n)) / log(double(threadsPerBlock * threadsPerBlock)) + qRound64(r);
    quint64 inputSize = 1;
    quint64 outputSize = 1;
    //quint64 modifiedN = n;
    //quint64 count = modifiedN * modifiedN;
    quint64 count = threadsPerBlock;


    return CGridSimGridletConfig(0, length, inputSize, outputSize, count);
}
#pragma endregion