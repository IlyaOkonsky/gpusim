#include "Optimizer.h"

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
    qLog_DebugMsg() << "!!!" << m_originalsFilePath;
    qApp->quit();
}
