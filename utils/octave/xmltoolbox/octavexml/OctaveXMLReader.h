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

#ifndef OCTAVEXMLREADER_H_
#define OCTAVEXMLREADER_H_

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

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

#include <xercesc/sax/InputSource.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

#include "OctaveHandler.h"
#include "TypesafeXMLHandler.h"
#include "GenericXMLHandler.h"

#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif

XERCES_CPP_NAMESPACE_USE

static const char* DFLT_XSD = "octave.xsd";

/**
 * OctaveXMLReader provides the interface for reading XML data into Octave. Two 
 * XML formats are supported (type safe and generic XML), also reading from 
 * the filesystem directly or parsing from a stream directly are supported. 
 * 
 * The Reader uses the SAX2 eventmodel for parsing, it also supports XML Schema 
 * Validation.
 */
class OctaveXMLReader
	{
	public:
		/**
		 * Constructor
		 */
		OctaveXMLReader();
		/**
		 * Destructor
		 */
		virtual ~OctaveXMLReader();
		/**
		 * Set the XML format to be read
		 */
		void setFormat(XML_FORMAT fmt);
		/**
		 * Returns the XML format which is set
		 */
		XML_FORMAT getFormat();
		/**
		 * Enable/disable XML validation
		 */
		void setValidation(bool flag);
		/**
		 * Check if validation is enabled
		 */
		bool getValidation();
		/**
		 * Read XML from the file system, return the converted data as 
		 * Octave values
		 */
		bool readXML(const string& fileName, octave_value_list& values);
		/**
		 * Read XML from stream, returning the Octave values
		 */
		bool readXML(istringstream& xmlStream, octave_value_list& values);
	private:
		bool validate;
		SAX2XMLReader* parser;
		OctaveHandler* handler;
		XML_FORMAT format;
		void initHandler();
};

#endif /*OCTAVEXMLREADER_H_*/
