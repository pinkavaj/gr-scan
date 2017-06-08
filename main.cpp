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


#include <cstdio>

#include "arguments.hpp"
#include "topblock.hpp"

int main(int argc, char **argv)
{
	Arguments arguments(argc, argv);
	
	TopBlock top_block(
		arguments.get_centre_freq_1(),
		arguments.get_centre_freq_2(),
		arguments.get_sample_rate(),
		arguments.get_fft_width(),
		arguments.get_bandwidth1(),
		arguments.get_bandwidth2(),
		arguments.get_step(),
		arguments.get_avg_size(),
		arguments.get_spread(),
		arguments.get_threshold(),
		arguments.get_time(),
		arguments.get_device());
	top_block.run();
	return 0; //actually, we never get here because of the rude way in which we end the scan
}
