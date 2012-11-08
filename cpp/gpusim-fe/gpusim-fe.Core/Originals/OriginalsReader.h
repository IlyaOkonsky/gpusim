#pragma once

#include "Original.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT COriginalsReader
    {
    public:
        static const quint32 c_defaultMinMatrixSize /*= 0*/;
        static const quint32 c_defaultMaxMatrixSize /*= 0*/;
        static const quint32 c_defaultminMatrixDistance /*= 16*/;

    public:
        // Zero max matrix size refers to no limitations for matrix size.
        COriginalsReader(COriginalsList &originals, quint32 minMatrixSize = c_defaultMinMatrixSize,
            quint32 maxMatrixSize = c_defaultMaxMatrixSize,
            quint32 minMatrixDistance = c_defaultminMatrixDistance);

        bool readOriginals(const QString &filePath);

    private:
        bool processLine(const QString &line, COriginal &o);

    private:
        COriginalsList &m_originals;
        quint32 m_minMatrixSize;
        quint32 m_maxMatrixSize;
        quint32 m_minMatrixDistance;

    private:
        static const QChar   c_columnSplitter;
        static const quint32 c_columnsCount;
    };
}