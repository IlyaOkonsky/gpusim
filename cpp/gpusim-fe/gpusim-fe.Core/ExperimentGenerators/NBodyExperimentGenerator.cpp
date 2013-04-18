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
    quint32 minN, quint32 maxN, quint32 nIncrement, quint32 minThreadsPerBlock, quint32 maxThreadsPerBlock,
    quint32 threadsPerBlockIncrement)
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

    Q_ASSERT(!((minN != maxN) && (minThreadsPerBlock != maxThreadsPerBlock)));

    if (m_minN == m_maxN)
        m_nIncrement = 0;

    if (m_minThreadsPerBlock == m_maxThreadsPerBlock)
        m_threadsPerBlockIncrement = 0;
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
            double xAsisValue = m_nIncrement ? currentN : currentThreadsPerBlock;
            sims.append(createSim(simNumber, currentN, currentThreadsPerBlock, xAsisValue));
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

CExperiment CNBodyExperimentGenerator::generateFromOriginals(const COriginalsList& originals, bool nIsAsisValue)
{
    QString expDirName = c_expDirNameFormat.arg(QString::number(0), QString::number(0),
        QString::number(0), QString::number(0), QString::number(0),
        QString::number(0));
    QString expName = c_expNameFormat.arg(QString::number(0), QString::number(0),
        QString::number(0), QString::number(0), QString::number(0),
        QString::number(0));

    // Generate simulations
    //
    CSimulationsList sims;
    quint32 simNumber = 0;
    for (auto I = originals.constBegin(), E = originals.constEnd(); I != E; ++I, ++simNumber)
    {
        double xAsisValue = nIsAsisValue? I->getN() : I->getThreadsPerBlock();
        sims.push_back(createSim(simNumber, I->getN(), I->getThreadsPerBlock(), xAsisValue));
    }

    // Create and return experiment
    return CExperiment(expDirName, expName, sims);
}

#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Tools
CSimulation CNBodyExperimentGenerator::createSim(quint32 simNumber, quint32 n, quint32 threadsPerBlock,
    double xAsisValue)
{
    QString simName = c_simNameFormat.arg(QString::number(simNumber), QString::number(n),
        QString::number(threadsPerBlock));
    QString configName = simName + c_configPostfix;
    return CSimulation(simNumber, simName, xAsisValue, createConfig(configName, n, threadsPerBlock));
}

CGridSimConfig CNBodyExperimentGenerator::createConfig(const QString &name, quint32 n, quint32 threadsPerBlock)
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

CGridSimResourcesConfig CNBodyExperimentGenerator::createResources(quint32 threadsPerBlock)
{
    // Machines list: only one machine with only one PE
    //
    CGridSimMachinesConfig machines;
    machines.append(CGridSimMachineConfig(0, 1, m_settings.getGPUCoreRating()));

    //
    // Resources list: N (up 1024) resources
    //
    CGridSimResourcesConfig resources;
    CGridSimResourceConfig resource;
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

double log2(double x)
{
    return log(x) / log(2.0f);
}

CGridSimGridletConfig CNBodyExperimentGenerator::createGridlet(quint32 n, quint32 threadsPerBlock)
{
    // NOTE: Formulas:
    //   Limitations divider constant is used for decreasing gridlets count via balancing with each gridlet's work
    //   length.
    //   
    //   small_thread_per_blocks_panalty = (n * log2(n)^2 * log2(threadsPerBlock) / threadsPerBlock) * small_tpb_penalty_weight;
    //   large_thread_per_blocks_penalty = threadsPerBlock * n * thread_per_block_penalty_weigth;
    //   length = n * (limitations_divider + large_thread_per_blocks_penalty) * multiplicative_length_scale_factor +
    //       + additive_length_scale_factor;
    //   in_size = 1;
    //   out_size = 1;
    //   count = n / limitations_divider;

    double limitationsDivider              = m_settings.getLimitationsDivider();
    double smallTPBPenaltyWeight           = m_settings.getSmallTPBPenaltyWeight();
    double largeTPBPenaltyWeight           = m_settings.getLargeTPBPenaltyWeight();
    double multiplicativeLengthScaleFactor = m_settings.getMultiplicativeLengthScaleFactor();
    double additiveLengthScaleFactor       = m_settings.getAdditiveLengthScaleFactor();
    
    double smallTPBPenalty = (smallTPBPenaltyWeight * n * log2(n) * log2(n) * log2(threadsPerBlock)) / (threadsPerBlock);
    double largeTPBPenalty = threadsPerBlock * n * largeTPBPenaltyWeight;

    double length = (n * limitationsDivider + smallTPBPenalty + largeTPBPenalty) * multiplicativeLengthScaleFactor +
        additiveLengthScaleFactor;
    double count = n / limitationsDivider;
    quint64 inputSize = 1;
    quint64 outputSize = 1;

    return CGridSimGridletConfig(0, length, inputSize, outputSize, count);
}
#pragma endregion