#pragma once

#include "Original.h"

namespace Model
{
    class COriginalsReader
    {
    public:
        COriginalsReader(COriginalsList &originals);

        bool readOriginals(const QString &filePath);

    private:
        COriginalsList &m_originals;

    private:
        bool processLine(const QString &line, COriginal &o);

    private:
        static const quint32 c_maxMatrixSize;
        static const QChar   c_columnSplitter;
        static const quint32 c_columnsCount;
    };
}