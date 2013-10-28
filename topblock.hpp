/*
	gr-scan - A GNU Radio signal scanner
	Copyright (C) 2012  Nicholas Tomlinson
	
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/


#include <cmath>
#include <stdint.h>

#include <gnuradio/top_block.h>
#include <osmosdr/source.h>
#include <gnuradio/blocks/stream_to_vector.h>
#include <gnuradio/fft/fft_vcc.h>
#include <gnuradio/blocks/complex_to_mag_squared.h>
#include <gnuradio/filter/single_pole_iir_filter_ff.h>
#include <gnuradio/blocks/nlog10_ff.h>
#include "scanner_sink.hpp"

class TopBlock : public gr::top_block
{
	public:
		TopBlock(double centre_freq_1, double centre_freq_2, double sample_rate, double fft_width, double bandwidth1, double bandwidth2,
				double step, unsigned int avg_size, double spread, double threshold, double ptime) : gr::top_block("Top Block"),
			vector_length(sample_rate/fft_width),
			window(GetWindow(vector_length)),
			
			source(osmosdr::source::make()), /* OsmoSDR Source */
			stv(gr::blocks::stream_to_vector::make(sizeof(float)*2, vector_length)), /* Stream to vector */
			/* Based on the logpwrfft (a block implemented in python) */
			fft(gr::fft::fft_vcc::make(vector_length, true, window, false, 1)),
			ctf(gr::blocks::complex_to_mag_squared::make(vector_length)),
			iir(gr::filter::single_pole_iir_filter_ff::make(1.0, vector_length)),
			lg(gr::blocks::nlog10_ff::make(10, vector_length, -20 * std::log10(float(vector_length)) -10 * std::log10(float(GetWindowPower()/vector_length)))),
			/* Sink - this does most of the interesting work */
			sink(make_scanner_sink(source, vector_length, centre_freq_1, centre_freq_2, sample_rate, bandwidth1, bandwidth2, step, avg_size, spread, threshold, ptime))
		{
			/* Set up the OsmoSDR Source */
			source->set_sample_rate(sample_rate);
			source->set_center_freq(centre_freq_1);
			source->set_freq_corr(0.0);
			source->set_gain_mode(false);
			source->set_gain(10.0);
			source->set_if_gain(20.0);
			
			/* Set up the connections */
			connect(source, 0, stv, 0);
			connect(stv, 0, fft, 0);
			connect(fft, 0, ctf, 0);
			connect(ctf, 0, iir, 0);
			connect(iir, 0, lg, 0);
			connect(lg, 0, sink, 0);
		}
		
	private:
		/* http://en.wikipedia.org/w/index.php?title=Window_function&oldid=508445914 */
		std::vector<float> GetWindow(size_t n)
		{
			std::vector<float> w;
			w.resize(n);
			
			double a = 0.16;
			double a0 = (1.0 - a)/2.0;
			double a1 = 0.5;
			double a2 = a/2.0;
			
			for (unsigned int i = 0; i < n; i++){
				w[i] = a0 - a1 * ::cos((2.0 * 3.14159 * (double)i)/(double)(n - 1)) + a2 * ::cos((4.0 * 3.14159 * (double)i)/(double)(n - 1));
			}
			return w;
		}
		
		double GetWindowPower()
		{
			double total = 0.0;
			BOOST_FOREACH (double d, window){
				total += d*d;
			}
			return total;
		}
		
		size_t vector_length;
		std::vector<float> window;
		
		osmosdr::source::sptr source;
		gr::blocks::stream_to_vector::sptr stv;
		gr::fft::fft_vcc::sptr fft;
		gr::blocks::complex_to_mag_squared::sptr ctf;
		gr::filter::single_pole_iir_filter_ff::sptr iir;
		gr::blocks::nlog10_ff::sptr lg;
		scanner_sink_sptr sink;
};
