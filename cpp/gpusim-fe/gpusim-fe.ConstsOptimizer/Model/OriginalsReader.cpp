#include "OriginalsReader.h"

#include "../../QLogger/QLog"

#include <QFile>
#include <QTextStream>
#include <QStringList>

using namespace Model;

//////////////////////////////////////////////////////////////////////////
// Constants
////////////////////////////////////////////////////////////////////////// 
#pragma region Public constants
#pragma endregion

//////////////////////////////////////////////////////////////////////////

#pragma region Private constants
const quint32 COriginalsReader::c_maxMatrixSize   = 1024;
const QChar   COriginalsReader::c_columnSplitter  = QChar(';');
const quint32 COriginalsReader::c_columnsCount    = 4;
#pragma endregion

//////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////// 

COriginalsReader::COriginalsReader(COriginalsList &originals)
    : m_originals(originals)
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
        QDoubleVector times;
        quint32 matrixSize;
        
        QString line = ts.readLine();
        COriginal o;
        if (!processLine(line, o))
            return false;

        // 2.1) Check for new matrix size
        //
        if (currentOriginal.getMatrixSize() != o.getMatrixSize())
        {
            if (currentOriginal.getMatrixSize() != 0)
                m_originals.push_back(currentOriginal);

            if (currentOriginal.getMatrixSize() >= c_maxMatrixSize)
                break;

            currentOriginal = o;
            continue;
        }
        
        // 2.2) Check for min of simulation time
        //
        if (currentOriginal.getSimulationTime() > o.getSimulationTime())
            currentOriginal.setSimulationTime(o.getSimulationTime());
    }
    
    return true;
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
        qLog_WarningMsg() << c_columnsCount << " divided by " << c_columnSplitter << "Expectec";
        return false;
    }

    o.setMatrixSize(l[0].toUInt());
    o.setSimulationTime(l[3].toDouble());
    return true;
}