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
#include <time.h>

#include "DataAccess.h"

DataAccess::DataAccess(const QString &path,const QString &password)
{
	this->path = path;
	this->password = password;
	file = new QFile(path);
}

DataAccess::~DataAccess()
{
	delete file;
}

int DataAccess::checkDatabase()
{
	if(!file->open(QIODevice::ReadWrite))
	{
		return -2;
	}
	struct header head;
	file->read((char*)&head, sizeof(header));
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
			strcpy((char*)&keyc, password.toAscii());
			gcry_cipher_setkey(hd, keyc, 16);
			gcry_cipher_setiv(hd, head.IV, 16); 
			struct cryptedHeader chead;
			file->read((char*)&chead, sizeof(cryptedHeader));
			gcry_cipher_decrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
			if(chead.id[0] != 'P' || chead.id[1] != 'A' && chead.id[2] != 'S')
			{
				gcry_cipher_close(hd);
				file->close();
				return -1;
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
			gcry_md_hash_buffer(GCRY_MD_SHA256, (char*)&keyhash, password.toAscii(), password.length());
			gcry_cipher_setkey(hd, (char*)&keyhash, 32);
			gcry_cipher_setiv(hd, head.IV, 16); 
			struct cryptedHeader chead;
			file->read((char*)&chead, sizeof(cryptedHeader));
			gcry_cipher_decrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
			if(chead.id[0] != 'P' || chead.id[1] != 'A' || chead.id[2] != 'S')
			{
				gcry_cipher_close(hd);
				file->close();
				return -1;
			}
			gcry_cipher_close(hd);
		}
	}
	else
	{
		file->close();
		return -2;
	}
	file->close();
	return 0;
}


QList< QVector< QString> > DataAccess::read()
{
	if(!file->open(QIODevice::ReadWrite))
	{
		QMessageBox box;
		box.setWindowTitle( tr("QPass") );
		box.setText( tr("Error opening database") );
		box.setIcon(QMessageBox::Critical);
		box.exec();
		return QList< QVector< QString> >();
	}
	QList< QVector< QString> > list;
	QVector< QString > tempVector(5); //In this version there is 5 columns
	file->seek(0);
	struct header head;
	file->read((char*)&head, sizeof(header));
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
			strcpy((char*)&keyc, password.toAscii());
			gcry_cipher_setkey(hd, keyc, 16);
			gcry_cipher_setiv(hd, head.IV, 16); 
			struct cryptedHeader chead;
			file->read((char*)&chead, sizeof(cryptedHeader));
			gcry_cipher_decrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
			if(chead.id[0] == 'P' && chead.id[1] == 'A' && chead.id[2] == 'S')
			{
				for(int i = 0; i < head.entriescount; i++)
				{    
					struct entryHeader e;
					file->read((char*)&e, sizeof(entryHeader));
					gcry_cipher_decrypt(hd, &e, sizeof(entryHeader), NULL, 0);
					char *entry = (char*)malloc(e.entryLength);
					file->read(entry, e.entryLength);
					gcry_cipher_decrypt(hd, entry, e.entryLength, NULL, 0);
					char temp[500];
					int start = 0;
					memcpy(temp, &entry[start], e.nameLength);
					start += e.nameLength;
					temp[e.nameLength] = 0;
					//templist.append(temp);
					tempVector[0] = temp;
					
					memcpy(temp, &entry[start], e.urlLength);
					start += e.urlLength;
					temp[e.urlLength] = 0;
					tempVector[1] = temp;
					
					memcpy(temp, &entry[start], e.usernameLength);
					start += e.usernameLength;
					temp[e.usernameLength] = 0;
					tempVector[2] = temp;
					
					memcpy(temp, &entry[start], e.passwordLength);
					start += e.passwordLength;
					temp[e.passwordLength] = 0;
					tempVector[3] = temp;
					
					memcpy(temp, &entry[start], e.notesLength);
					start += e.notesLength;
					temp[e.notesLength] = 0;
					tempVector[4] = temp;
					list.append(tempVector);
					free(entry);
				}
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
			gcry_md_hash_buffer(GCRY_MD_SHA256, (char*)&keyhash, password.toAscii(), password.length());
			gcry_cipher_setkey(hd, (char*)&keyhash, 32);
			gcry_cipher_setiv(hd, head.IV, 16); 
			struct cryptedHeader chead;
			file->read((char*)&chead, sizeof(cryptedHeader));
			gcry_cipher_decrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
			if(chead.id[0] == 'P' && chead.id[1] == 'A' && chead.id[2] == 'S')
			{
				for(int i = 0; i < head.entriescount; i++)
				{    
					struct entryHeader e;
					file->read((char*)&e, sizeof(entryHeader));
					gcry_cipher_decrypt(hd, &e, sizeof(entryHeader), NULL, 0);
					char *entry = (char*)malloc(e.entryLength);
					file->read(entry, e.entryLength);
					gcry_cipher_decrypt(hd, entry, e.entryLength, NULL, 0);
					char temp[500];
					int start = 0;
					memcpy(temp, &entry[start], e.nameLength);
					start += e.nameLength;
					temp[e.nameLength] = 0;
					//templist.append(temp);
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
			}
			gcry_cipher_close(hd);
		}
	}
	file->close();
	return list;
}

