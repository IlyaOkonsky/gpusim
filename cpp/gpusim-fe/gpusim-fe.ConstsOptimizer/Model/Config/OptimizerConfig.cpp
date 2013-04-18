#include "OptimizerConfig.h"
#include "NBEGSettingsHelpers.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QVector>

using namespace Model;

//////////////////////////////////////////////////////////////////////////

template <class T>
struct CPropInfo
{
    CPropInfo(const QString &name = QString(), T* pStartValue = nullptr, T* pEndValue = nullptr,
        T* pIncrement = nullptr, COptimizerConfig::CIncrementMode *pIM = nullptr)
        : m_name(name), m_pStartValue(pStartValue), m_pEndValue(pEndValue), m_pIncrement(pIncrement), m_pIM(pIM)
    {
    }

    QString m_name;
    T* m_pStartValue;
    T* m_pEndValue;
    T* m_pIncrement;
    COptimizerConfig::CIncrementMode *m_pIM;
};

typedef CPropInfo<QVariant> CVariantPropInfo;
typedef CPropInfo<quint32> CUInt32PropInfo;
typedef CPropInfo<double>  CDoublePropInfo;

//////////////////////////////////////////////////////////////////////////

static bool parseHeader(QTextStream &in)
{
    return (in.readLine() == QString("#gpusim-fe.ConstOptimizer.Config"));
}

static bool parseOriginalsParameters(QTextStream &in, quint32 &originalsMinN,
    quint32 &originalsMaxN, quint32 &originalsThreadsPerBlock)
{
    QString line = in.readLine();
    QStringList l = line.split(QChar(' '), QString::SkipEmptyParts);
    if (l.size() != 3)
        return false;

    bool okMinN, okMaxN, okTPB;
    originalsMinN            = l[0].toUInt(&okMinN);
    originalsMaxN            = l[1].toUInt(&okMaxN);
    originalsThreadsPerBlock = l[2].toUInt(&okTPB);
    return okMinN && okMaxN && okTPB;
}

static bool parseModes(QTextStream &in, COptimizerConfig::COptimizationMode &om,
    COptimizerConfig::CCompareMode &cm)
{
    QString line = in.readLine().toUpper();
    bool nextLineIsEmpty = (in.readLine().isEmpty());

    if (!nextLineIsEmpty)
        return false;

    QStringList l = line.split(QChar(' '), QString::SkipEmptyParts);
    if (l.size() != 2)
        return false;

    if ((l[0] != "R") && (l[0] != "S") && (l[0] != "A") && (l[0] != "R"))
        return false;

    om = (l[0] == "R")? COptimizerConfig::OM_Recursive : COptimizerConfig::OM_Sequential;
    cm = (l[1] == "A")? COptimizerConfig::CM_AbsoluteDifference: COptimizerConfig::CM_RelativeError;
    return true;
}

static bool parseProperty(QTextStream &in, CVariantPropInfo &info)
{
    QString line = in.readLine();
    if (!(line.startsWith('#') && line.contains(info.m_name)))
        return false;

    line = in.readLine();
    QStringList l = line.split(QChar(' '), QString::SkipEmptyParts);
    if (l.size() != 4)
        return false;

    QString sIncrementMode = l[3].toUpper();
    if (sIncrementMode == QLatin1String("A"))
        *info.m_pIM = COptimizerConfig::IM_Additive;
    else if (sIncrementMode == QLatin1String("M"))
        *info.m_pIM = COptimizerConfig::IM_Multiplicative;
    else
        return false;

    *info.m_pStartValue = l[0];
    *info.m_pEndValue   = l[1];
    *info.m_pIncrement  = l[2];

    // Check next line
    //
    return (in.readLine().isEmpty());
}

static bool parseProperty(QTextStream &in, CDoublePropInfo &info)
{
    QVariant vStartValue, vEndValue, vIncrement;
    CVariantPropInfo vInfo(info.m_name, &vStartValue, &vEndValue, &vIncrement, info.m_pIM);

    if (!parseProperty(in, vInfo))
        return false;

    bool okStartValue, okEndValue, okIncrement;
    *info.m_pStartValue = vStartValue.toDouble(&okStartValue);
    *info.m_pEndValue   = vEndValue.toDouble(&okEndValue);
    *info.m_pIncrement  = vIncrement.toDouble(&okIncrement);

    // Check and correct increment value
    //
    if (*info.m_pIncrement < 0.0f)
        *info.m_pIncrement = 0.0f;

    return (okStartValue && okEndValue && okIncrement);
}

static bool parseProperty(QTextStream &in, CUInt32PropInfo &info)
{
    QVariant vStartValue, vEndValue, vIncrement;
    CVariantPropInfo vInfo(info.m_name, &vStartValue, &vEndValue, &vIncrement, info.m_pIM);

    if (!parseProperty(in, vInfo))
        return false;

    bool okStartValue, okEndValue, okIncrement;
    *info.m_pStartValue = vStartValue.toUInt(&okStartValue);
    *info.m_pEndValue   = vEndValue.toUInt(&okEndValue);

    // Check and correct increment value
    //
    qint32 increment = vIncrement.toInt(&okIncrement);
    if (increment < 0)
        increment = 0;
    *info.m_pIncrement = increment;

    return (okStartValue && okEndValue && okIncrement);
}

//////////////////////////////////////////////////////////////////////////

