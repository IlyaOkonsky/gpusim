#include "OriginalsReader.h"

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


COriginalsReader::COriginalsReader(COriginalsList &originals)
    : m_originals(originals)
{

}

bool COriginalsReader::readOriginals(const QString &filePath)
{
    return false;
}