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

#ifndef OCTAVEXMLWRITER_H_
#define OCTAVEXMLWRITER_H_

#include "octave_xml.h"

#include <octave/oct-map.h>
#include <octave/ov-struct.h>
#include <octave/ov-cell.h>
#include <octave/lo-utils.h>
#include <octave/oct-obj.h>
#include <octave/ov-base.h>
#include <octave/str-vec.h>

#include <cstring>
#include <cctype>

#include <stdexcept>
#include <fstream>

/**
 * OctaveXMLWriter is the interface for the conversion of Octave variables into 
 * an XML format. Two different kinds of formats are supported, a generic XML and 
 * a type safe XML representation.
 * 
 * Writing is done by converting a Octave variable tree recursively, sending 
 * each value to the stream object. 
 */
class OctaveXMLWriter
{
	public:
		/**
		 * Constructor
		 */
		OctaveXMLWriter();
		/**
		 * Destructor
		 */
		virtual ~OctaveXMLWriter();
		/**
		 * Sets the XML format desired
		 */
		void setFormat(XML_FORMAT fmt);
		/**
		 * Returns the format currently set
		 */
		XML_FORMAT getFormat();
		/**
		 * Writes the Octave values into a file.
		 */
		bool writeXML(const octave_value& value, const string& fileName);
		/**
		 * Converts the Octave values into an XML buffer.
		 */		
		bool writeXML(const octave_value& value, ostringstream& output);
	private:
		XML_FORMAT format;
				
};

#endif /*OCTAVEXMLWRITER_H_*/