bool DataAccess::write(const QList< QVector< QString> > &data)
{
	if(!file->open(QIODevice::Truncate | QIODevice::ReadWrite))
		return false;
	struct header head;
	head.id[0] = 'P';
	head.id[1] = 'A';
	head.id[2] = 'S';
	head.version = 2;
	head.entriescount = data.count();
	
	srand(time(0));
	for(int i = 0; i < 16; i++)
		head.IV[i] = rand();
	if( file->write((char*)&head, sizeof(header)) == -1 )
		return false;
	const char *version = gcry_check_version(NULL);
	gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
	gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
	gcry_error_t error = gcry_cipher_open(&hd, GCRY_CIPHER_AES256, GCRY_CIPHER_MODE_CBC, 0);
	char keyhash[32];
	gcry_md_hash_buffer(GCRY_MD_SHA256, (char*)&keyhash, password.toAscii(), password.length());
	gcry_cipher_setkey(hd, (char*)&keyhash, 32);
	gcry_cipher_setiv(hd, head.IV, 16); 
	
	struct cryptedHeader chead;
	chead.id[0] = 'P';
	chead.id[1] = 'A';
	chead.id[2] = 'S';
	gcry_cipher_encrypt(hd, &chead, sizeof(cryptedHeader), NULL, 0);
	if( file->write((char*)&chead, sizeof(cryptedHeader)) == -1 )
		return false;
	for(int i = 0; i < data.count(); i++)
	{
		struct entryHeader e;
		e.entryLength = data[i][0].toUtf8().size()+data[i][1].toUtf8().size()+data[i][2].toUtf8().size()+data[i][3].toUtf8().size()+data[i][4].toUtf8().size();
		e.entryLength = ((e.entryLength/16)+1)*16;
		e.nameLength = data[i][0].toUtf8().size();
		e.urlLength = data[i][1].toUtf8().size();
		e.usernameLength = data[i][2].toUtf8().size();
		e.passwordLength = data[i][3].toUtf8().size();
		e.notesLength = data[i][4].toUtf8().size();
		char *entry = (char*)malloc(e.entryLength);
		int start = 0;
		for(int j = 0; j < 5; j++)
		{
			memcpy(entry+start, data[i][j].toUtf8(), data[i][j].toUtf8().size());
			start += data[i][j].toUtf8().size();
		}
		int length = e.entryLength;
		gcry_cipher_encrypt(hd, &e, sizeof(entryHeader), NULL, 0);
		gcry_cipher_encrypt(hd, entry, length, NULL, 0);
		if( file->write((char*)&e, sizeof(entryHeader)) == -1 )
			return false;
		if( file->write(entry, length) == -1 )
			return false;
		free(entry);
	}
	gcry_cipher_close(hd);
	file->close();
	return true;
}

void DataAccess::setPassword(const QString &password)
{
	this->password = password;
}

QString DataAccess::getPassword()
{
	return this->password;
}
