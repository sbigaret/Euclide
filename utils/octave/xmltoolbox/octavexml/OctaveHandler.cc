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

#include <iostream>
#include <string>

#include <sstream>

#include "OctaveHandler.h"

OctaveHandler::OctaveHandler() 
{
	count = 0;
	errors = 0;
	report = true;
};

OctaveHandler::~OctaveHandler() {
	delete root;
};

void OctaveHandler::warning(const SAXParseException& ex) 
{
	if (report) {
		char* msg = XMLString::transcode(ex.getMessage());
		cerr << "warning: " << msg << " at line: " << ex.getLineNumber() << endl;
	}
}

void OctaveHandler::error(const SAXParseException& ex) 
{
	if (report) {
		char* msg = XMLString::transcode(ex.getMessage());
		cerr << "error: " << msg << " at line: " << ex.getLineNumber() << endl;
	} 
	errors++;
}

void OctaveHandler::fatalError(const SAXParseException& ex) 
{
	if (report) {
		char* msg = XMLString::transcode(ex.getMessage());
		cerr << "fatalError: " << msg << " at line: " << ex.getLineNumber() << endl;
	}
	errors++;
}

octave_value* OctaveHandler::getRoot() 
{
	if (!root) {
		cerr << "Could not parse XML. Are you sure its well-formed (or validates against octave.xsd)?" << endl;
		return new octave_value(false);	
	}

	return root;
}	

int OctaveHandler::getNrOfErrors() {
	return errors;
}

void OctaveHandler::reportErrors(bool flag) {
	report = flag;
}

// workaround for failing attrs.getValue(char*)
string OctaveHandler::extractAttribute(const Attributes& attrs, const string& name) 
{
	string result;
	for (unsigned int i = 0; i < attrs.getLength(); i++) {
		string attribute = XMLString::transcode(attrs.getLocalName(i));
		if (strcmp( attribute.c_str(), name.c_str() ) == 0) {
			return XMLString::transcode(attrs.getValue(i));
		} 
	}
	return result;
}

string OctaveHandler::trim(const string& s) {
	if(s.length() == 0)
		return s;
	int b = s.find_first_not_of(" \t\n");
	int e = s.find_last_not_of(" \t\n");
	if(b == -1) // No non-spaces
		return "";
	if (b == 0 && e ==0) { // Empty string
		return "";
	}
	return string(s, b, e - b + 1);
}
