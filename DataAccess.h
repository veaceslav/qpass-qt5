/***************************************************************************
 *   Copyright (c) 2011 Mateusz Piękos <mateuszpiekos@gmail.com>           *
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
       * Returns stored decrypted data list.
       *
       * @return Decrypted data.
       */
      QList< QVector< QString> > read();
      /*! Writes data to encrypted file.
       *
       * This function encrypts data and writes it to a file.
       *
       * @param data Data to write.
       * @return True if operation was successful, otherwise false.
       */
      bool write(const QList< QVector< QString> > &data);
      /*! Checks if given file is database and password is correct.
       *
       * @return 0 if header of database is ok and password is correct, -1 if password is incorrect, -2 if file is corrupted.
       */
      int checkDatabase();
      /*! Changes password used to encryption and decryption database.
       *
       * @param password New password.
       */
      void setPassword(const QString &password);
      QString getPassword();
   private:
      QString path; /**< Path to database. */
      QString password; 
      QFile *file;
      gcry_cipher_handle *hd;
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

#endif //DATAACCESS_H