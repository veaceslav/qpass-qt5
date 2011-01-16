/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DATABASE_H
#define DATABASE_H

#include <QList>
#include <QStringList>
#include <QString>
#include <gcrypt.h>

class Database
{
	private:
		gcry_cipher_handle *hd;
	public:
		enum values
		{
			errorreading = 1,
			badpass,
			badformat,
			dbnotexists
		};
		char readData(QList<QStringList> *data, QString key, QString db = QString::null);
		char writeData(QList<QStringList> data, QString key, QString db = QString::null);
};

struct header
{
	char id[3]; //PAS
	int version; //0
	int entriescount;
	char IV[16];
};

struct cryptedHeader
{
	char id[3]; //PAS
	char space[13];
};

struct entryHeader
{
	short int entryLength;
	short int nameLength;
	short int urlLength;
	short int usernameLength;
	short int passwordLength;
	short int notesLength;
	char space[4];
};

#endif //DATABASE_H