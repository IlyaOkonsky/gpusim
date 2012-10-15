#include "Mainwindow.h"
#include "../ResultsDialog/ResultsDialog.h"

#include <QMessageBox>

CMainWindow::CMainWindow(QWidget *pParent /*= nullptr*/)
    : QDialog(pParent)
{
    ui.setupUi(this);
    ui.frmProgress->setVisible(false);
}

void CMainWindow::onExperimentProgress(quint8 val)
{
    ui.pbProgress->setValue(val);
}

void CMainWindow::onExperimentResult(Core::CExitCode ec, const Core::CExperiment &exp /*= Core::CExperiment()*/)
{
    ui.gbParameters->setEnabled(true);
    ui.btnExecExperiment->setEnabled(true);

    ui.frmProgress->setVisible(false);

    QString status;
    switch(ec)
    {
    case Core::EC_Error:
        status = "Error. See debug output for details.";
        break;

    case Core::EC_Canceled:
        status = "Experiment canceled.";
        break;

    case Core::EC_Ok:
        status = "Experiment succeeded.";
        break;

    }
    ui.lbStatus->setText(status);

    if (ec != Core::EC_Ok)
        return;

    CResultsDialog *dlg = new CResultsDialog(exp, this);
    dlg->show();
}

//////////////////////////////////////////////////////////////////////////

void CMainWindow::executeClick()
{
    emit performExperiment(ui.sbMinMatrixSize->value(), ui.sbMaxMatrixSize->value(), ui.sbMatrixSizeIncrement->value(),
        ui.sbBlockSize->value());

    ui.gbParameters->setEnabled(false);
    ui.btnExecExperiment->setEnabled(false);

    ui.frmProgress->setVisible(true);
    ui.btnCancelExperiment->setEnabled(true);
    ui.lbStatus->setText("Executing...");   
}

void CMainWindow::cancelClick()
{
    ui.btnCancelExperiment->setEnabled(false);
    emit cancelExperiment();
    ui.lbStatus->setText("Canceling...");   
}