#include "OriginalsReader.h"

#include "../QLogger/QLog"

#include <QFile>
#include <QTextStream>
#include <QStringList>

using namespace Core;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
const quint32 COriginalsReader::c_defaultMinN            = 0;
const quint32 COriginalsReader::c_defaultMaxN            = 0;
const quint32 COriginalsReader::c_defaultThreadsPerBlock = 0;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QString COriginalsReader::c_columnSplitter = QString("; ");
const quint32 COriginalsReader::c_columnsCount   = 3;
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

Core::COriginalsReader::COriginalsReader(COriginalsList &originals, quint32 minN /*= c_defaultMinN*/,
    quint32 maxN /*= c_defaultMaxN*/, quint32 threadsPerBlock /*= c_defaultThreadsPerBlock*/)
    : m_originals(originals), m_minN(minN), m_maxN(maxN), m_threadsPerBlock(threadsPerBlock)
{

}


bool COriginalsReader::readOriginals(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream ts(&file);

    // 1) Skip first line
    ts.readLine();

    // 2) Read data for matrix size
    //
    COriginal currentOriginal;
    while(!ts.atEnd())
    {
        quint32 matrixSize;
        
        QString line = ts.readLine();
        COriginal o;
        if (!processLine(line, o))
            return false;

        if (o.getN() < m_minN)
            continue;

        if (m_maxN && (o.getN() > m_maxN))
            continue;

        if (m_threadsPerBlock && (o.getThreadsPerBlock() != m_threadsPerBlock))
            continue;

        m_originals.push_back(o);
    }
    
    std::sort(m_originals.begin(), m_originals.end());
    return !m_originals.isEmpty();
}

bool COriginalsReader::processLine(const QString &line, COriginal &o)
{
    if (line.isEmpty())
    {
        qLog_WarningMsg() << "Not empty line expected.";
        return false;
    }

    QStringList l = line.split(c_columnSplitter , QString::SkipEmptyParts);
    if (l.size() != c_columnsCount)
    {
        qLog_WarningMsg() << c_columnsCount << " columns divided by " << c_columnSplitter << " expected";
        return false;
    }

    o.setN(l[0].toUInt());
    o.setThreadsPerBlock(l[1].toUInt());
    o.setSimulationTime(l[2].toDouble() * 1000.0f);
    return true;
}