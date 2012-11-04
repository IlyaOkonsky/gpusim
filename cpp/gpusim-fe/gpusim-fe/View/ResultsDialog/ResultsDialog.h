#pragma once

#include <QDialog>
#include "ui_ResultsDialog.h"

#include <qwt_plot_curve.h>
#include "../../../gpusim-fe.Core/Experiment.h"

class CResultsDialog : public QDialog
{
    Q_OBJECT

public:
    CResultsDialog(const Core::CExperiment &exp, QWidget *pParent = nullptr);

private:
    Ui::CResultsDialog ui;

};