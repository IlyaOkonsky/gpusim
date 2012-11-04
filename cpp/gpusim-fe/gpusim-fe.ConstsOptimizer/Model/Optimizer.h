#pragma once

#include "Original.h"

#include <QObject>

namespace Model
{
    class COptimizer : public QObject
    {
        Q_OBJECT;

    public:
        COptimizer(const QString &originalsFilePath, const QString &configFilePath,
            const QString &outputFilePath, QObject *pParent = nullptr);

    public slots:
        void optimize();

    private:
        static bool logAndReturn(bool res);

    private:
        void processOptimize();

        bool readOriginals();

        // Checks result for true, writes it into debug with "..." prefix and quits if check fails.
        void checkResult(bool res);

    private:
        QString m_originalsFilePath;
        QString m_configFilePath;
        QString m_outputFilePath;

        COriginalsList m_originals;
    };
}