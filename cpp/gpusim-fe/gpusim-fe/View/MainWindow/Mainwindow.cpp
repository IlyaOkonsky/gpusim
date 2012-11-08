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

    CResultsDialog *dlg = new CResultsDialog(exp, m_originals, this);
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

void CMainWindow::chooseOriginalsFileClick()
{
    QString originalsfileName = QFileDialog::getOpenFileName(this,
        tr("Choose originals file"), QDir::currentPath());
    ui.leOriginalsFilePath->setText(originalsfileName);
}

bool CMainWindow::loadOriginals()
{
    clearOriginals();

    Core::COriginalsReader reader(m_originals, ui.sbMinMatrixSize->value(),
        ui.sbMaxMatrixSize->value(), ui.sbMatrixSizeIncrement->value());
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
    quint32 minMatrixSize = m_originals.front().getMatrixSize();
    quint32 maxMatrixSize = m_originals.back().getMatrixSize();
    quint32 matrixSizeIncrement = (m_originals.back().getMatrixSize() - m_originals.front().getMatrixSize()) /
        (m_originals.size() - 1);

    ui.sbBlockSize->setValue(16);
    ui.sbMinMatrixSize->setValue(minMatrixSize);
    ui.sbMaxMatrixSize->setValue(maxMatrixSize);
    ui.sbMatrixSizeIncrement->setValue(matrixSizeIncrement);
}

void CMainWindow::useOriginalsChanged(bool val)
{
    ui.lbOriginalsFilePath->setVisible(val);
    ui.leOriginalsFilePath->setVisible(val);
    ui.btnChooseOriginalsFile->setVisible(val);
    ui.btnLoadOriginals->setVisible(val);

    if (!val)
        clearOriginals();
}

void CMainWindow::clearOriginals()
{
    m_originals.clear();
}