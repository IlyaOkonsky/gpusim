#include "ResultsDialog.h"

#include "../../../gpusim-fe.Core/Originals/OriginalsHelpers.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_legend.h>

#ifdef _DEBUG
#pragma comment(lib, "qwtd.lib")
#else
#pragma comment(lib, "qwt.lib")
#endif // _DEBUG

CResultsDialog::CResultsDialog(const Core::CExperiment &exp, const Core::COriginalsList &originals,
    QWidget *pParent /*= nullptr*/)
    : QDialog(pParent)
{
    ui.setupUi(this);
    configUI(exp, originals.size() == exp.getSimulationsRef().size());

    QwtPlotCurve *pExperimentCurve = buildExperimentCurve(exp, originals);
    QwtPlotCurve *pOriginalsCurve = buildOriginalsCurve(exp, originals);
    QwtPlotCurve *pRelErrorCurve = buildRelativeErrorCurve(exp, originals);

    pExperimentCurve->attach(ui.plot);
    if (pOriginalsCurve)
        pOriginalsCurve->attach(ui.plot);

    if (pRelErrorCurve)
        pRelErrorCurve->attach(ui.plotRelativeError);

    ui.plot->replot();
    ui.plotRelativeError->replot();

    updateStatistics(exp, originals);
}

//////////////////////////////////////////////////////////////////////////

void CResultsDialog::configUI(const Core::CExperiment &exp, bool originalsPresent)
{
    ui.plot->canvas()->setCursor(Qt::ArrowCursor);
    ui.plotRelativeError->canvas()->setCursor(Qt::ArrowCursor);

    QString matrixSizeAxisTitle = tr("Matrix size");
    QString plotXAsisTitle = originalsPresent? matrixSizeAxisTitle : tr("Simulation number");
    ui.plot->setAxisTitle(QwtPlot::xBottom, plotXAsisTitle);
    ui.plot->setAxisTitle(QwtPlot::yLeft, tr("Simulation time (ms)"));
    ui.plotRelativeError->setAxisTitle(QwtPlot::xBottom, matrixSizeAxisTitle);
    ui.plotRelativeError->setAxisTitle(QwtPlot::yLeft, tr("Relative error (%)"));

    QwtLegend *pLegend = new QwtLegend();
    ui.plot->insertLegend(pLegend, QwtPlot::BottomLegend);

    QwtLegend *pLegendRError = new QwtLegend();
    ui.plotRelativeError->insertLegend(pLegendRError, QwtPlot::BottomLegend);

    QwtPlotGrid *pGrid = new QwtPlotGrid();
    pGrid->setPen(QPen(Qt::lightGray));
    pGrid->attach(ui.plot);

    QwtPlotGrid *pGridRError = new QwtPlotGrid();
    pGridRError->setPen(QPen(Qt::lightGray));
    pGridRError->attach(ui.plotRelativeError);

    ui.plotRelativeError->setVisible(originalsPresent);
    ui.gbStats->setVisible(originalsPresent);

    connect(this, SIGNAL(rejected()), SLOT(deleteLater()));
}

//////////////////////////////////////////////////////////////////////////

QwtPlotCurve * CResultsDialog::buildExperimentCurve(const Core::CExperiment &exp,
    const Core::COriginalsList &originals)
{
    size_t simsCount = exp.getSimulationsRef().size();
    bool withOriginals = (simsCount == originals.size());
    
    CPointsList samples;
    for (int i = 0; i < simsCount; ++i)
    {
        double simTime = exp.getSimulationsRef()[i].getOutput().getTotalSimulationTime();
        quint32 simNumber = exp.getSimulationsRef()[i].getNumber();
        QString simName = exp.getSimulationsRef()[i].getName();

        double xValue = withOriginals? originals[i].getMatrixSize() : simNumber;
        samples.push_back(QPointF(xValue, simTime));
    }

    QwtPlotCurve *pCurve = new QwtPlotCurve(tr("Simulation results"));
    pCurve->setPen(QPen(Qt::blue, 2.0f));
    pCurve->setSamples(samples);
    return pCurve;
}

QwtPlotCurve * CResultsDialog::buildOriginalsCurve(const Core::CExperiment &exp,
    const Core::COriginalsList &originals)
{
    size_t simsCount = exp.getSimulationsRef().size();
    if (simsCount != originals.size())
        return nullptr;

    CPointsList samples;
    for (int i = 0; i < simsCount; ++i)
        samples.push_back(QPointF(originals[i].getMatrixSize(), originals[i].getSimulationTime()));

    QwtPlotCurve *pCurve = new QwtPlotCurve(tr("Originals results"));
    pCurve->setPen(QPen(Qt::red, 2.0f));
    pCurve->setSamples(samples);
    return pCurve;
}

QwtPlotCurve * CResultsDialog::buildRelativeErrorCurve(const Core::CExperiment &exp,
    const Core::COriginalsList &originals)
{
    //
    // TODO: refactor this code (this is somelike copy-paste from the Core module)
    //

    size_t simsCount = exp.getSimulationsRef().size();
    if (simsCount != originals.size())
        return nullptr;

    CPointsList samples;
    for (int i = 0; i < simsCount; ++i)
    {
        double originalTime = originals[i].getSimulationTime();
        double simTime = exp.getSimulationsRef()[i].getOutputRef().getTotalSimulationTime();
        double distance = qAbs(originalTime - simTime);
        double e = (distance *100) / originalTime;
        
        samples.push_back(QPointF(originals[i].getMatrixSize(), e));
    }

    QwtPlotCurve *pCurve = new QwtPlotCurve(tr("Relative error"));
    pCurve->setPen(QPen(Qt::blue, 2.0f));
    pCurve->setSamples(samples);
    return pCurve;
}


void CResultsDialog::updateStatistics(const Core::CExperiment &exp, const Core::COriginalsList &originals)
{
    //
    // TODO: refactor this code (this is somelike copy-paste from the Core module)
    //    
    double averangeRelativeError;
    double absolutDiff = Core::calculateDifference(originals, exp, &averangeRelativeError);
    
    if (absolutDiff == -1.0f)
        return;

    ui.lbAbsoluteDifference->setText(QString::number(absolutDiff));
    ui.lbAverangeRelativeError->setText(QString("%1 %").arg(averangeRelativeError));
}