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

CResultsDialog::CResultsDialog(const Core::CExperiment &exp, const Core::COriginalsList &originals, CXAsisMode mode,
    QWidget *pParent /*= nullptr*/)
    : QDialog(pParent)
{
    ui.setupUi(this);
    bool withOriginals = (exp.getSimulationsRef().size() == originals.size());

    configUI(exp, mode, withOriginals);

    QwtPlotCurve *pExperimentCurve = buildExperimentCurve(exp, mode);
    QwtPlotCurve *pOriginalsCurve = buildOriginalsCurve(exp, originals, mode);
    QwtPlotCurve *pRelErrorCurve = buildRelativeErrorCurve(exp, originals, mode);

    pExperimentCurve->attach(ui.plot);
    if (pOriginalsCurve)
        pOriginalsCurve->attach(ui.plot);

    if (pRelErrorCurve)
        pRelErrorCurve->attach(ui.plotRelativeError);

    ui.plot->replot();
    ui.plotRelativeError->replot();

    updateStatistics(exp, originals, mode, withOriginals);
}

//////////////////////////////////////////////////////////////////////////

void CResultsDialog::configUI(const Core::CExperiment &exp, CXAsisMode mode, bool withOriginals)
{
    ui.plot->canvas()->setCursor(Qt::ArrowCursor);
    ui.plotRelativeError->canvas()->setCursor(Qt::ArrowCursor);

    QString plotXAsisTitle = (mode == XAM_ThreadsPerBlock)? tr("Threads per block") : tr("N");

    ui.plot->setAxisTitle(QwtPlot::xBottom, plotXAsisTitle);
    ui.plot->setAxisTitle(QwtPlot::yLeft, tr("Simulation time (ms)"));

    ui.plotRelativeError->setAxisTitle(QwtPlot::xBottom, plotXAsisTitle);
    ui.plotRelativeError->setAxisTitle(QwtPlot::yLeft, tr("Relative error (%)"));

    QwtLegend *pLegend = new QwtLegend();
//    pLegend->set
    ui.plot->insertLegend(pLegend, QwtPlot::BottomLegend);

    QwtLegend *pLegendRError = new QwtLegend();
    ui.plotRelativeError->insertLegend(pLegendRError, QwtPlot::BottomLegend);

    QwtPlotGrid *pGrid = new QwtPlotGrid();
    pGrid->setPen(QPen(Qt::lightGray));
    pGrid->attach(ui.plot);

    QwtPlotGrid *pGridRError = new QwtPlotGrid();
    pGridRError->setPen(QPen(Qt::lightGray));
    pGridRError->attach(ui.plotRelativeError);

    ui.plotRelativeError->setVisible(withOriginals);
    ui.lbAbsoluteDifference->setVisible(withOriginals);
    ui.lbAbsoluteDifferenceDesc->setVisible(withOriginals);
    ui.lbAverangeRelativeError->setVisible(withOriginals);
    ui.lbAverangeRelativeErrorDesc->setVisible(withOriginals);

    connect(this, SIGNAL(rejected()), SLOT(deleteLater()));
}

//////////////////////////////////////////////////////////////////////////

QwtPlotCurve * CResultsDialog::buildExperimentCurve(const Core::CExperiment &exp, CXAsisMode mode)
{
    size_t simsCount = exp.getSimulationsRef().size();

    CPointsList samples;
    for (int i = 0; i < simsCount; ++i)
    {
        double  simTime       = exp.getSimulationsRef()[i].getOutput().getTotalSimulationTime();
        double  simXAsisValue = exp.getSimulationsRef()[i].getXAsisValue();

        samples.push_back(QPointF(simXAsisValue, simTime));
    }

    QwtPlotCurve *pCurve = new QwtPlotCurve(tr("Simulation results"));
    //pCurve->setPen(QPen(Qt::blue, 2.0f));
    pCurve->setPen(QPen(Qt::black, 2.0f, Qt::SolidLine));
    pCurve->setSamples(samples);
    return pCurve;
}

QwtPlotCurve * CResultsDialog::buildOriginalsCurve(const Core::CExperiment &exp,
    const Core::COriginalsList &originals, CXAsisMode mode)
{
    size_t simsCount = exp.getSimulationsRef().size();
    if (simsCount != originals.size())
        return nullptr;

    CPointsList samples;
    for (int i = 0; i < simsCount; ++i)
    {
        double  time = originals[i].getSimulationTime();
        quint32 N    = originals[i].getN();
        double  tpb  = originals[i].getThreadsPerBlock();

        double xValue = (mode == XAM_N) ? N : tpb;
        samples.push_back(QPointF(xValue, time));
    }

    QwtPlotCurve *pCurve = new QwtPlotCurve(tr("Originals results"));
    //pCurve->setPen(QPen(Qt::red, 2.0f));
    pCurve->setPen(QPen(Qt::black, 2.0f, Qt::DashLine));
    pCurve->setSamples(samples);
    return pCurve;
}

QwtPlotCurve * CResultsDialog::buildRelativeErrorCurve(const Core::CExperiment &exp,
    const Core::COriginalsList &originals, CXAsisMode mode)
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
        double simTime      = exp.getSimulationsRef()[i].getOutputRef().getTotalSimulationTime();
        double distance     = qAbs(originalTime - simTime);
        double e            = (distance *100) / originalTime;
        quint32 N           = originals[i].getN();
        double  tpb         = originals[i].getThreadsPerBlock();

        double xValue = (mode == XAM_N) ? N : tpb;

        samples.push_back(QPointF(xValue, e));
    }

    QwtPlotCurve *pCurve = new QwtPlotCurve(tr("Relative error"));
    //pCurve->setPen(QPen(Qt::blue, 2.0f));
    pCurve->setPen(QPen(Qt::black, 2.0f, Qt::SolidLine));
    pCurve->setSamples(samples);
    return pCurve;
}


void CResultsDialog::updateStatistics(const Core::CExperiment &exp, const Core::COriginalsList &originals,
    CXAsisMode mode, bool withOriginals)
{
    QString additionalInfo;
    if (mode == XAM_ThreadsPerBlock)
    {
        double minOrigTime = 0.0f;
        quint32 minOrigTPB = 0;

        if (withOriginals)
        {
            for (auto I = originals.constBegin(), E = originals.constEnd(); I != E; ++I)
            {
                double simTime = I->getSimulationTime();
                if ((minOrigTime == 0.0f) || (simTime < minOrigTime))
                {
                    minOrigTime = simTime;
                    minOrigTPB = I->getThreadsPerBlock();
                }
            }

            additionalInfo += QString("Minimal originals time (%1) is with Threads Per Block equal to %2.").arg(
                QString::number(minOrigTime), QString::number(minOrigTPB));
        }


        double minSimTime = 0.0f;
        double minTPB = 0.0f;
        for (auto I = exp.getSimulationsRef().constBegin(), E = exp.getSimulationsRef().constEnd(); I != E; ++I)
        {
            double simTime = I->getOutputRef().getTotalSimulationTime();
            if ((minSimTime == 0.0f) || (simTime < minSimTime))
            {
                minSimTime = simTime;
                minTPB = I->getXAsisValue();
            }

        }

        if (minOrigTPB)
        {
            minTPB = minOrigTPB;
            additionalInfo += QChar(' ');
        }

        additionalInfo += QString("Minimal simulation time (%1) is with Threads Per Block equal to %2.").arg(
            QString::number(minSimTime), QString::number(minTPB));
    }

    ui.lbAdditionalInfo->setText(additionalInfo);
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