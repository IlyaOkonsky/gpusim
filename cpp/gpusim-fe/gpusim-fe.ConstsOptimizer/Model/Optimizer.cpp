#include "Optimizer.h"
#include "OriginalsReader.h"

#include "../../QLogger/QLog"

#include <QCoreApplication>

using namespace Model;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

COptimizer::COptimizer(const QString &originalsFilePath, const QString &configFilePath, const QString &outputFilePath,
    QObject *pParent /*= nullptr*/)
    :QObject(pParent), m_originalsFilePath(originalsFilePath), m_configFilePath(configFilePath),
    m_outputFilePath(outputFilePath)
{

}

void COptimizer::optimize()
{
    if (!readOriginals())
    {
        qLog_CriticalMsg() << "Failed to read originals.";
        qApp->quit();
        return;
    }

    qApp->quit();
}

//////////////////////////////////////////////////////////////////////////

bool COptimizer::logAndReturn(bool res)
{
    qLog_DebugMsg() << "..." << res;
    return res;
}

//////////////////////////////////////////////////////////////////////////

bool COptimizer::readOriginals()
{
    qLog_DebugMsg() << "Reading originals from " << m_originalsFilePath << " file...";
    COriginalsReader reader(m_originals);
    bool res = reader.readOriginals(m_originalsFilePath);
    qLog_DebugMsg() << "..." << res;
    return res;
}