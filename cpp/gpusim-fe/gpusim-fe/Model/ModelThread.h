#pragma once

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QSharedPointer>

class CModel;

typedef QSharedPointer<CModel> CModelPtr;

class CModelThread : public QThread
{
public:
    CModelThread(QObject *pParent = 0);

    void waitForModelReady();

    // Returns pointer to the model object created in a new thread.
    // This method can return null pointer if no model object exists.
    // To ensure that model object is created in a new thread call
    // waitForModelReady() after model thread stars.
    CModelPtr getModel() const;

protected:
    virtual void run();

private:
    CModelPtr m_pModel;

    bool m_bModelReady;
    QMutex m_mModelReady;
    QWaitCondition m_wcModelReady;
};