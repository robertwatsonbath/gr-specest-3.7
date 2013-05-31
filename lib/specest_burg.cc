/*
 * Copyright 2009 Institut fuer Nachrichtentechnik / Uni Karlsruhe
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>
#include <cmath>
#include <gnuradio/io_signature.h>
#include <specest_burg.h>

using std::vector;


// I'd prefer doing this in the specest constructor, but for some reason
// throwing exceptions in the constructor ends up in segfaults in the Python
// domain
inline void
specest_check_arguments_impl(unsigned fft_len, unsigned num_samples, unsigned order)
{
	if (fft_len < 2) {
		throw std::invalid_argument("specest_burg: The length of FFT window should be greater than or equal to 2.");
	}
	if (num_samples < 2) {
		throw std::invalid_argument("specest_burg: The number of samples should be greater than or equal to 2.");
	}
	if (order < 2) {
		throw std::invalid_argument("specest_burg: The order of AR filter should be greater than or equal to 2.");
	}
}


specest_burg_sptr
specest_make_burg(unsigned int block_len,
					unsigned int fft_len,
					unsigned int order,
					bool fftshift,
					int decimation)
{
	specest_check_arguments_impl(fft_len, block_len, order);
	return gnuradio::get_initial_sptr(new specest_burg(
				block_len,
				fft_len,
				order,
				fftshift,
				decimation));
}


specest_burg::specest_burg(unsigned block_len,
							unsigned fft_len,
							unsigned order,
							bool fftshift,
							int decimation)
	: gr::hier_block2("burg",
			gr::io_signature::make(1, 1, sizeof(gr_complex)),
			gr::io_signature::make(1, 1, sizeof(float)*fft_len)),
	d_stream_to_vector(specest_make_stream_to_vector_overlap(sizeof(gr_complex), block_len, 0)),
	d_keep_one_in_n(gr::blocks::keep_one_in_n::make(sizeof(gr_complex) * block_len, decimation)),
	d_arburg(specest_make_arburg_vcc(block_len, order, fft_len)),
	d_pad_vector(specest_make_pad_vector(sizeof(gr_complex), order+1, fft_len)),
	d_fft(gr::fft::fft_vcc::make(fft_len, true, gr::filter::firdes::window(gr::filter::firdes::WIN_RECTANGULAR, fft_len, 1), fftshift)),
	d_mag_square(gr::blocks::complex_to_mag_squared::make(fft_len)),
	d_divide(specest_make_reciprocal_ff(fft_len))
{
	connect(self(), 0, d_stream_to_vector, 0);
	connect(d_stream_to_vector, 0, d_keep_one_in_n, 0);
	connect(d_keep_one_in_n, 0, d_arburg, 0);
	connect(d_arburg, 0, d_pad_vector, 0);
	connect(d_pad_vector, 0, d_fft, 0);
	connect(d_fft, 0, d_mag_square, 0);
	connect(d_mag_square, 0, d_divide, 0);
	connect(d_divide, 0, self(), 0);
}


specest_burg::~specest_burg()
{};


void
specest_burg::set_decimation(int n)
{
	d_keep_one_in_n->set_n(n);
}