COptimizerConfig::COptimizerConfig()
    : m_om(OM_Sequential), m_cm(CM_AbsoluteDifference),
    m_originalsMinN(0), m_originalsMaxN(0), m_originalsThreadsPerBlock(0),
    m_gpuCoreRatingS(0), m_gpuCoreRatingE(0), m_gpuCoreRatingI(0), m_gpuCoreRatingM(IM_Additive),
    m_resourceBaudRateS(0.0f), m_resourceBaudRateE(0.0f), m_resourceBaudRateI(0.0f), m_resourceBaudRateM(IM_Additive),
    m_resourceCostPerSecS(0.0f), m_resourceCostPerSecE(0.0f), m_resourceCostPerSecI(0.0f), m_resourceCostPerSecM(IM_Additive),
    m_linkBaudRateS(0.0f), m_linkBaudRateE(0.0f), m_linkBaudRateI(0.0f), m_linkBaudRateM(IM_Additive),
    m_limitationsDividerS(0.0f), m_limitationsDividerE(0.0f), m_limitationsDividerI(0.0f), m_limitationsDividerM(IM_Additive),
    m_smallTPBPenaltyWeightS(0.0f), m_smallTPBPenaltyWeightE(0.0f), m_smallTPBPenaltyWeightI(0.0f), m_smallTPBPenaltyWeightM(IM_Additive),
    m_largeTPBPenaltyWeightS(0.0f), m_largeTPBPenaltyWeightE(0.0f), m_largeTPBPenaltyWeightI(0.0f), m_largeTPBPenaltyWeightM(IM_Additive),
    m_multiplicativeLengthScaleFactorS(0.0f), m_multiplicativeLengthScaleFactorE(0.0f), m_multiplicativeLengthScaleFactorI(0.0f), m_multiplicativeLengthScaleFactorM(IM_Additive),
    m_additiveLengthScaleFactorS(0.0f), m_additiveLengthScaleFactorE(0.0f), m_additiveLengthScaleFactorI(0.0f), m_additiveLengthScaleFactorM(IM_Additive)
{

}

bool COptimizerConfig::readFromFile(const QString &filePath, COptimizerConfig &config)
{
    COptimizerConfig cfg;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QTextStream in(&file);
    if (!parseHeader(in))
        return false;

    if (!parseOriginalsParameters(in, cfg.m_originalsMinN, cfg.m_originalsMaxN, cfg.m_originalsThreadsPerBlock))
        return false;

    if (!parseModes(in, cfg.m_om, cfg.m_cm))
        return false;

    // Fill uint properties info
    //
    static QVector<CUInt32PropInfo> uintPropsInfo;
    uintPropsInfo.push_back(CUInt32PropInfo(getNBEGSPropName(NBEGSProp_gpuCoreRating),
        &cfg.m_gpuCoreRatingS, &cfg.m_gpuCoreRatingE, &cfg.m_gpuCoreRatingI, &cfg.m_gpuCoreRatingM));

    // Fill double properties info
    //
    static QVector<CDoublePropInfo> doublePropsInfo;
    doublePropsInfo.push_back(CDoublePropInfo(getNBEGSPropName(NBEGSProp_resourceBaudRate),
        &cfg.m_resourceBaudRateS, &cfg.m_resourceBaudRateE, &cfg.m_resourceBaudRateI, &cfg.m_resourceBaudRateM));
    doublePropsInfo.push_back(CDoublePropInfo(getNBEGSPropName(NBEGSProp_resourceCostPerSec),
        &cfg.m_resourceCostPerSecS, &cfg.m_resourceCostPerSecE, &cfg.m_resourceCostPerSecI, &cfg.m_resourceCostPerSecM));
    doublePropsInfo.push_back(CDoublePropInfo(getNBEGSPropName(NBEGSProp_linkBaudRate),
        &cfg.m_linkBaudRateS, &cfg.m_linkBaudRateE, &cfg.m_linkBaudRateI, &cfg.m_linkBaudRateM));

    doublePropsInfo.push_back(CDoublePropInfo(getNBEGSPropName(NBEGSProp_limitationsDivider),
        &cfg.m_limitationsDividerS, &cfg.m_limitationsDividerE, &cfg.m_limitationsDividerI, &cfg.m_limitationsDividerM));
    doublePropsInfo.push_back(CDoublePropInfo(getNBEGSPropName(NBEGSProp_smallTPBPenaltyWeight),
        &cfg.m_smallTPBPenaltyWeightS, &cfg.m_smallTPBPenaltyWeightE, &cfg.m_smallTPBPenaltyWeightI, &cfg.m_smallTPBPenaltyWeightM));
    doublePropsInfo.push_back(CDoublePropInfo(getNBEGSPropName(NBEGSProp_largeTPBPenaltyWeight),
        &cfg.m_largeTPBPenaltyWeightS, &cfg.m_largeTPBPenaltyWeightE, &cfg.m_largeTPBPenaltyWeightI, &cfg.m_largeTPBPenaltyWeightM));
    doublePropsInfo.push_back(CDoublePropInfo(getNBEGSPropName(NBEGSProp_multiplicativeLengthScaleFactor),
        &cfg.m_multiplicativeLengthScaleFactorS, &cfg.m_multiplicativeLengthScaleFactorE, &cfg.m_multiplicativeLengthScaleFactorI, &cfg.m_multiplicativeLengthScaleFactorM));
    doublePropsInfo.push_back(CDoublePropInfo(getNBEGSPropName(NBEGSProp_additiveLengthScaleFactor),
        &cfg.m_additiveLengthScaleFactorS, &cfg.m_additiveLengthScaleFactorE, &cfg.m_additiveLengthScaleFactorI, &cfg.m_additiveLengthScaleFactorM));

    // Read all uint properties
    //
    for (auto I = uintPropsInfo.begin(), E = uintPropsInfo.end(); I != E; ++I)
    {
        if (!parseProperty(in, *I))
            return false;
    }

    // Read all double properties
    //
    for (auto I = doublePropsInfo.begin(), E = doublePropsInfo.end(); I != E; ++I)
    {
        if (!parseProperty(in, *I))
            return false;
    }

    config = cfg;
    return true;
}