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

#include "GenericXMLHandler.h"

using namespace std;

#ifndef DEBUG
#define DEBUG 0
#endif

GenericXMLHandler::GenericXMLHandler() {
	
	// create initial element (root)
	ElementInfo* el = new ElementInfo();
	el->number = count++;
	el->name = "ROOT";
	el->namespaces = vector< pair<string,string> >();
	infos.push(el);
}

void GenericXMLHandler::startPrefixMapping
(   	
	const XMLCh *const    	 prefix,
	const XMLCh *const   	uri)
{
#if DEBUG
		cerr << "startPrefixMapping" << endl;
#endif
	
	string strPrefix = XMLString::transcode(prefix);
		
	if (strPrefix.length() == 0) {
		strPrefix = "xmlns";
	}

	string strUri = XMLString::transcode(uri);
	ns.push_back( pair<string, string>(strPrefix, strUri) );	
}

void GenericXMLHandler::comment
(   	
	const XMLCh *const chars,
	const unsigned int length)
{
	cs.push_back(XMLString::transcode(chars));
}

void GenericXMLHandler::startElement
(
		const XMLCh* const uri,
		const XMLCh* const localname,
		const XMLCh* const qname,
		const Attributes& attrs)
{
	ElementInfo* el = new ElementInfo();
	el->name = XMLString::transcode(localname);
	el->number = count++;

	// attach namespace information
	if (!ns.empty()) {
		el->namespaces = ns;
		ns.clear();
	}
	
	// attach comments
	if (!cs.empty()) {
		el->comments = cs;
		cs.clear();
	}
	
	// attrs
	string strQName;
	string strValue;

	for (unsigned int i = 0; i < attrs.getLength(); i++) {
		strQName =  XMLString::transcode(attrs.getQName(i));
		strValue = XMLString::transcode(attrs.getValue(i));
		el->attrs.push_back( pair<string, string>(strQName, strValue) );
	}
	infos.push(el);
}

void GenericXMLHandler::characters
(		
		const   XMLCh* const chars,
		const   unsigned int length) 
{
	if (infos.size() > 0) {
		ElementInfo* eInfo = infos.top();
		// make sure to append all contents
		string s = trim( XMLString::transcode(chars) );
		if (s.length() > 0) {
			eInfo->values.push_back(s);			
		}
	}	
}		

void GenericXMLHandler::endElement
(
	const XMLCh* const uri,
	const XMLCh* const localname,
	const XMLCh* const qname) 
{	
	// get top from element stack
	ElementInfo* info = infos.top();

	if (info != NULL) {
		infos.pop();
	
		// instantiate 
		octave_value* value = buildOctaveValue(info);
		//add to oct_value stack
		pair<ElementInfo*, octave_value*> p (info, value);
		oct_values.push( p );

		root = value;
	}
}

void GenericXMLHandler::ignorableWhitespace(const XMLCh *const chars,
						 					const unsigned int length)
{
	cout << "WS (" << length << "): " << XMLString::transcode(chars) << endl;
}

octave_value* GenericXMLHandler::buildOctaveValue(const ElementInfo* info)
{
	unsigned int number = info->number;
	
	// struct
	Octave_map map;
	map.assign("ORDER", octave_value( number ));

	if ( info->comments.empty() ) {
		map.assign("COMMENTS", octave_value( "" ));
	} else {
		// Create a cellarray with comments
		octave_value_list comments;
		for (unsigned int i = 0; i < info->comments.size(); i++) {
			comments.append( info->comments[i] );
		}		
		comments.reverse();
		map.assign("COMMENTS", octave_value( Cell( comments ) ));
	} 

	map.assign("NAME", octave_value(info->name) );

	if ( info->namespaces.empty() ) {
		map.assign("NAMESPACES", octave_value( "" ));
	} else {
		// create a struct with namespace elements
		Octave_map namespaces;
		for (unsigned int i = 0; i < info->namespaces.size(); i++) {
			namespaces.assign( info->namespaces[i].first, info->namespaces[i].second );
		}	
	
		map.assign("NAMESPACES", octave_value( namespaces ));
	}
	
	if ( info->attrs.empty() ) {
		map.assign("ATTRS", octave_value( "" ));
	} else {
		// create a struct with attributes
		Octave_map attrs;
		for (unsigned int i = 0; i < info->attrs.size(); i++) {
			attrs.assign( info->attrs[i].first, info->attrs[i].second );
		} 
		map.assign("ATTRS", octave_value( attrs ));
	} 
	
	if ( !info->values.empty() ) {
		map.assign("TEXT", octave_value(info->values[0]));
	}

	if (info->name == "ROOT") {
		pair<ElementInfo*, octave_value*> p = oct_values.top();
		map.assign( p.first->name , octave_value( Cell( *p.second ) ) );
	}
	
	// TODO: yes this is just a quick hack to get it work ;)

	// have cells of structs
	octave_value_list* list = NULL;
	string elementName = "";
	
	if ( (info->name != "ROOT") && (oct_values.size() > 0) ) {
	
		pair<ElementInfo*, octave_value*> p = oct_values.top();
		if (number < p.first->number) {
						
			//Octave_map map;
			while (oct_values.size() > 0 &&  p.first->number > number) {			
				oct_values.pop();
				
				// if a name already exists, convert it to a cell
				if (map.contains (p.first->name) && list == NULL) {
					
					list = new octave_value_list();
					
					elementName = p.first->name;
					
					Cell c = map.contents(p.first->name);					
					Cell innerCell = c.elem(0,0).cell_value();
					
					dim_vector d = innerCell.dims ();
					
					for (int k = 0; k < d.numel(); k++)
					{
						octave_value o_val = innerCell.elem(k);
						list->append(o_val);
					} 
					
					list->append(*p.second);
			
				} else if (map.contains (p.first->name) && list != NULL) {

					list->append(*p.second);
					
				} else {
					map.assign(p.first->name, octave_value( Cell( *p.second ) ) );	
				}
				
				if (oct_values.size() > 0) {
					p = oct_values.top();	
				}
			} // end while
			
			if (elementName != "") {
				list->reverse();
				map.assign(elementName, octave_value( Cell( *list ) ) );
				delete list;
				elementName = "";
				list = NULL;
			}
		}
	}

	return new octave_value(map);
}

