/* -*- c++ -*- */
/*
 * Copyright 2010 Communications Engineering Lab, KIT
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <armadillo>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <boost/test/unit_test.hpp>
#include <gnuradio/gr_complex.h>
#include <specest_music_armadillo_impl.h>

class music_armadillo_impl_test
{
 public:
	music_armadillo_impl_test();
	~music_armadillo_impl_test();

	gr_complexd* testdata;
	unsigned int testdata_len;
	double* testomegas;
	unsigned int testomegas_len;
};

music_armadillo_impl_test::music_armadillo_impl_test()
{
	#include "test_data_omegas.h"
	testomegas_len = sizeof(realomegas)/sizeof(double);
	testomegas = new double[testomegas_len];
	testdata_len = sizeof(data)/sizeof(gr_complexd);
	testdata = new gr_complexd[testdata_len];

	for (int i = 0; i < testomegas_len; i++)
	{
		testomegas[i] = realomegas[i];
	}
	for (int i = 0; i < testdata_len; i++)
	{
		testdata[i] = data[i];
	}

}

music_armadillo_impl_test::~music_armadillo_impl_test()
{
	delete testomegas;
	delete testdata;
}


BOOST_AUTO_TEST_CASE(qa_specest_music_armadillo_impl_test_throw)
{
	BOOST_CHECK_THROW(specest_music_impl *music = new specest_music_armadillo_impl(5, 1), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE(qa_specest_music_armadillo_impl_calculate)
{
	music_armadillo_impl_test tester;
	specest_music_impl* music = new specest_music_armadillo_impl(tester.testomegas_len, 80);
	double omegas[tester.testomegas_len];

	music->calculate(tester.testdata, tester.testdata_len, omegas);

	std::sort(omegas, omegas+tester.testomegas_len);
	for(int i = 0; i < tester.testomegas_len; i++)
	{
		BOOST_REQUIRE_CLOSE(omegas[i], tester.testomegas[i], 1);
	}
	delete music;
}


#if 0
BOOST_AUTO_TEST_CASE(qa_specest_music_armadillo_impl_calculate_pspectrum)
{
	const int pspectrum_len = 1024;
	double pspectrum[pspectrum_len];
	music_armadillo_impl_test tester;
	std::ofstream out("qa_specest_music_armadillo_impl.dat");
	if(!out)
		printf("Couldn't open file!");

	specest_music_impl* music = new specest_music_armadillo_impl(tester.testomegas_len ,80);
	music->calculate_pseudospectrum(tester.testdata, tester.testdata_len, pspectrum, pspectrum_len);

	for(int i = 0; i < pspectrum_len; i++)
		out << pspectrum[i] << " ";
	out.close();
	delete music;
}
#endif


#if 0
BOOST_AUTO_TEST_CASE(qa_specest_music_armadillo_impl_bench)
{
	const int pspectrum_len = 1024;
	const int n = 6;
	double omegas[n];
	double pspectrum[pspectrum_len];
	int iterations = 10;
	timespec start, end;
	long long delta_calc;
	long long delta_calc_pspec;
	music_armadillo_impl_test tester;
	std::ofstream out("qa_specest_music_armadillo_bench_impl.dat");
	for(int m = 10; m <= 100; m++)
	{
		specest_music_armadillo_impl* music = new specest_music_armadillo_impl(n, m);
		printf("Starting armadillo MUSIC benchmark with m=%u, ", m);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
		for(int i = 0; i < iterations; i++)
		{
			music->calculate(tester.testdata, tester.testdata_len, omegas);
		}
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
		delta_calc = (end.tv_sec - start.tv_sec)*1000000000LL + end.tv_nsec - start.tv_nsec;
		printf("took %llu ns\n", delta_calc);
		printf("Starting armadillo spectrum MUSIC benchmark with %u, ", m);
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
		for(int i = 0; i < iterations; i++)
		{
			music->calculate_pseudospectrum(tester.testdata, tester.testdata_len, pspectrum, pspectrum_len);
		}
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
		delta_calc_pspec = (end.tv_sec - start.tv_sec)*1000000000LL + end.tv_nsec - start.tv_nsec;
		printf("took  %llu ns\n", delta_calc_pspec);
		out << m << " " << iterations << " " << delta_calc << " " << delta_calc_pspec << std::endl;
		delete music;
	}
}
#endif

