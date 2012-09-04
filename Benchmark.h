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

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <QThread>

class Benchmark : public QThread
{
	Q_OBJECT;

	public:
		Benchmark(QObject *parent = 0);
		void run();
		double getTime();
		void setNumberOfIterations(int iterations);
	private:
		double time;
		int iterations;
};

#endif //BENCHMARK_H
