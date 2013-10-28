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

#include <stdlib.h>
#include <argp.h>

class Arguments
{
	public:
		Arguments(int argc, char **argv) :
			avg_size(1000),
			bandwidth1(25000.0),
			bandwidth2(-1.0),
			spread(50000.0),
			threshold(3.0),
			centre_freq_1(87000000.0),
			centre_freq_2(108000000.0),
			sample_rate(2000000.0),
			fft_width(1000.0),
			step(-1.0),
			ptime(-1.0)
		{
			argp_parse (&argp_i, argc, argv, 0, 0, this);
		}
		
		unsigned int get_avg_size()
		{
			return avg_size;
		}
		
		double get_bandwidth1()
		{
			return bandwidth1;
		}
		
		double get_bandwidth2()
		{
			if (bandwidth2 < 0.0){ //user did not specify a coarse band
				return bandwidth1 * 8.0; //I've found this to be a good choice for the coarse band
			}
			else {
				return bandwidth2; //if the user specified a bandwidth for this, use it
			}
		}
		
		double get_spread()
		{
			return spread;
		}
		
		double get_threshold()
		{
			return threshold;
		}
		
		double get_centre_freq_1()
		{
			return centre_freq_1;
		}
		
		double get_centre_freq_2()
		{
			return centre_freq_2;
		}
		
		double get_sample_rate()
		{
			return sample_rate;
		}
		
		double get_fft_width()
		{
			return fft_width;
		}
		
		double get_step()
		{
			if (step < 0.0){
				return sample_rate/4.0; //I've found this to be a good choice (slightly faster might be /3.0)
			}
			else {
				return step;
			}
		}
		
		double get_time()
		{
			return ptime;
		}
		
	private:
		static error_t s_parse_opt(int key, char *arg, struct argp_state *state)
		{
			Arguments *arguments = (Arguments *)state->input;
			return arguments->parse_opt (key, arg, state);
		}
		
		error_t parse_opt (int key, char *arg, struct argp_state *state)
		{
			switch (key)
			{
				case 'a':
					avg_size = atoi(arg);
					break;
				case 'f':
					bandwidth1 = atof(arg) * 1000.0; //kHz
					break;
				case 'c':
					bandwidth2 = atof(arg) * 1000.0; //kHz
					break;
				case 's':
					spread = atof(arg) * 1000.0; //kHz
					break;
				case 't':
					threshold = atof(arg);
					break;
				case 'x':
					centre_freq_1 = atof(arg) * 1000000.0; //MHz
					break;
				case 'y':
					centre_freq_2 = atof(arg) * 1000000.0; //MHz
					break;
				case 'r':
					sample_rate = atof(arg) * 1000000.0; //MSamples/s
					break;
				case 'w':
					fft_width = atoi(arg);
					break;
				case 'z':
					step = atof(arg) * 1000000.0; //MHz
					break;
				case 'p':
					ptime = atof(arg);
					break;
				case ARGP_KEY_ARG:
					if (state->arg_num > 0){
						argp_usage (state);
					}
					break;
				case ARGP_KEY_END:
					if (state->arg_num < 0){
						argp_usage(state);
					}
					break;
				default:
					return ARGP_ERR_UNKNOWN;
			}
			return 0;
		}
		
		static argp_option options[];
		static argp argp_i;
		
		unsigned int avg_size;
		double bandwidth1;
		double bandwidth2;
		double spread;
		double threshold;
		double centre_freq_1;
		double centre_freq_2;
		double sample_rate;
		double fft_width;
		double step;
		double ptime;
};

argp_option Arguments::options[] = {
	{"average", 'a', "COUNT", 0, "Average over COUNT samples"},
	{"fine-bandwidth", 'f', "FREQ", 0, "Bandwidth of the fine window in kHz"},
	{"coarse-bandwidth", 'c', "FREQ", 0, "Bandwidth of the coarse window in kHz"},
	{"spread", 's', "FREQ", 0, "Minimum frequency between detected signals"},
	{"threshold", 't', "POWER", 0, "Threshold for the difference between the coarse and fine filtered signals in dB"},
	{"start-frequency", 'x', "FREQ", 0, "Start frequency in MHz"},
	{"end-frequency", 'y', "FREQ", 0, "End frequency in MHz"},
	{"sample-rate", 'r', "RATE", 0, "Samplerate in Msamples/s"},
	{"fft-width", 'w', "COUNT", 0, "Width of FFT in samples"},
	{"step", 'z', "FREQ", 0, "Increment step in MHz"},
	{"time", 'p', "TIME", 0, "Time in seconds to scan on each frequency"},
	{0}
};
argp Arguments::argp_i = {options, s_parse_opt, 0, 0};

const char *argp_program_bug_address = "gr-scan@techmeology.co.uk";
const char *argp_program_version = 
"gr-scan " VERSION " - A GNU Radio signal scanner\n"
"Copyright (C) 2012  Nicholas Tomlinson\n"
"\n"
"This program is free software: you can redistribute it and/or modify\n"
"it under the terms of the GNU General Public License as published by\n"
"the Free Software Foundation, either version 3 of the License, or\n"
"(at your option) any later version.\n"
"\n"
"This program is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"GNU General Public License for more details.\n"
"\n"
"You should have received a copy of the GNU General Public License\n"
"along with this program.  If not, see <http://www.gnu.org/licenses/>.";