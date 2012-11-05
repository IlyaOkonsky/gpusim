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
    //
    struct COptimizerConfig
    {
        typedef enum _tagCOptimizationMode
        {
            OM_Recursive,
            OM_Sequential
        } COptimizationMode;

        COptimizerConfig();

        // File format is next:
        // <Header>
        // <Mode (recursive or not)>
        // <Properties>
        // For each property there must be 3 lines:
        // First for property name;
        // Second for start and end values and for increment value (splitter is space)
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

        COptimizationMode m_mode;

        quint32 m_cpuMachinePECountS;
        quint32 m_cpuMachinePECountE;
        quint32 m_cpuMachinePECountI;

        quint32 m_cpuMachinePERatingS;
        quint32 m_cpuMachinePERatingE;
        quint32 m_cpuMachinePERatingI;

        quint32 m_gpuMachinePECountS;
        quint32 m_gpuMachinePECountE;
        quint32 m_gpuMachinePECountI;

        quint32 m_gpuMachinePERatingS;
        quint32 m_gpuMachinePERatingE;
        quint32 m_gpuMachinePERatingI;

        double m_resourceBaudRateS;
        double m_resourceBaudRateE;
        double m_resourceBaudRateI;

        double m_resourceCostPerSecS;
        double m_resourceCostPerSecE;
        double m_resourceCostPerSecI;
        
        double m_linkBaudRateS;
        double m_linkBaudRateE;
        double m_linkBaudRateI;

        double m_loadOperationCostS;
        double m_loadOperationCostE;
        double m_loadOperationCostI;
        
        double m_saveOperationCostS;
        double m_saveOperationCostE;
        double m_saveOperationCostI;

    private:

    };
}