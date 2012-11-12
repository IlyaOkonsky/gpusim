#include "MatrixMultiplyExperimentGenerator.h"

using namespace Core;
using namespace Core::GridSimConfig;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Private constants
const QString CMatrixMultiplyExperimentGenerator::c_genName           = QString("Matrix multiply Experiment Generator");
const QString CMatrixMultiplyExperimentGenerator::c_expDirNameFormat  = QString("MME_[%1; %2]_Inc_%3_Block_%4");
const QString CMatrixMultiplyExperimentGenerator::c_expNameFormat     = QString("Matrix Multiply Experiment (Size [%1; %2], Increment %3, Block size %4)");
const QString CMatrixMultiplyExperimentGenerator::c_simNameFormat     = QString("Sim #%1(Size %2)");
const QString CMatrixMultiplyExperimentGenerator::c_configPostfix     = QString(".config");
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

#pragma region Constructor
CMatrixMultiplyExperimentGenerator::CMatrixMultiplyExperimentGenerator(
    const Settings::CMatrixMultiplyExperimentGeneratorSettings &settings,
    quint64 minMatrixSize, quint64 maxMatrixSize, quint64 matrixSizeIncrement, quint64 blockSize)
    : IExperimentGenerator(c_genName), m_settings(settings), m_minMatrixSize(minMatrixSize),
    m_maxMatrixSize(maxMatrixSize), m_matrixSizeIncrement(matrixSizeIncrement), m_blockSize(blockSize)

{
    Q_ASSERT(m_minMatrixSize >= 2);
    Q_ASSERT(m_maxMatrixSize <= 5000);
    Q_ASSERT(m_minMatrixSize <= m_maxMatrixSize);
    Q_ASSERT(m_matrixSizeIncrement > 0);
    Q_ASSERT(m_blockSize <= m_maxMatrixSize);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Experiment generation
CExperiment CMatrixMultiplyExperimentGenerator::generate()
{
    // Build experiment name and directory name
    //
    QString expDirName = c_expDirNameFormat.arg(getMatrixSizeString(m_minMatrixSize),
        getMatrixSizeString(m_maxMatrixSize), getMatrixSizeString(m_matrixSizeIncrement), QString::number(m_blockSize));
    QString expName = c_expNameFormat.arg(getMatrixSizeString(m_minMatrixSize),
        getMatrixSizeString(m_maxMatrixSize), getMatrixSizeString(m_matrixSizeIncrement), QString::number(m_blockSize));

    // Generate simulations
    //
    CSimulationsList sims;
    quint32 simNumber = 0;
    for (quint64 currentMatrixSize = m_minMatrixSize; currentMatrixSize <= m_maxMatrixSize;
        currentMatrixSize += m_matrixSizeIncrement)
    {
        sims.append(createSim(simNumber, currentMatrixSize, m_blockSize));
        ++simNumber;
    }
    
    // Create and return experiment
    return CExperiment(expDirName, expName, sims);
}
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Tools
QString CMatrixMultiplyExperimentGenerator::getMatrixSizeString(quint64 matrixSize)
{
    if (matrixSize <= 1000)
        return QString::number(matrixSize);
        
    return QString("%1k").arg(QString::number(matrixSize / 1000));
}


CSimulation CMatrixMultiplyExperimentGenerator::createSim(quint32 simNumber, quint64 matrixSize, quint64 blockSize)
{
    QString simName = c_simNameFormat.arg(QString::number(simNumber), QString::number(matrixSize));
    QString configName = simName + c_configPostfix;
    return CSimulation(simNumber, simName, createConfig(configName, matrixSize, blockSize));
}

CGridSimConfig CMatrixMultiplyExperimentGenerator::createConfig(const QString &name, quint64 matrixSize,
    quint64 blockSize)
{
    // Machines list (2 machines: one for CPU and one for GPU)
    //
    CGridSimMachinesConfig machines;
    machines.append(CGridSimMachineConfig(0, m_settings.getGPUMachinePECount(), m_settings.getGPUMachinePERating()));
    machines.append(CGridSimMachineConfig(1, m_settings.getCPUMachinePECount(), m_settings.getCPUMachinePERating()));

    //
    // Resources list with only one resource
    //
    CGridSimResourceConfig resource;
    resource.setArch(m_settings.getResourceArch());
    resource.setOS(m_settings.getResourceOS());
    resource.setBaudRate(m_settings.getResourceBaudRate());
    resource.setCostPerSec(m_settings.getResourceCostPerSec());
    resource.setMachines(machines);

    CGridSimResourcesConfig resources;
    resources.append(resource);

    // Gridlets list
    //
    CGridSimGridletsConfig gridltes;
    quint32 gridletsCount = matrixSize / blockSize;
    if (!gridletsCount)
    {
        gridletsCount = 1;
        blockSize = matrixSize;
    }

    for (quint32 i = 0; i < gridletsCount; ++i)
        gridltes.append(createGridlet(i, matrixSize, blockSize));

    // And finally config
    //
    return CGridSimConfig(name, m_settings.getLinkBaudRate(), resources, gridltes);
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
    double length = fBlockSize * pow(fMatrixSize, 2) * m_settings.getSaveOperationCost() + 
        2 * pow(fMatrixSize, 3) * m_settings.getLoadOperationCost();

    quint64 inputSize = 3 * blockSize;
    quint64 outputSize = blockSize;

    return CGridSimGridletConfig(id, length, inputSize, outputSize);
}
#pragma endregion