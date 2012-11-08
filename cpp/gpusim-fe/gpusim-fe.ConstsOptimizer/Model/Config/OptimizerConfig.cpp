#include "OptimizerConfig.h"
#include "MMEGSettingsHelpers.h"

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

static bool parseMode(QTextStream &in, COptimizerConfig::COptimizationMode &mode)
{
    QString line = in.readLine();
    bool nextLineIsEmpty = (in.readLine().isEmpty());

    if (!nextLineIsEmpty)
        return false;

    if (line == "R")
    {
        mode = COptimizerConfig::OM_Recursive;
        return true;
    }

    if (line != "S")
        return false;
        
    mode = COptimizerConfig::OM_Sequential;
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
    : m_mode(OM_Sequential),
    m_cpuMachinePECountS(0), m_cpuMachinePECountE(0), m_cpuMachinePECountI(0), m_cpuMachinePECountM(IM_Additive),
    m_cpuMachinePERatingS(0), m_cpuMachinePERatingE(0), m_cpuMachinePERatingI(0), m_cpuMachinePERatingM(IM_Additive),
    m_gpuMachinePECountS(0), m_gpuMachinePECountE(0), m_gpuMachinePECountI(0), m_gpuMachinePECountM(IM_Additive),
    m_gpuMachinePERatingS(0), m_gpuMachinePERatingE(0), m_gpuMachinePERatingI(0), m_gpuMachinePERatingM(IM_Additive),
    m_resourceBaudRateS(0.0f), m_resourceBaudRateE(0.0f), m_resourceBaudRateI(0.0f), m_resourceBaudRateM(IM_Additive),
    m_resourceCostPerSecS(0.0f), m_resourceCostPerSecE(0.0f), m_resourceCostPerSecI(0.0f), m_resourceCostPerSecM(IM_Additive),
    m_linkBaudRateS(0.0f), m_linkBaudRateE(0.0f), m_linkBaudRateI(0.0f), m_linkBaudRateM(IM_Additive),
    m_loadOperationCostS(0.0f), m_loadOperationCostE(0.0f), m_loadOperationCostI(0.0f), m_loadOperationCostM(IM_Additive),
    m_saveOperationCostS(0.0f), m_saveOperationCostE(0.0f), m_saveOperationCostI(0.0f), m_saveOperationCostM(IM_Additive)
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

    if (!parseMode(in, cfg.m_mode))
        return false;

    // Fill uint properties info
    //
    static QVector<CUInt32PropInfo> uintPropsInfo;
    uintPropsInfo.push_back(CUInt32PropInfo(getMMEGSPropName(MMEGSProp_cpuMachinePECount),
        &cfg.m_cpuMachinePECountS, &cfg.m_cpuMachinePECountE, &cfg.m_cpuMachinePECountI, &cfg.m_cpuMachinePECountM));
    uintPropsInfo.push_back(CUInt32PropInfo(getMMEGSPropName(MMEGSProp_cpuMachinePERating),
        &cfg.m_cpuMachinePERatingS, &cfg.m_cpuMachinePERatingE, &cfg.m_cpuMachinePERatingI, &cfg.m_cpuMachinePERatingM));
    uintPropsInfo.push_back(CUInt32PropInfo(getMMEGSPropName(MMEGSProp_gpuMachinePECount),
        &cfg.m_gpuMachinePECountS, &cfg.m_gpuMachinePECountE, &cfg.m_gpuMachinePECountI, &cfg.m_gpuMachinePECountM));
    uintPropsInfo.push_back(CUInt32PropInfo(getMMEGSPropName(MMEGSProp_gpuMachinePERating),
        &cfg.m_gpuMachinePERatingS, &cfg.m_gpuMachinePERatingE, &cfg.m_gpuMachinePERatingI, &cfg.m_gpuMachinePERatingM));

    // Fill double properties info
    //
    static QVector<CDoublePropInfo> doublePropsInfo;
    doublePropsInfo.push_back(CDoublePropInfo(getMMEGSPropName(MMEGSProp_resourceBaudRate),
        &cfg.m_resourceBaudRateS, &cfg.m_resourceBaudRateE, &cfg.m_resourceBaudRateI, &cfg.m_resourceBaudRateM));
    doublePropsInfo.push_back(CDoublePropInfo(getMMEGSPropName(MMEGSProp_resourceCostPerSec),
        &cfg.m_resourceCostPerSecS, &cfg.m_resourceCostPerSecE, &cfg.m_resourceCostPerSecI, &cfg.m_resourceCostPerSecM));
    doublePropsInfo.push_back(CDoublePropInfo(getMMEGSPropName(MMEGSProp_linkBaudRate),
        &cfg.m_linkBaudRateS, &cfg.m_linkBaudRateE, &cfg.m_linkBaudRateI, &cfg.m_linkBaudRateM));
    doublePropsInfo.push_back(CDoublePropInfo(getMMEGSPropName(MMEGSProp_loadOperationCost),
        &cfg.m_loadOperationCostS, &cfg.m_loadOperationCostE, &cfg.m_loadOperationCostI, &cfg.m_loadOperationCostM));
    doublePropsInfo.push_back(CDoublePropInfo(getMMEGSPropName(MMEGSProp_saveOperationCost),
        &cfg.m_saveOperationCostS, &cfg.m_saveOperationCostE, &cfg.m_saveOperationCostI, &cfg.m_saveOperationCostM));

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