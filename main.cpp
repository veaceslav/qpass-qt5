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
	
	//Code to check if there is any running instance of QPass.
	//It works very good on windows but on UNIX when you kill qpass
	//process it doesn't remove this shared memory.
	QSharedMemory memory("91628a41-6284-41a9-9b25-7b3cc365ced9"); 
	if(!memory.create(1, QSharedMemory::ReadWrite))
	{
#ifndef Q_OS_UNIX
		QMessageBox box;
		box.setText( QObject::tr("One instance of QPass is already running!") );
		box.setIcon( QMessageBox::Warning );
		box.exec();
		return 0;
#endif
#ifdef Q_OS_UNIX
		QMessageBox box;
		box.setText( QObject::tr("One instance of QPass is already running!\nIf you are sure that it isn't(it could occur when you kill process or application crashed) you can ignore this message, but it is dangerous to your database to have more than one instance running!") );
		box.setStandardButtons( QMessageBox::Close | QMessageBox::Ignore);
		box.setIcon( QMessageBox::Warning );
		if(box.exec() == QMessageBox::Close)
	return 0;
#endif
	}
	
	MainWindow mainWindow;
	
	return app.exec();
}
