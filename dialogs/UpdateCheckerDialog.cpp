/***************************************************************************
 *   Copyright (c) 2010-2011 Mateusz Piękos <mateuszpiekos@gmail.com>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "UpdateCheckerDialog.h"
#include "qpass-config.h"

UpdateCheckerDialog::UpdateCheckerDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	installedVersionLabel->setText( VERSION );
	latestVersionLabel->setText( tr("Checking...") );

	checker = new UpdateChecker(this);
	connect(checker, SIGNAL(gotLatestVersion(QString)), this, SLOT(setLatestVersion(QString)));
	checker->checkForUpdates();
}

void UpdateCheckerDialog::setAutomaticCheckingChecked(bool checked)
{
	automaticCheckingBox->setChecked(checked);
}

bool UpdateCheckerDialog::getAutomaticCheckingChecked()
{
	return automaticCheckingBox->isChecked();
}

void UpdateCheckerDialog::setLatestVersion(QString version)
{
	if(version == QString::Null())
	{
		latestVersionLabel->setText( tr("Error") );
		return;
	}
	latestVersionLabel->setText(version);

	if(latestVersionLabel->text() != installedVersionLabel->text())
		infoLabel->setText( tr("There is new version available to download!") );
	else
		infoLabel->setText( tr("You are using latest version of QPass") );
}

