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

#ifndef TYPESAFEXMLHANDLER_H_
#define TYPESAFEXMLHANDLER_H_

#include "OctaveHandler.h"

static const char* C_TYPE = "type";
static const char* C_SIZE = "size";

/**
 * TypesafeXMLHandler is a SAX2 XML Handler which provides the capability 
 * of parsing the Octave type safe XML format into a tree of Octave variables.
 */
class TypesafeXMLHandler: public OctaveHandler
{
	public:
		TypesafeXMLHandler();
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
};

#endif /*TYPESAFEXMLHANDLER_H_*/
