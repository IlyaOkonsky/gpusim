#pragma once

#include "Original.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT COriginalsReader
    {
    public:
        static const quint32 c_defaulMaxMatrixSize /*= 0*/;

    public:
        // Zero max matrix size refers to no limitations for matrix size.
        COriginalsReader(COriginalsList &originals, quint32 maxMatrixSize = c_defaulMaxMatrixSize);

        bool readOriginals(const QString &filePath);

    private:
        bool processLine(const QString &line, COriginal &o);

    private:
        COriginalsList &m_originals;
        quint32 m_maxMatrixSize;

    private:
        static const QChar   c_columnSplitter;
        static const quint32 c_columnsCount;
    };
}