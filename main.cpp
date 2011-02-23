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

#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QSharedMemory>
#include <QObject>
#include <QTranslator>

#ifndef Q_OS_WIN
#include <QLocalSocket>
#include <QLocalServer>
#endif

#include "MainWindow.h"
#include "PredefinedSettings.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	QCoreApplication::setApplicationName("QPass");
	QCoreApplication::setOrganizationName("QPass");
		
	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(PredefinedSettings::dataPath()+"/translations/"+locale);
	app.installTranslator(&translator);

#ifndef Q_OS_WIN
	QLocalSocket socket;
	socket.connectToServer("QPass");
	if (socket.waitForConnected(500)) 
	{
		QMessageBox box;
		box.setWindowTitle("QPass");
		box.setText( QObject::tr("One instance of QPass is already running!") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
		return 0; // Exit already a process running 
	}

	QLocalServer m_localServer;
	m_localServer.removeServer("QPass");
	int res = m_localServer.listen("QPass");
#else
	//Code to check if there is any running instance of QPass.
	//It works very good on windows but on UNIX when you kill qpass
	//process it doesn't remove this shared memory.
	QSharedMemory memory("91628a41-6284-41a9-9b25-7b3cc365ced9"); 
	if(!memory.create(1, QSharedMemory::ReadWrite))
	{
		QMessageBox box;
		box.setWindowTitle("QPass");
		box.setText( QObject::tr("One instance of QPass is already running!") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
		return 0;
	}
#endif

	MainWindow mainWindow;
	
	return app.exec();
}
