#pragma once

#include "OptimizerConfig.h"
#include <QString>
#include <QTextStream>

namespace Model
{
    // TODO: Refactor optimizer configuration part. This code is very raw and evaluative.
    // NOTE: Members postfix description:
    //       - S postfix is for Start value
    //       - E postfix is for End value
    //       - I postfix is for value Increment
    //       - M postfix is for value increment Mode (A - Additive or M - Multiplicative)
    //
    struct COptimizerConfig
    {
        typedef enum _tagCOptimizationMode
        {
            OM_Recursive,
            OM_Sequential
        } COptimizationMode;

        typedef enum _tagCCompareMode
        {
            CM_AbsoluteDifference,
            CM_RelativeError
        } CCompareMode;

        typedef enum _tagCIncrementMode
        {
            IM_Additive,
            IM_Multiplicative
        } CIncrementMode;

        COptimizerConfig();

        // File format is next:
        // <Header>
        // <Originals parameters: in one line min matrix size, max matrix size and min distance between sizes
        // (increment. Splitter is space)>
        // <Mode (R - recursive or S - sequential) and Compare mode (A - absolute distance or R - relative error)>
        // <Line break - \r\n>
        // <Properties>
        // For each property there must be 3 lines:
        // First for property name;
        // Second for start and end values, for increment value and increment mode(A symbol for additive and
        // M symbol for multiplicative). Splitter is space.
        // Third line must be empty (\r\n)
        // 
        // Properties names
        // #cpuMachinePECount
        // #cpuMachinePERating
        // #gpuMachinePECount
        // #gpuMachinePERating
        // #resourceBaudRate
        // #resourceCostPerSec
        // #linkBaudRate
        // #loadOperationCost
        // #saveOperationCost
        // 
        // If increment value is 0 or negative - corresponding variable shall not be variated.
        //
        static bool readFromFile(const QString &filePath, COptimizerConfig &config);

        COptimizationMode m_om;
        CCompareMode m_cm;

        quint32        m_originalsMinMatrixSize;
        quint32        m_originalsMaxMatrixSize;
        quint32        m_originalsMinSizeIncrement;

        quint32        m_cpuMachinePECountS;
        quint32        m_cpuMachinePECountE;
        quint32        m_cpuMachinePECountI;
        CIncrementMode m_cpuMachinePECountM;

        quint32        m_cpuMachinePERatingS;
        quint32        m_cpuMachinePERatingE;
        quint32        m_cpuMachinePERatingI;
        CIncrementMode m_cpuMachinePERatingM;

        quint32        m_gpuMachinePECountS;
        quint32        m_gpuMachinePECountE;
        quint32        m_gpuMachinePECountI;
        CIncrementMode m_gpuMachinePECountM;

        quint32        m_gpuMachinePERatingS;
        quint32        m_gpuMachinePERatingE;
        quint32        m_gpuMachinePERatingI;
        CIncrementMode m_gpuMachinePERatingM;

        double         m_resourceBaudRateS;
        double         m_resourceBaudRateE;
        double         m_resourceBaudRateI;
        CIncrementMode m_resourceBaudRateM;

        double         m_resourceCostPerSecS;
        double         m_resourceCostPerSecE;
        double         m_resourceCostPerSecI;
        CIncrementMode m_resourceCostPerSecM;
        
        double         m_linkBaudRateS;
        double         m_linkBaudRateE;
        double         m_linkBaudRateI;
        CIncrementMode m_linkBaudRateM;

        double         m_loadOperationCostS;
        double         m_loadOperationCostE;
        double         m_loadOperationCostI;
        CIncrementMode m_loadOperationCostM;
        
        double         m_saveOperationCostS;
        double         m_saveOperationCostE;
        double         m_saveOperationCostI;
        CIncrementMode m_saveOperationCostM;

    private:

    };
}