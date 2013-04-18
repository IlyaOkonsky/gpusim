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
    typedef enum _tagCXAsisMode
    {
        XAM_N,
        XAM_ThreadsPerBlock
    } CXAsisMode;

    CResultsDialog(const Core::CExperiment &exp, const Core::COriginalsList &originals, CXAsisMode mode = XAM_N,
        QWidget *pParent = nullptr);

private:
    typedef QVector<QPointF> CPointsList;

private:
    void configUI(const Core::CExperiment &exp, CXAsisMode mode, bool withOriginals);

    QwtPlotCurve *buildExperimentCurve(const Core::CExperiment &exp, CXAsisMode mode);

    // Returns nullptr if originals and experiment are not for same matrix sizes
    //
    QwtPlotCurve *buildOriginalsCurve(const Core::CExperiment &exp,
        const Core::COriginalsList &originals, CXAsisMode mode);

    // Returns nullptr if originals and experiment are not for same matrix sizes
    //
    QwtPlotCurve *buildRelativeErrorCurve(const Core::CExperiment &exp,
        const Core::COriginalsList &originals, CXAsisMode mode);

    void updateStatistics(const Core::CExperiment &exp, const Core::COriginalsList &originals, CXAsisMode mode,
        bool withOriginals);

private:
    Ui::CResultsDialog ui;

};