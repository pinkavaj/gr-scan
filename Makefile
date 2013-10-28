#
#	gr-scan - A GNU Radio signal scanner
#	Copyright (C) 2012  Nicholas Tomlinson
#	
#	This program is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#	
#	This program is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#	
#	You should have received a copy of the GNU General Public License
#	along with this program.  If not, see <http://www.gnu.org/licenses/>. 
#

VERSION=2013102901
CXXFLAGS=-DVERSION="\"gr-scan $(VERSION)\""  -Wall -lgnuradio-filter -lgnuradio-fft -lgnuradio-runtime -lgnuradio-osmosdr -lboost_system -O2 -s -Wno-unused-function

gr-scan: *.cpp *.hpp Makefile
	g++ $(CXXFLAGS) -o gr-scan main.cpp

clean:
	rm -f gr-scan gr-scan.tar.gz

dist:
	mkdir gr-scan-$(VERSION)
	cp *.cpp *.hpp Makefile COPYING gr-scan-$(VERSION)
	tar -cf - gr-scan-$(VERSION) | gzip -9 -c - > gr-scan-$(VERSION).tar.gz
	rm -r gr-scan-$(VERSION)
