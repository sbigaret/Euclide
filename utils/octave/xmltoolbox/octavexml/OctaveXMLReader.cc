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

#include "OctaveXMLReader.h"

static bool checkForErrors(XML_FORMAT fmt, int errors);


OctaveXMLReader::OctaveXMLReader()
{
	// initialize XERCES Platform
	try {
		XMLPlatformUtils::Initialize();
	} catch (const XMLException& ex) {
		char* msg = XMLString::transcode(ex.getMessage());
		cerr << "Error initializing Xerces: \n" << msg << "\n";
		XMLString::release(&msg);
	}
	
	validate = false;
	format = FMT_ANY;	
	parser = NULL;
	handler = NULL;
}

OctaveXMLReader::~OctaveXMLReader()
{	
	XMLPlatformUtils::Terminate();
}

bool OctaveXMLReader::readXML(const string& fileName, octave_value_list& values) {
	
	parser = XMLReaderFactory::createXMLReader();
	initHandler();

	if (handler == NULL) {
		cout << "Handler is null" << endl;
		return false;
	}

	try {
		parser->parse(fileName.c_str());
	}
	/*
    catch (const OutOfMemoryException& e) {
        error("OutOfMemoryException");
        return 1;         
    } */
	catch (const SAXParseException& ex) {
		char* msg = XMLString::transcode(ex.getMessage());
		error(msg);
		XMLString::release(&msg);
		return false;
	}	
	catch (const XMLException& ex) {
		char* msg = XMLString::transcode(ex.getMessage());
		error(msg);
		XMLString::release(&msg);		

		return false;
	}
	catch (...) {
		error("Exception while parsing");
		return false;
	}

	if (validate && !checkForErrors(format, handler->getNrOfErrors() ) ) {
		return false;
	}
		
	values = octave_value(*handler->getRoot());
	
	return true;
}

bool OctaveXMLReader::readXML(istringstream& xmlStream, octave_value_list& values) {
	
	// determine stream size
	xmlStream.seekg(0, ios::end);
	int length = xmlStream.tellg();
	xmlStream.seekg(0, ios::beg);

	MemBufInputSource* memBufIS = new MemBufInputSource
	(
			(const XMLByte*)xmlStream.str().c_str()
			, length
			, "streamBuffer"
			, false
	);
	
	parser = XMLReaderFactory::createXMLReader();

	initHandler();
	if (handler == NULL) {
		cout << "Handler is null" << endl;
		return false;
	}

	parser->setContentHandler(handler);
	parser->setErrorHandler(handler);

	try {
		parser->parse(*memBufIS);
	}
	/*
    catch (const OutOfMemoryException& e) {
        error("OutOfMemoryException");
        delete memBufIS;
        return 1;         
    } */
	catch (const SAXParseException& ex) {
		char* msg = XMLString::transcode(ex.getMessage());
		error(msg);
		XMLString::release(&msg);
		delete memBufIS;
		return false;
	}	
	catch (const XMLException& ex) {
		char* msg = XMLString::transcode(ex.getMessage());
		error(msg);
		XMLString::release(&msg);
		delete memBufIS;
		return false;
	}
	catch (...) {
		error("Exception while parsing");
		delete memBufIS;
		return false;
	}

	if (validate && !checkForErrors(format, handler->getNrOfErrors() ) ) {
		delete memBufIS;
		return false;
	}
	
	values = octave_value(*handler->getRoot());
	delete memBufIS;
	return true;
}

void OctaveXMLReader::setFormat(XML_FORMAT fmt) {
	format = fmt;
}

XML_FORMAT OctaveXMLReader::getFormat() {
	return format;
}

void OctaveXMLReader::setValidation(bool flag) {
	validate = flag;
}

bool OctaveXMLReader::getValidation() {
	return validate;
}

void OctaveXMLReader::initHandler() {

	XMLCh* propertyValue = XMLString::transcode(DFLT_XSD);
	ArrayJanitor<XMLCh> janValue(propertyValue);
	
	switch (format) {
		case FMT_ANY:
			handler = new GenericXMLHandler();

			break;
		case FMT_TYPESAFE:
			handler = new TypesafeXMLHandler();
			
			if (validate) {
				parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
		
				parser->setFeature(XMLUni::fgXercesDynamic, false);
				parser->setFeature(XMLUni::fgXercesSchema, true);
				parser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);
		
				parser->setProperty(
						XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation,
						propertyValue);
			}
			break;
		default:	
			handler = new GenericXMLHandler();
									
			break;
	}
	
	if (!validate) {
		handler->reportErrors(false);
	}
	
	parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
	
	parser->setContentHandler(handler);
	parser->setErrorHandler(handler);
	parser->setEntityResolver(handler);
	parser->setLexicalHandler(handler);
}

bool checkForErrors(XML_FORMAT fmt, int errors) {
	if (errors > 0) {
		cout << "Encountered : " << errors << " error(s)."<< endl;
		if (fmt == FMT_TYPESAFE) {
			cout << "Are you sure its a validating Octave typesafe XML?" << endl;
		} else {
			cout << "Are you sure its a well-formed XML?" << endl;
		}
		return false;
	}
	return true;
}
