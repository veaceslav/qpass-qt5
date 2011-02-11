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

#ifndef PREDEFINEDSETTINGS_H
#define PREDEFINEDSETTINGS_H

#include <QString>

/*!
* This class provides functions returning values such as default paths.
*/
class PredefinedSettings
{
	public:
		/*! This function returns path to directory to store data.
		*
		* Returned value depends on platform.
		*
		* @return Data directory path.
		*/
		static QString dataPath();
		/*! This functions returns path to database.
		* 
		* Returned value depends on platform.
		*
		* @return Path to database.
		*/
		static QString databasePath();
};

#endif
