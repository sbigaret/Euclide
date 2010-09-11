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

#ifndef GENERICXMLHANDLER_H_
#define GENERICXMLHANDLER_H_

#include "OctaveHandler.h"

/**
 * GenericXMLHandler is the SAX2 XML Handler used to parse _any_ XML format 
 * in order to represent it as a Octave tree of structure and string values.
 */
class GenericXMLHandler: public OctaveHandler
{
	public:	
		GenericXMLHandler();
		void comment(
		   	const XMLCh *const chars,
			const unsigned int length
		);
		void startPrefixMapping(
			const XMLCh *const prefix,
			const XMLCh *const uri
		);
	    void startElement(
			const XMLCh* const uri,
			const XMLCh* const localname,
			const XMLCh* const qname,
			const Attributes& attrs
		);
		void endElement(
			const XMLCh* const uri,
			const XMLCh* const localname,
			const XMLCh* const qname
		);
		void characters(const   XMLCh* const chars,
	                    const   unsigned int length
	    );
		void ignorableWhitespace(const XMLCh *const chars,
								 const unsigned int length
		);
	private:
		octave_value* buildOctaveValue(const ElementInfo* info);
		// store namespace info
		vector< pair<string, string> > ns;
		// comments
		vector< string > cs;
};

#endif /*GENERICXMLHANDLER_H_*/
