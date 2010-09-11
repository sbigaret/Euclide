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

#include <fstream>

#include "octavexml/OctaveXMLReader.h"

static OctaveXMLReader reader;


DEFUN_DLD(xml_load, args, , 
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {v = } xml_load(filename [, format = ('any'|'typesafe')])\n\
\n\
Read XML file and load it into the Octave workspace.\n\
\n\
@end deftypefn") 
{
	int argNum; 
	string fileName;
	string attswitch;
	octave_value_list values;
	bool success = false;
	
	argNum = args.length();

	switch (argNum) {
		case 2:
			fileName = args(0).string_value();
			
			if (fileName.length() < 3) {
				error("Invalid filename");
				return octave_value(false);	
			}
			
			// attswitch flag specified, determine format
			attswitch = args(1).string_value();
			if (attswitch.length() < 0) {
				error("Invalid format (expected: 'any'|'typesafe')");
				return octave_value(false);				
			}
			if (attswitch == "typesafe") {
				reader.setFormat(FMT_TYPESAFE);
				success = reader.readXML(fileName, values);	
			} else if (attswitch == "any") {
				reader.setFormat(FMT_ANY);
				success = reader.readXML(fileName, values);	
			} else {
				error("Invalid format (expected: 'any'|'typesafe')");
				return octave_value(false);
			}
			break;
		case 1:
			fileName = args(0).string_value();
			
			if (fileName.length() < 3) {
				error("Invalid filename");
				return octave_value(false);	
			}
			
			// if no flags specified, use the 'typesafe' format
			reader.setFormat(FMT_TYPESAFE);
			success = reader.readXML(fileName, values);	
			break;
		default:
			cerr << "Usage: xml_load(filename [, format = ('any'|'typesafe')])" << endl;
			return octave_value(false);
			break;
	}
	
	if (success) {		
		return values;
	}
	
	// should not get here
	return octave_value(false);
}

DEFUN_DLD(xml_parse, args, ,
"-*- texinfo -*-\n\
@deftypefn {Loadable Function} {v = } xml_parse(xmlstring [, format = ('any'|'typesafe')])\n\
\n\
Convert XML string to Octave variable/structure.\n\
\n\
@end deftypefn") 
{
	int argNum; 
	string xml;
	string attswitch;
	bool success = false;
	octave_value_list values;
	
	argNum = args.length();

	if (argNum > 0) { 
		xml = args(0).string_value();
	}
	
	if (xml.length() < 0) {
		error("Invalid XML string");
		return octave_value(false);	
	}

	istringstream stream(xml, istringstream::in);
	
	switch (argNum) {
		case 2:
			// attswitch flag specified, determine format
			attswitch = args(1).string_value();
			if (attswitch.length() < 0) {
				error("Invalid format (expected: 'any'|'typesafe')");
				return octave_value(false);				
			}
			if (attswitch == "typesafe") {
				reader.setFormat(FMT_TYPESAFE);
				success = reader.readXML(stream, values);	
			} else if (attswitch == "any") {
				reader.setFormat(FMT_ANY);
				success = reader.readXML(stream, values);
			} else {
				error("Invalid format (expected: 'any'|'typesafe')");
				return octave_value(false);
			}
			break;
		case 1:		
			// if no flags specified, use the 'typesafe' format
			reader.setFormat(FMT_TYPESAFE);
			success = reader.readXML(stream, values);
			break;
		default:
			cerr << "Usage: xml_parse(xmlstring [, format = ('any'|'typesafe')])" << endl;	
			return octave_value(false);
			break;
	}

	if (success) {
		return values;
	}
	
	// should not get here
	return octave_value(false);
}

