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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <specest_esprit_vcf.h>
#include <specest_esprit_fortran_impl.h>


specest_esprit_vcf_sptr
specest_make_esprit_vcf (unsigned int n, unsigned int m, unsigned int nsamples, bool is_doa)
{
	return specest_esprit_vcf_sptr (new specest_esprit_vcf (n, m, nsamples, is_doa));
}


specest_esprit_vcf::specest_esprit_vcf (unsigned int n, unsigned int m, unsigned int nsamples, bool is_doa)
	: gr::sync_block ("esprit_vcf",
		gr::io_signature::make (1, 1, nsamples * sizeof (gr_complex)),
		gr::io_signature::make (1, 1, n * sizeof (float))),
	d_n(n),
	d_m(m),
	d_nsamples(nsamples),
	d_impl(new specest_esprit_fortran_impl(n, m, is_doa)),
	d_in_buf(d_nsamples, 0),
	d_out_buf(d_n, 0)
{
}


specest_esprit_vcf::~specest_esprit_vcf ()
{
	delete d_impl;
}


int
specest_esprit_vcf::work (int noutput_items,
                          gr_vector_const_void_star &input_items,
                          gr_vector_void_star &output_items)
{
	const gr_complex* in = static_cast<const gr_complex*>(input_items[0]);
	float* out = static_cast<float*> (output_items[0]);

	for (int item = 0; item < noutput_items; item++) {
		for(int i = 0; i < d_nsamples; i++)
			d_in_buf[i] = static_cast<gr_complexd>(in[i]);

		d_impl->calculate(&d_in_buf[0], d_nsamples, &d_out_buf[0]);

		for(int i = 0; i < d_n; i++)
			out[i] = float(d_out_buf[i]);

		in += d_nsamples;
		out += d_n;
	}

	return noutput_items;
}

