#pragma once

#include "Original.h"

namespace Core
{
    #pragma region Original code
    //class GPUSIM_FE_CORE_EXPORT COriginalsReader
    //{
    //public:
    //    static const quint32 c_defaultMinMatrixSize /*= 0*/;
    //    static const quint32 c_defaultMaxMatrixSize /*= 0*/;
    //    static const quint32 c_defaultminMatrixDistance /*= 16*/;

    //public:
    //    // Zero max matrix size refers to no limitations for matrix size.
    //    COriginalsReader(COriginalsList &originals, quint32 minMatrixSize = c_defaultMinMatrixSize,
    //        quint32 maxMatrixSize = c_defaultMaxMatrixSize,
    //        quint32 minMatrixDistance = c_defaultminMatrixDistance);

    //    bool readOriginals(const QString &filePath);

    //private:
    //    bool processLine(const QString &line, COriginal &o);

    //private:
    //    COriginalsList &m_originals;
    //    quint32 m_minMatrixSize;
    //    quint32 m_maxMatrixSize;
    //    quint32 m_minMatrixDistance;

    //private:
    //    static const QChar   c_columnSplitter;
    //    static const quint32 c_columnsCount;
    //};
    #pragma endregion

    class GPUSIM_FE_CORE_EXPORT COriginalsReader
    {
    public:
        static const quint32 c_defaultMinN /*= 0*/;
        static const quint32 c_defaultMaxN /*= 0*/;
        static const quint32 c_defaultThreadsPerBlock /*= 0*/;

    public:
        // Zero max n refers to no limitations for n.
        // If threds per block value provided (not zero) read all originals from minN to maxN with threadPerBlock
        // equal to passed.
        COriginalsReader(COriginalsList &originals, quint32 minN = c_defaultMinN, quint32 maxN = c_defaultMaxN,
            quint32 threadsPerBlock = c_defaultThreadsPerBlock);

        bool readOriginals(const QString &filePath);

    private:
        bool processLine(const QString &line, COriginal &o);

    private:
        COriginalsList &m_originals;

        quint32 m_minN;
        quint32 m_maxN;
        quint32 m_threadsPerBlock;

    private:
        static const QString c_columnSplitter;
        static const quint32 c_columnsCount;
    };
}