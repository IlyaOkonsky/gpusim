#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

namespace Tools
{
    template <class TModel>
    class CModelThread : public QThread
    {
    public:
        typedef QSharedPointer<TModel> TModelPtr;

    public:
        CModelThread(QObject *pParent = nullptr)
            : QThread(pParent), m_bModelReady(false)
        {

        }

        void waitForModelReady()
        {
            QMutexLocker lock(&m_mModelReady);
            while(!m_bModelReady)
                m_wcModelReady.wait(&m_mModelReady);
        }

        // Returns pointer to the model object created in a new thread.
        // This method can return null pointer if no model object exists.
        // To ensure that model object is created in a new thread call
        // waitForModelReady() after model thread stars.
        TModelPtr getModel() const
        {
            return m_pModel;
        }

        // After user closed application we must close model, but closing must be async, so call close slot from model
        // thread and wait for  model thread ends. Starts event loop, and invokes "close" slot of the model.
        // NOTE: Connects finished() signal to event's loop quit() slots.
        //
        void waitForFinish()
        {
            QEventLoop eventLoop;
            eventLoop.connect(this, SIGNAL(finished()), SLOT(quit()), Qt::QueuedConnection);
            QMetaObject::invokeMethod(m_pModel.data(), "close", Qt::QueuedConnection);
            eventLoop.exec();
        }

    protected:
        void run() override
        {
            m_mModelReady.lock();

            m_pModel = TModelPtr(new TModel());
            m_bModelReady = true;
            m_wcModelReady.wakeAll();
            m_mModelReady.unlock();

            exec();
            m_pModel.clear();
            m_bModelReady = false;
        }

    private:
        TModelPtr m_pModel;

        bool m_bModelReady;
        QMutex m_mModelReady;
        QWaitCondition m_wcModelReady;
    };
}