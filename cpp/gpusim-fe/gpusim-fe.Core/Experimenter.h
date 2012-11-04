#pragma once

#include "Simulator.h"
#include "Experiment.h"

namespace Core
{
    class GPUSIM_FE_CORE_EXPORT CExperimenter: public QObject
    {
        Q_OBJECT;

    public:
        CExperimenter(const QString &simulatorJarPath, const QString &workingDir = QDir::currentPath(),
            const CExperiment &experiment = CExperiment(), QObject *pParent = nullptr);

        CExperiment getExperiment() const;
        const CExperiment &getExperimentRef() const;
        void setExperiment(const CExperiment &experiment);

        bool isReady() const;
        bool isExecuting() const;
        bool isCanceling() const;

    public slots:
        void execute();
        void execute(const Core::CExperiment &experiment);
        void cancel();

    signals:
        // Progress can be from 0 to 100.
        void progress(Core::CExperimenter *pExperimenter, quint8 val);
        void executed(Core::CExperimenter *pExperimenter, Core::CExitCode ec = EC_None);

    private slots:
        void onSimulatorProgress(Core::CSimulator *pSim, quint8 val);
        void onSimulatorExecuted(Core::CSimulator *pSim, Core::CExitCode ec);

    private:
        void processCurrentSim();

        void emitProgress(quint8 val);
        void processExecuted(CExitCode ec);
        void processCancel();

        void createSimulator(const CSimulation &sim);
    private:
        typedef enum _tagCState
        {
            State_Ready,
            State_Executing,
            State_Canceling
        } CState;

    private:
        CState m_state;

        QString m_simulatorJarPath;
        QString m_workingDir;
        QString m_simulatorsWorkingDir;
        CSimulator *m_pSimulator;
        CExperiment m_experiment;

        CSimulationsList m_simulations;
        CSimulationsList::iterator m_itCurrentSim;
    };

    typedef QSharedPointer<CExperimenter> CExperimenterPtr;
}