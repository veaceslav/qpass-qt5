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

#include "Benchmark.h"

#include <cstdio>
#include <ctime>
#include <gcrypt.h>

using namespace std;

Benchmark::Benchmark(QObject *parent) : QThread(parent)
{
}

void Benchmark::run()
{
	gcry_check_version(NULL);
	gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
	gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);

	QString password("some password");
	char salt[8];
	char key[32];

	clock_t before = clock();
	gcry_kdf_derive(password.toUtf8().data(), password.toUtf8().size(), GCRY_KDF_PBKDF2, GCRY_MD_SHA256, salt, sizeof(salt), iterations, 32, key);
	time = double(clock()-before)/CLOCKS_PER_SEC;
}

double Benchmark::getTime()
{
	return time;
}

void Benchmark::setNumberOfIterations(int iterations)
{
	this->iterations = iterations;
}
