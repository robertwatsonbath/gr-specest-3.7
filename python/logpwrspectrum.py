#
# Copyright 2008 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#

from gnuradio import gr
from gnuradio import blocks
import math
import specest

class _logpwrspectrum_base(gr.hier_block2):
    """
    Create a log10(abs(spectral estimation)) stream chain, with real or complex input.
    """

    def __init__(self, sample_rate, pspectrum_len, ref_scale, frame_rate, avg_alpha, average, n, m, nsamples, estimator = 'esprit'):
        """
        Create an log10(abs(spectrum_estimate)) stream chain.
        Provide access to the setting the filter and sample rate.
        @param sample_rate      Incoming stream sample rate
        @param pspectrum_len    Number of FFT bins
        @param ref_scale        Sets 0 dB value input amplitude
        @param frame_rate       Output frame rate
        @param avg_alpha        averaging (over time) constant [0.0-1.0]
        @param average          Whether to average [True, False]
        @param n                Parameter n for the estimator
        @param m                Parameter m for the estimator
        @param nsamples         Number of samples to use for estimation
        @param estimator        Estimator to use, can be either 'esprit' or 'music'
        """
        gr.hier_block2.__init__(self, self._name,
                                gr.io_signature(1, 1, self._item_size),          # Input signature
                                gr.io_signature(1, 1, gr.sizeof_float*pspectrum_len)) # Output signature

        self._sd = blks2.stream_to_vector_decimator(item_size=self._item_size,
                                              sample_rate=sample_rate,
                                              vec_rate=frame_rate,
                                              vec_len=nsamples)

        if estimator == 'esprit':
            est = specest.esprit_spectrum_vcf(n,m,nsamples,pspectrum_len)

        elif estimator == 'music':
            est = specest.music_spectrum_vcf(n,m,nsamples,pspectrum_len)

        else:
            est = specest.esprit_spectrum_vcf(n,m,nsamples,pspectrum_len)


        self._avg = gr.single_pole_iir_filter_ff(1.0, pspectrum_len)
        self._log = gr.nlog10_ff(20, pspectrum_len,
                                 -20*math.log10(pspectrum_len)              # Adjust for number of bins
                                 -20*math.log10(ref_scale/2)+3.0)      # Adjust for reference scale
        self.connect(self, self._sd, est, self._avg, self._log, self)

        self._average = average
        self._avg_alpha = avg_alpha
        self.set_avg_alpha(avg_alpha)
        self.set_average(average)

    def set_decimation(self, decim):
        """
        Set the decimation on stream decimator.
        @param decim the new decimation
        """
        self._sd.set_decimation(decim)

    def set_vec_rate(self, vec_rate):
        """
        Set the vector rate on stream decimator.
        @param vec_rate the new vector rate
        """
        self._sd.set_vec_rate(vec_rate)

    def set_sample_rate(self, sample_rate):
        """
        Set the new sampling rate
        @param sample_rate the new rate
        """
        self._sd.set_sample_rate(sample_rate)

    def set_average(self, average):
        """
        Set the averaging filter on/off.
        @param average true to set averaging on
        """
        self._average = average
        if self._average:
            self._avg.set_taps(self._avg_alpha)
        else:
            self._avg.set_taps(1.0)

    def set_avg_alpha(self, avg_alpha):
        """
        Set the average alpha and set the taps if average was on.
        @param avg_alpha the new iir filter tap
        """
        self._avg_alpha = avg_alpha
        self.set_average(self._average)

    def sample_rate(self):
        """
        Return the current sample rate.
        """
        return self._sd.sample_rate()

    def decimation(self):
        """
        Return the current decimation.
        """
        return self._sd.decimation()

    def frame_rate(self):
        """
        Return the current frame rate.
        """
        return self._sd.frame_rate()

    def average(self):
        """
        Return whether or not averaging is being performed.
        """
        return self._average

    def avg_alpha(self):
        """
        Return averaging filter constant.
        """
        return self._avg_alpha

class logpwrspectrum_f(_logpwrspectrum_base):
        """
        Create a spectral estimation block chain, with real input.
        """
        _name = "logpwrspectrum_f"
        _item_size = gr.sizeof_float

class logpwrspectrum_c(_logpwrspectrum_base):
        """
        Create a spectral estimation block chain, with complex input.
        """
        _name = "logpwrspectrum_c"
        _item_size = gr.sizeof_gr_complex
