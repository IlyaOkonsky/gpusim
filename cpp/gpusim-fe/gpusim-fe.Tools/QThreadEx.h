#pragma once

#include "Tools_global.h"

#include <QThread>

namespace Tools
{
    class GPUSIM_FE_TOOLS_EXPORT QThreadEx : public QThread
    {
        Q_OBJECT
    public:
        explicit QThreadEx(QObject *pParent = nullptr);

    protected:
        void run();
    };
}