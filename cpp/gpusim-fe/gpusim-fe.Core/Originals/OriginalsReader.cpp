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
const quint32 COriginalsReader::c_defaultMinMatrixSize     = 0;
const quint32 COriginalsReader::c_defaultMaxMatrixSize     = 0;
const quint32 COriginalsReader::c_defaultminMatrixDistance = 0;
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const QChar   COriginalsReader::c_columnSplitter  = QChar(';');
const quint32 COriginalsReader::c_columnsCount    = 4;
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

COriginalsReader::COriginalsReader(COriginalsList &originals,
    quint32 minMatrixSize /*= c_defaulMinMatrixSize*/,
    quint32 maxMatrixSize /*= c_defaulMaxMatrixSize*/,
    quint32 minMatrixDistance /*= c_defaultminMatrixDistance*/)
    : m_originals(originals), m_minMatrixSize(minMatrixSize), m_maxMatrixSize(maxMatrixSize),
    m_minMatrixDistance(minMatrixDistance)
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

        // 2.1) Check for new matrix size
        //
        if (currentOriginal.getMatrixSize() != o.getMatrixSize())
        {
            quint32 currentMatrixSize = currentOriginal.getMatrixSize();
            quint32 lastMatrixSize = m_originals.isEmpty() ? 0 : m_originals.back().getMatrixSize();
            quint32 currentDistance = currentMatrixSize - lastMatrixSize;

            if ((currentMatrixSize != 0) && (currentMatrixSize >= m_minMatrixSize))
            {
                if ((!lastMatrixSize) || (currentDistance >= m_minMatrixDistance))
                    m_originals.push_back(currentOriginal);
            }

            if (m_maxMatrixSize && (currentMatrixSize >= m_maxMatrixSize))
                break;

            currentOriginal = o;
            continue;
        }
        
        // 2.2) Check for min of simulation time
        //
        if (currentOriginal.getSimulationTime() > o.getSimulationTime())
            currentOriginal.setSimulationTime(o.getSimulationTime());
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

    o.setMatrixSize(l[0].toUInt());
    o.setSimulationTime(l[3].toDouble());
    return true;
}