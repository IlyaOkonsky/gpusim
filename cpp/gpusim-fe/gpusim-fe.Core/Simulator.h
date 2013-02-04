#pragma once

#include "Simulation.h"
#include "Enums.h"

#include <QDir>
#include <QProcess>

namespace Core
{
    // TODO: add error reporting mechanism (to log and to UI)
    //
    class GPUSIM_FE_CORE_EXPORT CSimulator: public QObject
    {
        Q_OBJECT;

    public:
        CSimulator(const QString &simulatorJarPath, const QString &workingDir = QDir::currentPath(),
            const CSimulation &simulation = CSimulation(), QObject *pParent = nullptr);

        CSimulation getSimulation() const;
        const CSimulation &getSimulationRef() const;
        void setSimulation(const CSimulation &simulation);

    public slots:
        void execute();
        void execute(const Core::CSimulation &simulation);
        void cancel();

    signals:
        // Progress can be from 0 to 100.
        void progress(Core::CSimulator *pSim, quint8 val);
        void executed(Core::CSimulator *pSim, Core::CExitCode ec = EC_None);

    private slots:
        void onJavaProcessError(QProcess::ProcessError error);
        void onJavaProcessStarted();
        void onJavaProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
        void onJavaProcessReadyReadStdOut();
        void onJavaProcessReadyReadStdErr();

    private:
        static QString buildFullSimualtionFilePath(const QString &workingDir, const QString &fileNameFormat,
            quint32 number);

        void emitProgress(quint8 val);
        void processExecuted(CExitCode ec);
        void processCancel();

    private:
        typedef enum _tagCState
        {
            State_Ready,
            State_StartingJavaProcess,
            State_WaitJavaProcessExecResult,
            State_Canceling
        } CState;

    private:
        CState m_state;

        CSimulation m_simulation;

        QString m_simulatorJarPath;
        QString m_workingDir;
        QString m_configFilePath;
        QString m_outputFilePath;
        QProcess *m_pJavaProcess;

    private:
        static const QString c_configFileNameFormat;
        static const QString c_outputFileNameFormat;
        static const QString c_javaProcessExecFormat;
    };
}