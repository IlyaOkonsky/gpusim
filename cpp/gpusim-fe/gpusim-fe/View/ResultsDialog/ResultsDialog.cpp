#include "ResultsDialog.h"


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

CResultsDialog::CResultsDialog(const Core::CExperiment &exp, QWidget *pParent /*= nullptr*/)
: QDialog(pParent)
{
    ui.setupUi(this);

    ui.plot->canvas()->setCursor(Qt::ArrowCursor);
    ui.plot->setAxisTitle(QwtPlot::xBottom, tr("Simulation number"));
    ui.plot->setAxisTitle(QwtPlot::yLeft, tr("Simulation time"));


    QwtPlotCurve *pCurve = new QwtPlotCurve(tr("Simulations time"));
    pCurve->setPen(QPen(Qt::blue, 2.0f));

    QwtPlotGrid *pGrid = new QwtPlotGrid();
    pGrid->setPen(QPen(Qt::lightGray));
    pCurve->attach(ui.plot);
    QwtLegend *pLegend = new QwtLegend();
    ui.plot->insertLegend(pLegend, QwtPlot::RightLegend);
    pGrid->attach(ui.plot);

    double minSimTime = -1.0f;
    double maxSimTime = -1.0f;
    double averangeSimTime = 0;
    QString minSimTimeSimName;
    QString maxSimTimeSimName;

    size_t simsCount = exp.getSimulationsRef().size();

    QVector<QPointF> samples;
    for (int i = 0; i < simsCount; ++i)
    {
        double simTime = exp.getSimulationsRef()[i].getOutput().getTotalSimulationTime();
        quint32 simNumber = exp.getSimulationsRef()[i].getNumber();
        QString simName = exp.getSimulationsRef()[i].getName();

        samples.push_back(QPointF(simNumber, simTime));

        averangeSimTime += simTime;
        if ((minSimTime == -1.0f) || (simTime < minSimTime))
        {
            minSimTime = simTime;
            minSimTimeSimName = simName;
        }

        if ((maxSimTime == -1.0f) || (simTime > maxSimTime))
        {
            maxSimTime = simTime;
            maxSimTimeSimName = simName;
        }
    }

    averangeSimTime /= (double) simsCount;

    pCurve->setSamples(samples);
    ui.plot->replot();

    ui.lbMinExecTime->setText(QString("%1 at simulation %2").arg(QString::number(minSimTime), minSimTimeSimName));
    ui.lbMaxExecTime->setText(QString("%1 at simulation %2").arg(QString::number(maxSimTime), maxSimTimeSimName));
    ui.lbAverangeExecTime->setText(QString::number(averangeSimTime));

    connect(this, SIGNAL(rejected()), SLOT(deleteLater()));
}