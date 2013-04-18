#pragma once

#include "../../../gpusim-fe.Core/Experiment.h"
#include "../../../gpusim-fe.Core/Enums.h"
#include "../../../gpusim-fe.Core/Originals/Original.h"

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
    void performExperiment(quint32 minN, quint32 maxN, quint32 minThreadsPerBlock, quint32 maxThreadsPerBlock);
    void cancelExperiment();

private slots:
    void executeClick();
    void cancelClick();
    void useOriginalsChanged(bool val);
    void chooseOriginalsFileClick();
    bool loadOriginals();

private:
    void fillUIFromOriginals();
    void clearOriginals();

private:
    Ui::CMainWindow ui;
    Core::COriginalsList m_originals;
};