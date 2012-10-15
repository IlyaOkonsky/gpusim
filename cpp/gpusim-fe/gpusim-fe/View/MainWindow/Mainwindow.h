#pragma once

#include "Core/Experiment.h"
#include "Core/Enums.h"

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
    void performExperiment(quint64 matrixSize, quint64 minBlockSize, quint64 maxBlockSize, quint64 blockSizeIncrement);
    void cancelExperiment();

private slots:
    void executeClick();
    void cancelClick();

private:
    Ui::CMainWindow ui;
};