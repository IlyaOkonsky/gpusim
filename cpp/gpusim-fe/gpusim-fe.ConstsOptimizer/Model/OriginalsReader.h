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
    };
}