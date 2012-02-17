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

#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QList>
#include <QVector>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QObject>
#include <gcrypt.h>

struct header
{
	char id[3]; //PAS
	int version; //2
	int entriescount;
	char IV[16];
};

struct cryptedHeader
{
	char id[3]; //PAS
	char space[13];
};

struct pbkdf2Header
{
	char salt[8];
	int iterations;
	char space[4];
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

enum errorCode
{
	SUCCESS = 0,
	INVALID_PASSWORD = -1,
	GCRYPT_ERROR = -2,
	FILE_ERROR = -3
};

/*! 
 * This class provides a access to encrypted data.
 */
class DataAccess : public QObject
{
	public:
		/*! Initializes class.
		*
		* Constructs a new object to represent the file with the given name.
		*
		* @param path Path to database file.
		* @param password Password to database.
		*/
		DataAccess(const QString &path,const QString &password);
		~DataAccess();
		/*! Returns stored data.
		* 
		* Reads entries from encrypted file
		*
		* @param list List to put read data.
		* @return status of operation.
		*/
		errorCode read(QList< QVector< QString> > &list);
		/*! Writes data to encrypted file.
		*
		* This function encrypts data and writes it to a file.
		*
		* @param data Data to write.
		* @return True if operation was successful, otherwise false.
		*/
		errorCode write(const QList< QVector< QString> > &data);
		/*! Changes password used to encryption and decryption database.
		*
		* @param password New password.
		*/
		void setPassword(const QString &password);
		/*! Returns password used to encrypt and decrypt.
		* 
		* @return password used to encrypt and decrypt.
		*/
		QString getPassword();
	private:
		QString path; /**< Path to database. */
		QString password; 
		char *key;
		struct pbkdf2Header pbkdf2;
		QFile *file;
		gcry_cipher_handle *hd;
		gcry_error_t err;
};



#endif //DATAACCESS_H
