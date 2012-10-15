#include "ModelThread.h"
#include "Model.h"

#include <QMutexLocker>

CModelThread::CModelThread( QObject *pParent /*= 0*/ )
    : QThread(pParent), m_bModelReady(false)
{

}

void CModelThread::waitForModelReady()
{
    QMutexLocker lock(&m_mModelReady);
    while(!m_bModelReady)
        m_wcModelReady.wait(&m_mModelReady);
}

CModelPtr CModelThread::getModel() const
{
    return m_pModel;
}

void CModelThread::run()
{
    m_mModelReady.lock();

    m_pModel = CModelPtr(new CModel());
    m_bModelReady = true;
    m_wcModelReady.wakeAll();
    m_mModelReady.unlock();

    exec();
    m_pModel.clear();
    m_bModelReady = false;
}