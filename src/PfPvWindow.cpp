/*
 * Copyright (c) 2009 Sean C. Rhea (srhea@srhea.net)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "PfPvWindow.h"
#include "PfPvPlot.h"
#include "RideItem.h"
#include <QtGui>

PfPvWindow::PfPvWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout;
    QHBoxLayout *qaLayout = new QHBoxLayout;

    pfPvPlot = new PfPvPlot();
    QLabel *qaCPLabel = new QLabel(tr("Watts:"), this);
    qaCPValue = new QLineEdit(QString("%1").arg(pfPvPlot->getCP()));
    qaCPValue->setValidator(new QIntValidator(0, 9999, qaCPValue));
    QLabel *qaCadLabel = new QLabel(tr("RPM:"), this);
    qaCadValue = new QLineEdit(QString("%1").arg(pfPvPlot->getCAD()));
    qaCadValue->setValidator(new QIntValidator(0, 999, qaCadValue));
    QLabel *qaClLabel = new QLabel(tr("Crank Length (m):"), this);
    qaClValue = new QLineEdit(QString("%1").arg(1000 * pfPvPlot->getCL()));
    shadeZonesPfPvCheckBox = new QCheckBox;
    shadeZonesPfPvCheckBox->setText("Shade zones");
    shadeZonesPfPvCheckBox->setCheckState(Qt::Checked);

    qaLayout->addWidget(qaCPLabel);
    qaLayout->addWidget(qaCPValue);
    qaLayout->addWidget(qaCadLabel);
    qaLayout->addWidget(qaCadValue);
    qaLayout->addWidget(qaClLabel);
    qaLayout->addWidget(qaClValue);
    qaLayout->addWidget(shadeZonesPfPvCheckBox);

    vlayout->addWidget(pfPvPlot);
    vlayout->addLayout(qaLayout);
    setLayout(vlayout);

    connect(pfPvPlot, SIGNAL(changedCP(const QString&)),
            qaCPValue, SLOT(setText(const QString&)) );
    connect(pfPvPlot, SIGNAL(changedCAD(const QString&)),
            qaCadValue, SLOT(setText(const QString&)) );
    connect(pfPvPlot, SIGNAL(changedCL(const QString&)),
            qaClValue, SLOT(setText(const QString&)) );
    connect(qaCPValue, SIGNAL(editingFinished()),
	    this, SLOT(setQaCPFromLineEdit()));
    connect(qaCadValue, SIGNAL(editingFinished()),
	    this, SLOT(setQaCADFromLineEdit()));
    connect(qaClValue, SIGNAL(editingFinished()),
	    this, SLOT(setQaCLFromLineEdit()));
    connect(shadeZonesPfPvCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(setShadeZonesPfPvFromCheckBox()));
}

void
PfPvWindow::setData(RideItem *ride)
{
    pfPvPlot->setData(ride);
    // update the QLabel widget with the CP value set in PfPvPlot::setData()
    qaCPValue->setText(QString("%1").arg(pfPvPlot->getCP()));
}

void
PfPvWindow::zonesChanged()
{
    pfPvPlot->refreshZoneItems();
    pfPvPlot->replot();
    qaCPValue->setText(QString("%1").arg(pfPvPlot->getCP()));
}

void
PfPvWindow::setShadeZonesPfPvFromCheckBox()
{
    if (pfPvPlot->shadeZones() != shadeZonesPfPvCheckBox->isChecked()) {
        pfPvPlot->setShadeZones(shadeZonesPfPvCheckBox->isChecked());
    }
}

void
PfPvWindow::setQaCPFromLineEdit()
{
    int value = qaCPValue->text().toInt();
    pfPvPlot->setCP(value);
}

void
PfPvWindow::setQaCADFromLineEdit()
{
    int value = qaCadValue->text().toInt();
    pfPvPlot->setCAD(value);
}

void
PfPvWindow::setQaCLFromLineEdit()
{
    double value = qaClValue->text().toDouble();
    pfPvPlot->setCL(value);
}


