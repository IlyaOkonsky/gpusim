#include "MatrixMultiplyExperimentGenerator.h"

using namespace Core;
using namespace Core::GridSimConfig;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Private constants
const QString CMatrixMultiplyExperimentGenerator::c_genName           = QString("Matrix multiply Experiment Generator");
const QString CMatrixMultiplyExperimentGenerator::c_expDirNameFormat  = QString("MME_#%1_[%2; %3]_Inc_%4");
const QString CMatrixMultiplyExperimentGenerator::c_expNameFormat     = QString("Matrix Multiply Experiment (Size %1, Block size [%2; %3], Increment %4)");
const QString CMatrixMultiplyExperimentGenerator::c_simNameFormat     = QString("Sim #%1(BlockSize %2)");
const QString CMatrixMultiplyExperimentGenerator::c_configPostfix     = QString(".config");

const quint32 CMatrixMultiplyExperimentGenerator::c_cpuMachinePECount  = 8;
const quint32 CMatrixMultiplyExperimentGenerator::c_cpuMachinePERating = 100;
const quint32 CMatrixMultiplyExperimentGenerator::c_gpuMachinePECount  = 128;
const quint32 CMatrixMultiplyExperimentGenerator::c_gpuMachinePERating = 20;

const QString CMatrixMultiplyExperimentGenerator::c_resourceArch       = QString("Matrix Multiply Experiment Arch");
const QString CMatrixMultiplyExperimentGenerator::c_resourceOS         = QString("Matrix Multiply Experiment OS");
const double  CMatrixMultiplyExperimentGenerator::c_resourceBaudRate   = 10000000000.0f; // (10 Gbps)
const double  CMatrixMultiplyExperimentGenerator::c_resourceCostPerSec = 1.0f; // default cost?

const double  CMatrixMultiplyExperimentGenerator::c_loadOperationCost  = 10.0f;
const double  CMatrixMultiplyExperimentGenerator::c_saveOperationCost  = 100.0f;
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor
CMatrixMultiplyExperimentGenerator::CMatrixMultiplyExperimentGenerator(quint64 matrixSize, quint64 minBlockSize,
    quint64 maxBlockSize, quint64 blockSizeIncrement /*= 1*/)
    : IExperimentGenerator("Matrix multiply Experiment Generator"), m_matrixSize(matrixSize),
    m_minBlockSize(minBlockSize), m_maxBlockSize(maxBlockSize), m_blockSizeIncrement(blockSizeIncrement)
{
    Q_ASSERT(m_minBlockSize >= 2);
    Q_ASSERT(m_maxBlockSize <= m_matrixSize / 2);
    Q_ASSERT(m_minBlockSize <= maxBlockSize);
    Q_ASSERT(blockSizeIncrement > 0);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Experiment generation
CExperiment CMatrixMultiplyExperimentGenerator::generate()
{
    // Short large matrix size with add k postfix
    //
    QString sMatrixSize;
    if (m_matrixSize <= 1000)
        sMatrixSize = QString::number(m_matrixSize);
    else
        sMatrixSize = QString("%1k").arg(QString::number(m_matrixSize));

    // Build experiment name and directory name
    //
    QString expDirName = c_expDirNameFormat.arg(sMatrixSize, QString::number(m_minBlockSize),
        QString::number(m_maxBlockSize), QString::number(m_blockSizeIncrement));
    QString expName = c_expNameFormat.arg(sMatrixSize, QString::number(m_minBlockSize), QString::number(m_maxBlockSize),
        QString::number(m_blockSizeIncrement));

    // Generate simulations
    //
    CSimulationsList sims;
    quint32 simNumber = 0;
    for (quint64 currentBlockSize = m_minBlockSize; currentBlockSize <= m_maxBlockSize;
        currentBlockSize += m_blockSizeIncrement)
    {
        sims.append(createSim(simNumber, m_matrixSize, currentBlockSize));
        ++simNumber;
    }
    
    // Create and return experiment
    return CExperiment(expDirName, expName, sims);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Tools
CSimulation CMatrixMultiplyExperimentGenerator::createSim(quint32 simNumber, quint64 matrixSize, quint64 blockSize)
{
    QString simName = c_simNameFormat.arg(QString::number(simNumber), QString::number(blockSize));
    QString configName = simName + c_configPostfix;
    return CSimulation(simNumber, simName, createConfig(configName, matrixSize, blockSize));
}

CGridSimConfig CMatrixMultiplyExperimentGenerator::createConfig(const QString &name, quint64 matrixSize,
    quint64 blockSize)
{
    // Machines list (2 machines: one for CPU and one for GPU)
    //
    CGridSimMachinesConfig machines;
    machines.append(CGridSimMachineConfig(0, c_cpuMachinePECount, c_cpuMachinePERating));
    machines.append(CGridSimMachineConfig(1, c_gpuMachinePECount, c_gpuMachinePERating));

    //
    // Resources list with only one resource
    //
    CGridSimResourceConfig resource;
    resource.setArch(c_resourceArch);
    resource.setOS(c_resourceOS);
    resource.setBaudRate(c_resourceBaudRate);
    resource.setCostPerSec(c_resourceCostPerSec);
    resource.setMachines(machines);

    CGridSimResourcesConfig resources;
    resources.append(resource);

    // Gridlets list
    //
    CGridSimGridletsConfig gridltes;
    quint32 gridletsCount = matrixSize / blockSize;

    for (quint32 i = 0; i < gridletsCount; ++i)
        gridltes.append(createGridlet(i, matrixSize, blockSize));

    // And finally config
    //
    return CGridSimConfig(name, resources, gridltes);
}

CGridSimGridletConfig CMatrixMultiplyExperimentGenerator::createGridlet(quint32 id, quint64 matrixSize,
    quint64 blockSize)
{
    // NOTE: Formulas:
    //   length = block_size * matrix_size^2 * save + 2 * matrix_size^3 * load;
    //   in_size = 3 * block_size;
    //   out_size = block_size;

    double fMatrixSize = matrixSize;
    double fBlockSize = blockSize;
    double length = fBlockSize * pow(fMatrixSize, 2) * c_saveOperationCost + 
        2 * pow(fMatrixSize, 3) * c_loadOperationCost;

    quint64 inputSize = 3 * blockSize;
    quint64 outputSize = blockSize;

    return CGridSimGridletConfig(id, length, inputSize, outputSize);
}
#pragma endregion