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

#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class UpdateChecker : public QObject
{
	Q_OBJECT
	public:
		UpdateChecker(QObject *parent);
		void checkForUpdates();
	private:
		QNetworkAccessManager *manager;
	private slots:
		void replyFinished(QNetworkReply *reply);
	signals:
		void gotLatestVersion(QString);
};

#endif

