#pragma once

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
        // <Originals parameters: in one line min N, max N and threadPerBlock value increment. Splitter is space)>
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
        // #gpuCoreRating
        // #resourceBaudRate
        // #resourceCostPerSec
        // #linkBaudRate
        // #limitationsDivider
        // #smallTPBPenaltyWeight
        // #largeTPBPenaltyWeight
        // #multiplicativeLengthScaleFactor
        // #additiveLengthScaleFactor
        // 
        // If increment value is 0 or negative - corresponding variable shall not be variated.
        //
        static bool readFromFile(const QString &filePath, COptimizerConfig &config);

        COptimizationMode m_om;
        CCompareMode m_cm;

        quint32        m_originalsMinN;
        quint32        m_originalsMaxN;
        quint32        m_originalsThreadsPerBlock;

        quint32        m_gpuCoreRatingS;
        quint32        m_gpuCoreRatingE;
        quint32        m_gpuCoreRatingI;
        CIncrementMode m_gpuCoreRatingM;

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

        double         m_limitationsDividerS;
        double         m_limitationsDividerE;
        double         m_limitationsDividerI;
        CIncrementMode m_limitationsDividerM;

        double         m_smallTPBPenaltyWeightS;
        double         m_smallTPBPenaltyWeightE;
        double         m_smallTPBPenaltyWeightI;
        CIncrementMode m_smallTPBPenaltyWeightM;

        double         m_largeTPBPenaltyWeightS;
        double         m_largeTPBPenaltyWeightE;
        double         m_largeTPBPenaltyWeightI;
        CIncrementMode m_largeTPBPenaltyWeightM;

        double         m_multiplicativeLengthScaleFactorS;
        double         m_multiplicativeLengthScaleFactorE;
        double         m_multiplicativeLengthScaleFactorI;
        CIncrementMode m_multiplicativeLengthScaleFactorM;
        
        double         m_additiveLengthScaleFactorS;
        double         m_additiveLengthScaleFactorE;
        double         m_additiveLengthScaleFactorI;
        CIncrementMode m_additiveLengthScaleFactorM;

    private:

    };
}