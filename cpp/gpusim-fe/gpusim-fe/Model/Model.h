#pragma once

#include "../../gpusim-fe.Core/Experimenter.h"

class CModel : public QObject
{
    Q_OBJECT;

public:
    CModel(QObject *pParent = nullptr);

public slots:
    void close();

    void performExperiment(quint64 minMatrixSize, quint64 maxMatrixSize, quint64 matrixSizeIncrement, quint64 blockSize);
    void cancelExperiment();
    
signals:
    void experimentProgress(quint8 val);
    void experimentResult(Core::CExitCode ec, const Core::CExperiment &exp = Core::CExperiment());

private slots:
    void onExperimeterProgress(Core::CExperimenter *pExperimenter, quint8 val);
    void onExperimeterExecuted(Core::CExperimenter *pExperimenter, Core::CExitCode ec);

private:
    void createExperementer();

private:
    typedef enum _tagCState
    {
        State_Ready,
        State_WaitingForResult
    } CState;

private:
    CState m_state;
    bool m_closing;

    Core::CExperimenter m_experimenter;

private:
    static const QString c_mmegSettingsFilePath;
    static const QString c_simulatorJarPath;
    static const QString c_experimentsWorkingDir;
};