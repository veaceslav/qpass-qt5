/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "database.h"

#include <QMessageBox>
#include <QFile>
#include <ctime>

using namespace std;


char Database::readData(QList<QStringList> *data, QString key, QString db)
{
	if(db.isNull())
	{
#ifdef Q_WS_X11
		db = getenv("HOME");
		db += "/.qpassdb";
#else
		db = "db";
#endif //Q_WS_X11
	}
	QFile file(db);
	if(!file.open(QIODevice::ReadOnly))
	{
		if(file.error() == QFile::OpenError)
			return dbnotexists;
		return errorreading;
	}
	
	struct header head;
	file.read((char*)&head, sizeof(header));
	if(head.id[0] == 'P' && head.id[1] == 'A' && head.id[2] == 'S')
	{
		if(head.version == 1)
		{
			const char *version = gcry_check_version(NULL);
			gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
			gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
			gcry_error_t error = gcry_cipher_open(&hd, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
			char keyc[32];
			memset((char*)&keyc, 0, 32);
			strcpy((char*)&keyc, key.toAscii());
			gcry_cipher_setkey(hd, keyc, 16);
			gcry_cipher_setiv(hd, head.IV, 16); 
			struct cryptedHeader chead;
			file.read((char*)&chead, sizeof(cryptedHeader));
			gcry_cipher_decrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
			if(chead.id[0] == 'P' && chead.id[1] == 'A' && chead.id[2] == 'S')
			{
				for(int i = 0; i < head.entriescount; i++)
				{
					QStringList templist;
					struct entryHeader e;
					file.read((char*)&e, sizeof(entryHeader));
					gcry_cipher_decrypt(hd, &e, sizeof(entryHeader), NULL, 0);
					char *entry = (char*)malloc(e.entryLength);
					file.read(entry, e.entryLength);
					gcry_cipher_decrypt(hd, entry, e.entryLength, NULL, 0);
					char temp[500];
					int start = 0;
					memcpy(temp, &entry[start], e.nameLength);
					start += e.nameLength;
					temp[e.nameLength] = 0;
					templist.append(temp);
					
					memcpy(temp, &entry[start], e.urlLength);
					start += e.urlLength;
					temp[e.urlLength] = 0;
					templist.append(temp);
					
					memcpy(temp, &entry[start], e.usernameLength);
					start += e.usernameLength;
					temp[e.usernameLength] = 0;
					templist.append(temp);
					
					memcpy(temp, &entry[start], e.passwordLength);
					start += e.passwordLength;
					temp[e.passwordLength] = 0;
					templist.append(temp);
					
					memcpy(temp, &entry[start], e.notesLength);
					start += e.notesLength;
					temp[e.notesLength] = 0;
					templist.append(temp);
					data->append(templist);
					delete entry;
				}
			}
			else
			{
				gcry_cipher_close(hd);
				file.close();
				return badpass;
			}
			gcry_cipher_close(hd);
		}
		else if(head.version == 2)
		{
			const char *version = gcry_check_version(NULL);
			gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
			gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
			gcry_error_t error = gcry_cipher_open(&hd, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
			char keyhash[32];
			gcry_md_hash_buffer(GCRY_MD_SHA256, (char*)&keyhash, key.toAscii(), key.length());
			gcry_cipher_setkey(hd, (char*)&keyhash, 32);
			gcry_cipher_setiv(hd, head.IV, 16); 
			struct cryptedHeader chead;
			file.read((char*)&chead, sizeof(cryptedHeader));
			gcry_cipher_decrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
			if(chead.id[0] == 'P' && chead.id[1] == 'A' && chead.id[2] == 'S')
			{
				for(int i = 0; i < head.entriescount; i++)
				{
					QStringList templist;
					struct entryHeader e;
					file.read((char*)&e, sizeof(entryHeader));
					gcry_cipher_decrypt(hd, &e, sizeof(entryHeader), NULL, 0);
					char *entry = (char*)malloc(e.entryLength);
					file.read(entry, e.entryLength);
					gcry_cipher_decrypt(hd, entry, e.entryLength, NULL, 0);
					char temp[500];
					int start = 0;
					memcpy(temp, &entry[start], e.nameLength);
					start += e.nameLength;
					temp[e.nameLength] = 0;
					templist.append(temp);
					
					memcpy(temp, &entry[start], e.urlLength);
					start += e.urlLength;
					temp[e.urlLength] = 0;
					templist.append(temp);
					
					memcpy(temp, &entry[start], e.usernameLength);
					start += e.usernameLength;
					temp[e.usernameLength] = 0;
					templist.append(temp);
					
					memcpy(temp, &entry[start], e.passwordLength);
					start += e.passwordLength;
					temp[e.passwordLength] = 0;
					templist.append(temp);
					
					memcpy(temp, &entry[start], e.notesLength);
					start += e.notesLength;
					temp[e.notesLength] = 0;
					templist.append(temp);
					data->append(templist);
					delete entry;
				}
			}
			else
			{
				gcry_cipher_close(hd);
				file.close();
				return badpass;
			}
			gcry_cipher_close(hd);
		}
	}
	else
	{
		file.close();
		return badformat;
	}
	file.close();
	return 0;
}

char Database::writeData(QList<QStringList> data, QString key, QString db)
{
	//QString db;
	if(db.isNull())
	{
#ifdef Q_WS_X11
		db = getenv("HOME");
		db += "/.qpassdb";
#else
		db = "db";
#endif //Q_WS_X11
	}
	QFile file(db);
	file.setPermissions(QFile::ReadOwner|QFile::WriteOwner);
	if(!file.open(QIODevice::WriteOnly))
	{
		return -1;
	}
	struct header head;
	head.id[0] = 'P';
	head.id[1] = 'A';
	head.id[2] = 'S';
	head.version = 2;
	head.entriescount = data.size();
	
	srand(time(0));
	for(int i = 0; i < 16; i++)
		head.IV[i] = rand();
	if( file.write((char*)&head, sizeof(header)) == -1 )
		return -1;
	const char *version = gcry_check_version(NULL);
	gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
	gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
	gcry_error_t error = gcry_cipher_open(&hd, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
	char keyhash[32];
	gcry_md_hash_buffer(GCRY_MD_SHA256, (char*)&keyhash, key.toAscii(), key.length());
	gcry_cipher_setkey(hd, (char*)&keyhash, 32);
	gcry_cipher_setiv(hd, head.IV, 16); 
	
	struct cryptedHeader chead;
	chead.id[0] = 'P';
	chead.id[1] = 'A';
	chead.id[2] = 'S';
	gcry_cipher_encrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
	if( file.write((char*)&chead, sizeof(cryptedHeader)) == -1 )
		return -1;
	for(int i = 0; i < data.size(); i++)
	{
		struct entryHeader e;
		e.entryLength = data[i][0].size()+data[i][1].size()+data[i][2].size()+data[i][3].size()+data[i][4].size();
		e.entryLength = ((e.entryLength/16)+1)*16;
		e.nameLength = data[i][0].size();
		e.urlLength = data[i][1].size();
		e.usernameLength = data[i][2].size();
		e.passwordLength = data[i][3].size();
		e.notesLength = data[i][4].size();
		char *entry = (char*)malloc(e.entryLength);
		int start = 0;
		for(int j = 0; j < 5; j++)
		{
			memcpy(entry+start, data[i][j].toAscii(), data[i][j].size());
			start += data[i][j].size();
		}
		int length = e.entryLength;
		gcry_cipher_encrypt(hd, &e, sizeof(entryHeader), NULL, 0);
		gcry_cipher_encrypt(hd, entry, length, NULL, 0);
		if( file.write((char*)&e, sizeof(entryHeader)) == -1 )
			return -1;
		if( file.write(entry, length) == -1 )
			return -1;
		delete entry;
	}
	gcry_cipher_close(hd);

	file.close();
	return 0;
}