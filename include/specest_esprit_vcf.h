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

#ifndef INCLUDED_SPECEST_ESPRIT_VCF_H
#define INCLUDED_SPECEST_ESPRIT_VCF_H

#include <specest_api.h>
#include <gnuradio/sync_block.h>
#include <specest_esprit_impl.h>

class specest_esprit_vcf;
typedef boost::shared_ptr<specest_esprit_vcf> specest_esprit_vcf_sptr;

SPECEST_API specest_esprit_vcf_sptr specest_make_esprit_vcf (unsigned int n, unsigned int m, unsigned int nsamples, bool is_doa = false);

/*!
 * \brief
 *
 */
class SPECEST_API specest_esprit_vcf : public gr::sync_block
{
	friend SPECEST_API specest_esprit_vcf_sptr specest_make_esprit_vcf (unsigned int n, unsigned int m, unsigned int nsamples, bool is_doa);

	specest_esprit_vcf (unsigned int n, unsigned int m, unsigned int nsamples, bool is_doa);

 public:
	~specest_esprit_vcf ();

	int work (int noutput_items,
	          gr_vector_const_void_star &input_items,
	          gr_vector_void_star &output_items);

	unsigned int d_m;
	unsigned int d_n;
	unsigned int d_nsamples;
	specest_esprit_impl* d_impl;

	std::vector<gr_complexd> d_in_buf;
	std::vector<double> d_out_buf;
};

#endif /* INCLUDED_SPECEST_ESPRIT_VCF_H */

