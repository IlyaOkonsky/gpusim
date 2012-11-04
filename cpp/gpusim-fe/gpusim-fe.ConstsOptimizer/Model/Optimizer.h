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
        QString m_originalsFilePath;
        QString m_configFilePath;
        QString m_outputFilePath;

        COriginalsList m_originals;
    };
}