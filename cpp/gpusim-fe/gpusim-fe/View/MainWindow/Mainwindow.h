#pragma once

#include "../../../gpusim-fe.Core/Experiment.h"
#include "../../../gpusim-fe.Core/Enums.h"

#include <QDialog>
#include "ui_Mainwindow.h"

class CMainWindow : public QDialog
{
    Q_OBJECT

public:
    CMainWindow(QWidget *pParent = nullptr);

public slots:
    void onExperimentProgress(quint8 val);
    void onExperimentResult(Core::CExitCode ec, const Core::CExperiment &exp = Core::CExperiment());

signals:
    void performExperiment(quint64 minMatrixSize, quint64 maxMatrixSize, quint64 matrixSizeIncrement, quint64 blockSize);
    void cancelExperiment();

private slots:
    void executeClick();
    void cancelClick();

private:
    Ui::CMainWindow ui;
};