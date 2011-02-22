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

#include <QNetworkRequest>

UpdateCheckerDialog::UpdateCheckerDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	installedVersionLabel->setText( VERSION );
	latestVersionLabel->setText( tr("Checking...") );

	manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	manager->get(QNetworkRequest(QUrl("http://qpass.sourceforge.net/latest")));
}


UpdateCheckerDialog::~UpdateCheckerDialog()
{
	delete manager;
}

void UpdateCheckerDialog::replyFinished(QNetworkReply *reply)
{
	if(reply->error() != QNetworkReply::NoError)
	{
		latestVersionLabel->setText( tr("Error") );
		return;
	}

	reply->open(QIODevice::ReadOnly);
	QByteArray data = reply->read(20);
	if(data.size() > 0)
	{
		latestVersionLabel->setText( data.split('\n')[0] );
	}
	reply->close();
	reply->deleteLater();

	if(latestVersionLabel->text() != installedVersionLabel->text())
		infoLabel->setText( tr("There is new version available to download!") );
	else
		infoLabel->setText( tr("You are using latest version of QPass") );
}

