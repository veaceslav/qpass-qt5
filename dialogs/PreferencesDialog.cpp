/***************************************************************************
 *   Copyright (c) 2010-2012 Mateusz Piękos <mateuszpiekos@gmail.com>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	benchmark = NULL;
	connect(benchmarkButton, SIGNAL(clicked()), this, SLOT(runBenchmark()));
}

void PreferencesDialog::setVisibleElementsAmount(int amount)
{
	visibleElementsAmountBox->setValue(amount);
}

int PreferencesDialog::getVisibleElementsAmount()
{
	return visibleElementsAmountBox->value();
}

void PreferencesDialog::setClipboardTimeout(int timeout)
{
	if(timeout == 0)
		clipboardBox->setChecked(false);
	else
		clipboardBox->setChecked(true);
	clipboardTimeoutBox->setValue(timeout);
}

int PreferencesDialog::getClipboardTimeout()
{
	if(!clipboardBox->isChecked())
		return 0;
	else
		return clipboardTimeoutBox->value();
}

void PreferencesDialog::setShowPassword(bool show)
{
	showPasswordBox->setChecked(show);
}

bool PreferencesDialog::getShowPassword()
{
	return showPasswordBox->isChecked();
}

void PreferencesDialog::setNumberOfIterations(int iterations)
{
	iterationsBox->setValue(iterations);
}

int PreferencesDialog::getNumberOfIterations()
{
	return iterationsBox->value();
}

void PreferencesDialog::runBenchmark()
{
	if(benchmark == NULL)
	{
		benchmark = new Benchmark(this);
		connect(benchmark, SIGNAL(finished()), this, SLOT(showResult()));
	}
	resultLabel->setText(tr("Checking..."));
	benchmark->setNumberOfIterations(iterationsBox->value());
	benchmark->start();
}

void PreferencesDialog::showResult()
{
	resultLabel->setText(QString::number(benchmark->getTime())+"s");
}
