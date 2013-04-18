#include "Mainwindow.h"
#include "../ResultsDialog/ResultsDialog.h"
#include "../../../gpusim-fe.Core/Originals/OriginalsReader.h"

#include <QMessageBox>
#include <QFileDialog>

CMainWindow::CMainWindow(QWidget *pParent /*= nullptr*/)
    : QDialog(pParent)
{
    ui.setupUi(this);
    ui.frmProgress->setVisible(false);
    useOriginalsChanged(false);
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

    CResultsDialog::CXAsisMode mode = (ui.sbMinN->value() != ui.sbMaxN->value())
        ? CResultsDialog::XAM_N : CResultsDialog::XAM_ThreadsPerBlock;
    CResultsDialog *dlg = new CResultsDialog(exp, m_originals, mode, this);
    dlg->show();
}

//////////////////////////////////////////////////////////////////////////

void CMainWindow::executeClick()
{
    if ((ui.sbMinN->value() != ui.sbMaxN->value()) && (ui.sbMinThreadsPerBlock->value() != ui.sbMaxThreadsPerBlock->value()))
    {
        QMessageBox::critical(this, "Wrong parameters!",
            "Only one parameter (N or threads per block can be changed, not both).");
        return;
    }
    
    emit performExperiment(ui.sbMinN->value(), ui.sbMaxN->value(), ui.sbMinThreadsPerBlock->value(),
        ui.sbMaxThreadsPerBlock->value());

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

void CMainWindow::chooseOriginalsFileClick()
{
    QString originalsfileName = QFileDialog::getOpenFileName(this,
        tr("Choose originals file"), QDir::currentPath());
    ui.leOriginalsFilePath->setText(originalsfileName);
}

bool CMainWindow::loadOriginals()
{
    clearOriginals();

    quint32 minN = ui.sbMinN->value();
    quint32 maxN = ui.sbMaxN->value();
    quint32 threadsPerBlock = (minN != maxN)? ui.sbMinThreadsPerBlock->value() : 0;
    Core::COriginalsReader reader(m_originals, minN, maxN, threadsPerBlock);
    if (!reader.readOriginals(ui.leOriginalsFilePath->text()))
    {
        QMessageBox::critical(this, "Failed to load originals.", "Failed to load originals from specified file, "
            "details in log file.");
        return false;
    }

    fillUIFromOriginals();

    return true;
}

void CMainWindow::fillUIFromOriginals()
{
    quint32 minN = m_originals.front().getN();
    quint32 maxN = m_originals.back().getN();

    ui.sbMinN->setValue(m_originals.front().getN());
    ui.sbMaxN->setValue(m_originals.back().getN());

    ui.sbMinThreadsPerBlock->setValue(m_originals.front().getThreadsPerBlock());
    ui.sbMaxThreadsPerBlock->setValue(m_originals.back().getThreadsPerBlock());
}

void CMainWindow::useOriginalsChanged(bool val)
{
    ui.lbOriginalsFilePath->setVisible(val);
    ui.leOriginalsFilePath->setVisible(val);
    ui.btnChooseOriginalsFile->setVisible(val);
    ui.btnLoadOriginals->setVisible(val);

    ui.sbMinThreadsPerBlock->setEnabled(!val);
    ui.sbMaxThreadsPerBlock->setEnabled(!val);

    if (!val)
        clearOriginals();
}

void CMainWindow::clearOriginals()
{
    m_originals.clear();
}