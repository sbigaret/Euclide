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


#include "octavexml/octave_xml.h"
#include "octavexml/OctaveXMLWriter.h"

static 	OctaveXMLWriter writer;

DEFUN_DLD(xml_save, args, , 
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {success = } xml_save(filename, v [, format = ('any'|'typesafe')])\n\
\n\
 Convert a workspace variable/structure to XML saving it to a file.\n\
\n\
@end deftypefn")		   
{
	int argNum; 
	string fileName;
	string attswitch;
	bool success = false;
	argNum = args.length();
	
	
	if (argNum == 3) {
		// extract name of target file
		fileName = args(0).string_value();
		if (fileName.length() < 5) {
			error("No filename specificed. Must be at least 5 chars long.");
			return octave_value(false);	
		}
		
		// attswitch flag specified, determine format
		attswitch = args(2).string_value();
		if (attswitch.length() <= 0) {
			error("Invalid attswitch (expected: 'any'|'typesafe')");
			return octave_value(false);				
		}
		if (attswitch == "typesafe") {
			writer.setFormat(FMT_TYPESAFE);
			success = writer.writeXML(args(1), fileName);	
		} else if (attswitch == "any") {
			writer.setFormat(FMT_ANY);
			success = writer.writeXML(args(1), fileName);
		} else {
			error("Invalid attswitch (expected: 'any'|'typesafe')");
			return octave_value(false);
		}
	} else if (argNum == 2) {
		// extract name of target file
		fileName = args(0).string_value();
		if (fileName.length() < 5) {
			error("No filename specificed. Must be at least 5 chars long.");
			return octave_value(false);	
		}
		
		// no attrs specified, default format 'typesafe'
		writer.setFormat(FMT_TYPESAFE);
		success = writer.writeXML(args(1), fileName);
	} else {
		cerr << "Usage: xml_save(filename, v [, format = ('any'|'typesafe')])" << endl;
		return octave_value(false);		
	}
	
	// everything is fine
	return octave_value(success);
}

DEFUN_DLD(xml_format, args,,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {xmlstring = } xml_format(v [, format = ('any'|'typesafe')])\n\
\n\
Convert Octave variable/structure to XML string representation.\n\
\n\
@end deftypefn") 
{
	int argNum; 
	string attswitch;
	bool success = false;
	argNum = args.length();

	ostringstream output;

	switch (argNum) {
		case 2:
			// attswitch flag specified, determine format
			attswitch = args(1).string_value();
			if (attswitch.length() < 0) {
				error("Invalid format (expected: 'any'|'typesafe')");
				return octave_value(false);				
			}
			if (attswitch == "typesafe") {
				writer.setFormat(FMT_TYPESAFE);
				success = writer.writeXML(args(0), output);	
			} else if (attswitch == "any") {
				writer.setFormat(FMT_ANY);
				success = writer.writeXML(args(0), output);
			} else {
				error("Invalid format (expected: 'any'|'typesafe')");
				return octave_value(false);
			}
			break;
		case 1:
			// if no flags specified, use the 'typesafe' format
			writer.setFormat(FMT_TYPESAFE);
			success = writer.writeXML(args(0), output);
			break;
		default:
			cerr << "Usage: xml_format(v [, format = ('any'|'typesafe')])" << endl;
			return octave_value(false);
			break;
	}
	if (!success) {
		return octave_value(false);
	}
	
	return octave_value(output.str());
}
