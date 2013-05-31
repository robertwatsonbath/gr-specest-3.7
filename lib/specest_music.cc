/* -*- c++ -*- */
/*
 * Copyright 2011 Communications Engineering Lab, KIT
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
#include <specest_music.h>


specest_music_sptr
specest_make_music (unsigned int n, unsigned int m, unsigned int nsamples, unsigned int pspectrum_len, unsigned int decimation)
{
	return gnuradio::get_initial_sptr (new specest_music (n, m, nsamples, pspectrum_len, decimation));
}


specest_music::specest_music (unsigned int n, unsigned int m, unsigned int nsamples, unsigned int pspectrum_len, unsigned int decimation)
	: gr::hier_block2 ("music",
		gr::io_signature::make (1, 1, sizeof (gr_complex)),
		gr::io_signature::make (1, 1, pspectrum_len * sizeof (float))),
		d_decimation(decimation),
		d_s2v(gr::blocks::stream_to_vector::make(sizeof(gr_complex),nsamples)),
		d_decimate(gr::blocks::keep_one_in_n::make(nsamples * sizeof(gr_complex), decimation)),
		d_music(specest_make_music_spectrum_vcf(n,m,nsamples,pspectrum_len))

{
	connect(self(), 0, d_s2v, 0);
	connect(d_s2v, 0, d_decimate, 0);
	connect(d_decimate, 0, d_music, 0);
	connect(d_music, 0, self(), 0);
}


specest_music::~specest_music ()
{
}

unsigned int
specest_music::decimation()
{
	return d_decimation;
}

void
specest_music::set_decimation(unsigned int n)
{
	d_decimate->set_n(n);
	d_decimation = n;
}
