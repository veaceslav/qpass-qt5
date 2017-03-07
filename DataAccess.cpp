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
#include <time.h>

#include "DataAccess.h"

DataAccess::DataAccess(const QString &path,const QString &password)
{
	this->path = path;
	this->password = password;
	this->key = NULL;
	file = new QFile(path);
}

DataAccess::~DataAccess()
{
	if(key != NULL)
		delete key;
	delete file;
}

errorCode DataAccess::read(QList< QVector< QString> > &list)
{
	if(!file->open(QIODevice::ReadOnly))
		return FILE_ERROR;

	QVector< QString > tempVector(5); //In this version there are 5 columns
	bool oldVersion = false;

	file->seek(0);
	struct header head;
	if(!file->read((char*)&head, sizeof(header)))
		return FILE_ERROR;
	if(head.id[0] == 'P' && head.id[1] == 'A' && head.id[2] == 'S')
	{
		if(head.version == 1)
		{
			oldVersion = true;
			gcry_check_version(NULL);
			gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
			gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
			gcry_cipher_open(&hd, GCRY_CIPHER_AES128, GCRY_CIPHER_MODE_CBC, 0);
			if(key == NULL)
				key = new char[32];
			memset(key, 0, 32);
            strcpy(key, password.toLatin1());
			gcry_cipher_setkey(hd, key, 16);
			delete key;
			key = NULL;
			gcry_cipher_setiv(hd, head.IV, 16); 
			struct cryptedHeader chead;
			if(!file->read((char*)&chead, sizeof(cryptedHeader)))
				return FILE_ERROR;
			gcry_cipher_decrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
			if(!(chead.id[0] == 'P' && chead.id[1] == 'A' && chead.id[2] == 'S'))
				return INVALID_PASSWORD; 
		}
		else if(head.version == 2)
		{
			oldVersion = true;
			gcry_check_version(NULL);
			gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
			gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
			gcry_cipher_open(&hd, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
			if(key == NULL)
			{
				key = new char[32];
				gcry_md_hash_buffer(GCRY_MD_SHA256, key, password.toUtf8().data(), password.toUtf8().size());
			}
			gcry_cipher_setkey(hd, key, 32);
			delete key;
			key = NULL;
			gcry_cipher_setiv(hd, head.IV, 16); 
			struct cryptedHeader chead;
			if(!file->read((char*)&chead, sizeof(cryptedHeader)))
				return FILE_ERROR;
			gcry_cipher_decrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
			if(!(chead.id[0] == 'P' && chead.id[1] == 'A' && chead.id[2] == 'S'))
				return INVALID_PASSWORD;
		}
		else if(head.version == 3)
		{
			gcry_check_version(NULL);
			gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
			gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
			err = gcry_cipher_open(&hd, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
			if(err)
				return GCRYPT_ERROR;
			if(!file->read((char*)&pbkdf2, sizeof(pbkdf2Header)))
				return FILE_ERROR;
			if(key == NULL)
			{
				key = new char[32];
				err = gcry_kdf_derive(password.toUtf8().data(), password.toUtf8().size(), GCRY_KDF_PBKDF2, GCRY_MD_SHA256, pbkdf2.salt, sizeof(pbkdf2.salt), pbkdf2.iterations, 32, key);
				if(err)
					return GCRYPT_ERROR;
			}
			gcry_cipher_setkey(hd, key, 32);
			gcry_cipher_setiv(hd, head.IV, 16); 
			struct cryptedHeader chead;
			if(!file->read((char*)&chead, sizeof(cryptedHeader)))
				return FILE_ERROR;
			gcry_cipher_decrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
			if(!(chead.id[0] == 'P' && chead.id[1] == 'A' && chead.id[2] == 'S'))
				return INVALID_PASSWORD;
 
		}
		else
			return INVALID_PASSWORD;

		for(int i = 0; i < head.entriescount; i++)
		{    
			struct entryHeader e;
			if(!file->read((char*)&e, sizeof(entryHeader)))
				return FILE_ERROR;
			gcry_cipher_decrypt(hd, &e, sizeof(entryHeader), NULL, 0);

			char *entry = (char*)malloc(e.entryLength);
			if(!file->read(entry, e.entryLength))
				return FILE_ERROR;
			gcry_cipher_decrypt(hd, entry, e.entryLength, NULL, 0);

			char temp[500];
			int start = 0;
			memcpy(temp, &entry[start], e.nameLength);
			start += e.nameLength;
			temp[e.nameLength] = 0;
			tempVector[0] = QString::fromUtf8(temp);
					
			memcpy(temp, &entry[start], e.urlLength);
			start += e.urlLength;
			temp[e.urlLength] = 0;
			tempVector[1] = QString::fromUtf8(temp);
					
			memcpy(temp, &entry[start], e.usernameLength);
			start += e.usernameLength;
			temp[e.usernameLength] = 0;
			tempVector[2] = QString::fromUtf8(temp);
					
			memcpy(temp, &entry[start], e.passwordLength);
			start += e.passwordLength;
			temp[e.passwordLength] = 0;
			tempVector[3] = QString::fromUtf8(temp);
					
			memcpy(temp, &entry[start], e.notesLength);
			start += e.notesLength;
			temp[e.notesLength] = 0;
			tempVector[4] = QString::fromUtf8(temp);

			list.append(tempVector);
			free(entry);
		}
		gcry_cipher_close(hd);
	}
	file->close();

	if(oldVersion)
		return SUCCESS_OLD_VERSION;
	else
		return SUCCESS;
}

errorCode DataAccess::write(const QList< QVector< QString> > &data)
{
	if(!file->open(QIODevice::Truncate | QIODevice::WriteOnly))
		return FILE_ERROR;
	struct header head;

	head.id[0] = 'P';
	head.id[1] = 'A';
	head.id[2] = 'S';
	head.version = 3;
	head.entriescount = data.count();
	
	srand(time(0));
	for(int i = 0; i < 16; i++)
		head.IV[i] = rand() & 0xFF;

	if( file->write((char*)&head, sizeof(header)) == -1 )
		return FILE_ERROR;

	gcry_check_version(NULL);
	gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
	gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
	gcry_cipher_open(&hd, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
	if(key == NULL)
	{
		for(int i = 0; i < 8; i++)
			pbkdf2.salt[i] = rand() & 0xFF;

		key = new char[32];
		err = gcry_kdf_derive(password.toUtf8().data(), password.toUtf8().size(), GCRY_KDF_PBKDF2, GCRY_MD_SHA256, pbkdf2.salt, sizeof(pbkdf2.salt), pbkdf2.iterations, 32, key);
		if(err)
			return GCRYPT_ERROR;
	}
	gcry_cipher_setkey(hd, key, 32);
	gcry_cipher_setiv(hd, head.IV, 16); 

	if( file->write((char*)&pbkdf2, sizeof(pbkdf2Header)) == -1 )
		return FILE_ERROR;
	
	struct cryptedHeader chead;
	chead.id[0] = 'P';
	chead.id[1] = 'A';
	chead.id[2] = 'S';
	gcry_cipher_encrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
	if( file->write((char*)&chead, sizeof(cryptedHeader)) == -1 )
		return FILE_ERROR;

	for(int i = 0; i < data.count(); i++)
	{
		struct entryHeader e;
		e.nameLength = data[i][0].toUtf8().size();
		e.urlLength = data[i][1].toUtf8().size();
		e.usernameLength = data[i][2].toUtf8().size();
		e.passwordLength = data[i][3].toUtf8().size();
		e.notesLength = data[i][4].toUtf8().size();
		e.entryLength = e.nameLength + e.urlLength + e.usernameLength + e.passwordLength + e.notesLength;
		e.entryLength = ((e.entryLength/16)+1)*16;
		char *entry = (char*)malloc(e.entryLength);
		int start = 0;
		for(int j = 0; j < 5; j++)
		{
			memcpy(entry+start, data[i][j].toUtf8().constData(), data[i][j].toUtf8().size());
			start += data[i][j].toUtf8().size();
		}
		int length = e.entryLength;
		gcry_cipher_encrypt(hd, &e, sizeof(entryHeader), NULL, 0);
		gcry_cipher_encrypt(hd, entry, length, NULL, 0);
		if( file->write((char*)&e, sizeof(entryHeader)) == -1 )
			return FILE_ERROR;
		if( file->write(entry, length) == -1 )
			return FILE_ERROR;
		free(entry);
	}
	gcry_cipher_close(hd);
	file->close();
	return SUCCESS;
}

void DataAccess::setPassword(const QString &password)
{
	this->password = password;
	free(key);
	key = NULL;
}

QString DataAccess::getPassword()
{
	return this->password;
}

void DataAccess::setNumberOfIterations(int iterations)
{
	if(key != NULL)
	{
		delete key;
		key = NULL;
	}
	pbkdf2.iterations = iterations;
}

int DataAccess::getNumberOfIterations()
{
	return pbkdf2.iterations;
}
