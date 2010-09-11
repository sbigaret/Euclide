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
#ifndef OCTAVEHANDLER_
#define OCTAVEHANDLER_

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/Attributes.hpp>

// octave imports
#include <octave/oct.h>
#include <octave/oct-map.h>
#include <octave/ov-struct.h>
#include <octave/ov-cell.h>
#include <octave/lo-utils.h>
#include <octave/oct-obj.h>
#include <octave/ov-base.h>
#include <octave/str-vec.h>

#include <stack>
#include <map>
#include <sstream>

using namespace std;

XERCES_CPP_NAMESPACE_USE;

static const char* C_STRUCT = "struct";
static const char* C_CELL = "cell";
static const char* C_CHAR = "char";
static const char* C_DOUBLE = "double";
static const char* C_COMPLEX = "complex";

/**
 * The ElementInfo structure is a temporary helper object used during the 
 * parsing of XML data.   
 */
struct ElementInfo {
	public:
	    unsigned int number;
	    string  name;
		string  type;
    	dim_vector vec;
    	unsigned int    count;
	    vector<string> values;
	    stringstream value;
	    vector< pair<string, string> > attrs;
	    vector< pair<string, string> > namespaces;
	    vector<string> comments;
};

/**
 * OctaveHandler is the abstract base class used to implement Octave SAX2 Handlers.
 * 
 * The handler contains two stacks, called 'infos' and 'oct_values'. During parsing 
 * of an XML element the data read is stored in ElementInfo objects temporarily, 
 * which are put on the 'infos' stack. 
 * At the end of the element, this stack is processed and the elements are 
 * converted to Octave data types which are put onto the 'oct_values' stack. 
 */
class OctaveHandler: public DefaultHandler 
{
	public:
	    OctaveHandler();
	    virtual ~OctaveHandler();
	    virtual void startElement(
			const XMLCh* const uri,
			const XMLCh* const localname,
			const XMLCh* const qname,
			const Attributes& attrs
		) = 0;
		virtual void endElement(
			const XMLCh* const uri,
			const XMLCh* const localname,
			const XMLCh* const qname
		) = 0;
		virtual void characters(const   XMLCh* const chars,
                        const   unsigned int length
        ) = 0;
        void warning(const SAXParseException&);
        void error(const SAXParseException&);
        void fatalError(const SAXParseException&);
		virtual octave_value* getRoot();
		int getNrOfErrors();
		void reportErrors(bool flag);
	protected:
		bool report;
		unsigned int errors;
	    unsigned int count;
		octave_value* root;
		stack<ElementInfo*> infos;
		stack<pair <ElementInfo*, octave_value*> > oct_values;
		string extractAttribute(const Attributes& attrs, const string& name);
		string trim(const string& s);		
};

#endif /*OCTAVEHANDLER_*/

