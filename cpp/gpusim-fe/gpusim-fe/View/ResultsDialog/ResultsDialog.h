#pragma once

#include <QDialog>
#include "ui_ResultsDialog.h"

#include <qwt_plot_curve.h>
#include "../../../gpusim-fe.Core/Experiment.h"
#include "../../../gpusim-fe.Core/Originals/Original.h"

#include <QVector>
#include <QPointF>

class CResultsDialog : public QDialog
{
    Q_OBJECT

public:
    CResultsDialog(const Core::CExperiment &exp, const Core::COriginalsList &originals,
        QWidget *pParent = nullptr);

private:
    typedef QVector<QPointF> CPointsList;

private:
    void configUI(const Core::CExperiment &exp, bool originalsPresent);

    QwtPlotCurve *buildExperimentCurve(const Core::CExperiment &exp,
        const Core::COriginalsList &originals);

    // Returns nullptr if originals and experiment are not for same matrix sizes
    //
    QwtPlotCurve *buildOriginalsCurve(const Core::CExperiment &exp,
        const Core::COriginalsList &originals);

    // Returns nullptr if originals and experiment are not for same matrix sizes
    //
    QwtPlotCurve *buildRelativeErrorCurve(const Core::CExperiment &exp,
        const Core::COriginalsList &originals);

    void updateStatistics(const Core::CExperiment &exp, const Core::COriginalsList &originals);

private:
    Ui::CResultsDialog ui;

};