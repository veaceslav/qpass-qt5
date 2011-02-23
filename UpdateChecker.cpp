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

#include <QNetworkRequest>

#include "UpdateChecker.h"

UpdateChecker::UpdateChecker(QObject *parent) : QObject(parent)
{
}

void UpdateChecker::checkForUpdates()
{
	manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	manager->get(QNetworkRequest(QUrl("http://qpass.sourceforge.net/latest")));
}

void UpdateChecker::replyFinished(QNetworkReply *reply)
{
	if(reply->error() != QNetworkReply::NoError)
	{
		emit gotLatestVersion(QString::Null());
		return;
	}

	reply->open(QIODevice::ReadOnly);
	QByteArray data = reply->read(20);
	QString version;
	if(data.size() > 0)
	{
		version = data.split('\n')[0];
	}
	reply->close();
	reply->deleteLater();
	manager->deleteLater();

	emit gotLatestVersion(version);
}

