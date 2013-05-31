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

#ifndef INCLUDED_SPECEST_CYCLO_FAM_CALCSPECTRUM_VCF_H
#define INCLUDED_SPECEST_CYCLO_FAM_CALCSPECTRUM_VCF_H

#include <specest_api.h>
#include <gnuradio/sync_interpolator.h>
#include <specest_cyclo_fam_calcspectrum_impl.h>

class specest_cyclo_fam_calcspectrum_vcf;
typedef boost::shared_ptr<specest_cyclo_fam_calcspectrum_vcf> specest_cyclo_fam_calcspectrum_vcf_sptr;

SPECEST_API specest_cyclo_fam_calcspectrum_vcf_sptr specest_make_cyclo_fam_calcspectrum_vcf (int Np, int P, int L);

/*!
 * \brief Does the heavy lifting of specest_cyclo_fam.
 *
 */
class SPECEST_API specest_cyclo_fam_calcspectrum_vcf : public gr::sync_interpolator
{
    friend SPECEST_API specest_cyclo_fam_calcspectrum_vcf_sptr specest_make_cyclo_fam_calcspectrum_vcf (int Np, int P, int L);

	specest_cyclo_fam_calcspectrum_vcf (int Np, int P, int L);

    specest_cyclo_fam_calcspectrum_impl *d_calcspectrum;

    int      d_Np;
    int      d_P;
    int      d_N;
    int      d_L;
    unsigned d_p_index;
    int      d_K;            // Interpolation Factor

    float const *d_outbuffer;

 public:
	~specest_cyclo_fam_calcspectrum_vcf ();

	int get_Np(){ return d_Np; };
	int get_N(){ return d_N; };
	int get_P(){ return d_P; };
    int get_L(){ return d_L; };

	int work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items);

    const std::vector<std::vector<float> > &get_estimate(){return d_calcspectrum->get_outputs();};
};

#endif /* INCLUDED_SPECEST_CYCLO_FAM_CALCSPECTRUM_VCF_H */

