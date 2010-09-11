/*

Copyright (C) 2007 Thomas Geiger (tom.geiger@gmail.com)

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.

Octave is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, write to the Free
Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

*/

#include <octave/oct.h>

using namespace std;

DEFUN_DLD(xml_help, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} xml_help\n\
\n\
Prints help information for XMLtoolbox functions.\n\
\n\
@end deftypefn") 
{
	cout << "xml_help:" << endl;
	cout << endl;
	cout << "Octave XMLtoolbox 0.4.9" << endl; 
	cout << endl;
	cout << " - xml_load - Read XML file and load it into the Octave workspace." << endl;
	cout << " - xml_save - Convert a workspace variable/structure to XML saving it to a file." << endl;
	cout << " - xml_parse - Convert XML string to Octave variable/structure." << endl;
	cout << " - xml_format - Convert Octave variable/structure to XML string representation." << endl;
	cout << endl;
	
	return octave_value(true);	 
}
