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

#include "OctaveXMLWriter.h"

// some prototypes
static void writeAny(ostringstream& xml, const char* parent, const octave_value& val,
		int index, int level = -1);
static void addNamespaces(ostringstream& xml, Octave_map& m);
static void addAttributes(ostringstream& xml, Octave_map& m);
static void addComments(ostringstream& xml, Octave_map& m);
static bool compareOctaveValues(const octave_value a, const octave_value& b);

static string trim(const string& s);

static void writeTypesafe(ostringstream& xml, const char* name,
		const octave_value val, int index, bool attrs, int level = 0);
static string indent(int i, bool typesafe);
static void writeTypeInfo(ostringstream& xml, const octave_value val, int index,
		const char* name);
static void writeDimensions(ostringstream& xml, dim_vector dv);

// constants
static const int C_OUTPUT_PRECISION = 14; // set precision as used in geodise

static const char* C_EMPTY = "";
static const char* C_ATTR = "ATTRS";
static const char* C_COMMENT = "COMMENTS";
static const char* C_NAME = "NAME";
static const char* C_NAMESPACE = "NAMESPACES";
static const char* C_CONTENT = "TEXT";
static const char* C_OP = "<";
static const char* C_CL = ">";
static const char* C_INDENT = "    "; // four spaces
static const char* C_INDENT_TYPESAFE = "  "; // two spaces (for geodise compat.)
static const char* C_ROOT = "root";
static const char* C_DUMMY_ELEM = "ROOT";
static const char* C_ORDER = "ORDER";


OctaveXMLWriter::OctaveXMLWriter()
{
	format = FMT_ANY;
}

OctaveXMLWriter::~OctaveXMLWriter()
{
}

bool OctaveXMLWriter::writeXML(const octave_value& value, ostringstream& output) {
	
	output << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>";

	// evaluate datatype for each arg (recursive)		
	switch (format) {
	case FMT_ANY:
		output << endl;
		writeAny(output, C_ROOT, value, 1);
		return true;
		break;
	case FMT_TYPESAFE:

		output.precision(C_OUTPUT_PRECISION);

		output << endl;
		writeTypesafe(output, C_ROOT, value, 1, true);
		output << endl;
		return true;
		break;
	default:
		break;
	}
	return false;	
}

bool OctaveXMLWriter::writeXML(const octave_value& value, const string& fileName) {
	
	ostringstream output;
	
	// try to create the file
	// TODO: some checking needed here
	ofstream xml(fileName.c_str(), ios::out);	
		
	// now write it
	writeXML(value, output);
	
	// pass to file
	xml << output.str();
	
	xml.close();
	
	return true;
}

void OctaveXMLWriter::setFormat(XML_FORMAT fmt) {
	format = fmt;
}

XML_FORMAT OctaveXMLWriter::getFormat() {
	return format;
}


/*
 * writeAny considers the special names ATTRIBUTE/CONTENT when constructing XML from an octave value
 */
static void writeAny(ostringstream& xml, const char* parent,
		const octave_value& val, int index, int level) {

	string indentStr = indent(level, false);
	
	if (val.is_map()) {
		Octave_map m = val.map_value();

		addComments(xml, m);

		// XXX: if there's no NAME, extract name from current element
				
		// string (mandatory)
		string name = "NO_NAME";
		if ( m.contains(C_NAME) ) {
			name = m.contents(C_NAME).elem(0,0).string_value();
		} else {
			cout << "ERROR: any - no NAME attribute specified!" << endl;
			return;
		}
		
		// okay, name has been extracted
		m.del(C_NAME);

		// this element was of used at the previous level, 
		// here its not eeded any more
		m.del(C_ORDER);

		// lets start with an open tag and the name of the element...
		if (name != C_DUMMY_ELEM) {
			xml << indentStr << C_OP << name;
		}

		addNamespaces(xml, m);
		
		addAttributes(xml, m);

		bool content = false;
		bool empty = true;

		// do we have some content within the element?
		if (m.contains(C_CONTENT)) {

			Cell cell = m.contents(C_CONTENT);
			if (cell.elem(0,0).is_string() && cell.elem(0,0).string_value().length() > 0) {

				// have end tag for the current element
				if (name != C_DUMMY_ELEM) {
					xml << C_CL;
				}

				string text = m.contents(C_CONTENT).elem(0,0).string_value();
				if (trim(text).length() > 0) {
					xml << text;
					empty = false;
					content = true;
				}
			}
			m.del(C_CONTENT);
		}

		// do we have any children left?
		if (m.numel() > 0) {

			if (!content && name != C_DUMMY_ELEM) {
				xml << C_CL;
			}

			empty = false;

			xml << endl;

			// well this part is pretty funky....
			// first we collect all children (cells) and add them to a list,
			// we sort them taking the ORDER tag into account 
			// so we can restore the order of the elements within 
			// the XML structure properly

			// sort elements by ORDER tag
			vector <octave_value> children;

			Octave_map::iterator i = m.begin();
			int j = 0;
			while (i != m.end()) {
				// process elements (cells of structs)
				Cell cell = m.contents(i);
				Cell innerCell = cell.elem(0,0).cell_value();
				dim_vector d = innerCell.dims();

				for (int k = 0; k < d.numel(); k++) {
					children.push_back(innerCell.elem(k));
				}

				i++;
				j++;
			}

			// have the elements sorted asc by ORDER tag
			std::sort(children.begin(), children.end(), compareOctaveValues);

			// now step down the recursion and process the elements
			for (unsigned int l = 0; l < children.size(); l++) {
				writeAny(xml, C_EMPTY, children[l], index, level + 1);
			}

		} else {
			indentStr.clear();
		}

		// close for empty tag
		if (empty) {
			if (name != C_DUMMY_ELEM) {
				xml << "/>"<< endl;
			}
			return;
		}

		// close for normal tag
		if (name != C_DUMMY_ELEM) {
			xml << indentStr << "</"<< name << C_CL << endl;
		}
	}
}

/*
 * have a comparator in order to sort the values by ORDER
 */
static bool compareOctaveValues(const octave_value a, const octave_value& b) {
	
	// only to this if both values have an order element, 
	// otherwise assume that the element with an order is higher than 
	// an element without
	int orderA = 0;
	int orderB = 0;
	
	Octave_map amap = a.map_value();
	Octave_map bmap = b.map_value();
		
	if (amap.contains(C_ORDER)) {
		orderA = amap.contents(C_ORDER).elem(0,0).int_value();
	} 

	if (bmap.contains(C_ORDER)) {
		orderB = bmap.contents(C_ORDER).elem(0,0).int_value();
	} 

	return orderA < orderB;
}

static void addNamespaces(ostringstream& xml, Octave_map& m) {
	if (m.contains(C_NAMESPACE)) {
		// cellarray of map (name, value)

		Cell cell = m.contents(C_NAMESPACE);

		if ( !m.contents(C_NAMESPACE).elem(0,0).is_string() ) {

			Octave_map namespace_map = cell.elem(0,0).map_value();
			string_vector key_list = namespace_map.keys();

			for (int i = 0; i < key_list.length(); i++) {
				xml << " "<< key_list[i]<< "=\""<< namespace_map.contents(key_list[i]).elem(0,0).string_value() << "\"";
			}
		}

		m.del(C_NAMESPACE);
	}
}

static void addAttributes(ostringstream& xml, Octave_map& m) {

	// look for special fields
	if (m.contains(C_ATTR)) {
		// cellarray of string

		Cell cell = m.contents(C_ATTR);

		if ( !cell.elem(0,0).is_string() ) {
			Octave_map attr_map = cell.elem(0,0).map_value();
			string_vector key_list = attr_map.keys();

			for (int i = 0; i < key_list.length(); i++) {
				xml << " "<< key_list[i]<< "=\""<< attr_map.contents(key_list[i]).elem(0,0).string_value() << "\"";
			}
		}

		m.del(C_ATTR);
	}

}

static void addComments(ostringstream& xml, Octave_map& m) {

	if (m.contains(C_COMMENT)) {
		// cellarray of string

		Cell cell = m.contents(C_COMMENT);

		if ( !cell.elem(0,0).is_string() ) {
			Cell comments = cell.elem(0,0).cell_value();
			dim_vector d = comments.dims();
			for (int i = comments.numel() - 1; i >= 0; i--) {
				xml << "<!--"<< comments.elem(i).string_value() << "-->"<< endl;
			}
		}

		m.del(C_COMMENT);
	}
}

/*
 * writeTypesafe converts the octave values to XML which has information for type-safe reconstruction
 */
static void writeTypesafe(ostringstream& xml, const char* name,
		const octave_value val, int index, bool attrs, int level) {

	dim_vector dim_vec = val.dims();
	int dimensions = dim_vec.length();

	// indentation
	string indentStr = indent(level, true);
	xml << indentStr;
	if (name != C_EMPTY) {
		xml << C_OP << name;
	}

	if (val.is_scalar_type()) {
		// find out what kind of scalar -> complex?
		if (val.is_complex_type()) {
			Complex c = val.complex_value();
			
			xml << " idx=\""<< index << "\" type=\"complex\" size=\"1 1\">";
			xml << endl;
			xml << indent(level+1, true);
			xml << "<item idx=\"1\" type=\"double\" size=\"1 1\">";
			xml << c.real() << "</item>";
			xml << endl;
			xml << indent(level+1, true);
			xml << "<item idx=\"2\" type=\"double\" size=\"1 1\">";
			xml << c.imag() << "</item>";
			xml << endl;
			
		} else if (val.is_numeric_type()) {
			indentStr.clear();
		
			writeTypeInfo(xml, val, index, "double");
			octave_write_double(xml, val.double_value());		
			
		}

	} else if (val.is_string()) {

		indentStr.clear();
		writeTypeInfo(xml, val, index, "char");
		
		if (dimensions > 1) {
			charMatrix mtx = val.char_matrix_value();
			for (int j = 0; j < dim_vec(0); j++) {
				xml << mtx.row_as_string(j);
			}
		} else {
			xml << val.string_value();
		}

	} else if (val.is_matrix_type()) {
		xml << " idx=\""<< index << "\" type=\"";

		if (val.is_complex_matrix()) {
			ComplexMatrix mtx = val.complex_matrix_value();

			xml << "complex\" size=\"" << dim_vec(0) << " " << dim_vec(1) << "\">"; 
			
			for (int i = 0; i < dim_vec(1); i++) {
				xml << endl;
				xml << indent(level +1, true);
				xml << "<item idx=\""<< i+1
					<< "\" type=\"double\" size=\"" << dim_vec(0) << " 2\">";
				
				// well, these both loops might be improved some day... 
				for (int j = 0; j < dim_vec(0); j++) {
					Complex c = mtx.elem(j,i);
					xml << c.real() << " ";
				}	
				for (int j = 0; j < dim_vec(0); j++) {
					Complex c = mtx.elem(j,i);
					xml << c.imag() << " ";
				}	
				xml << "</item>";
			}

			xml << endl;
		} else if (val.is_real_matrix()) {

			NDArray arr = val.array_value();

			indentStr.clear();

			xml << "double\"";
			writeDimensions(xml, dim_vec);
			xml << C_CL;

			// this output needs to be generated this way 
			// to make sure it is printed in the same line
			// therefore it is compatible to GEODISE
			for (int i = 0; i < dim_vec.numel(); i++) {
				xml << arr.elem(i) << " ";
			}
			
		} else {
			xml << "unknown\"";
		}

	} else if (val.is_map()) {

		// XXX: this section might see some improvements?	
		Octave_map m = val.map_value();
		string_vector key_list = m.keys();

		writeTypeInfo(xml, val, index, "struct");

		xml << endl;
		Octave_map::iterator i = m.begin();
		int j = 0;
		while (i != m.end()) {
			Cell cell = m.contents(i);
			dim_vector d = cell.dims();

			// is it a real cell or is it a struct?    		
			if (d.length() != 2) {
				writeTypesafe (xml, C_EMPTY, cell, index, attrs, level + 1);
			} else {
				for (int k = 0; k < d.numel(); k++) {
					octave_value o_val = cell.elem(k);
					writeTypesafe (xml, key_list[j].c_str(), o_val, k+1, attrs,
							level + 1);
				}
			}
			i++;
			j++;
		}

	} else if (val.is_cell()) {
		dim_vector dv = val.dims();
		// somehow a cellarray is a bit different and does not want to be printed 
		// using writeTypeInfo()
		xml << " idx=\""<< index << "\" type=\"cell\" size=\""<< dv(0)
				<< " "<< dv(1) << "\">";

		xml << endl;
		octave_cell cell = val.cell_value();
		//int elems = 1;
		for (int k = 0; k < dv.numel(); k++) {
			writeTypesafe (xml, "item", cell.cell_value().elem(k), k+1, attrs, level+1);
		}

	} else {
		// we should never cross this place
		error("unknown type");
	}

	if (name != C_EMPTY) {
		xml << indentStr;
		xml << "</"<< name << C_CL << endl;
	}
}

/*
 * helper method - generates an indentation string according to the level passed
 */
static string indent(int i, bool typesafe) {
	string str;
	string in;
	if (typesafe) {
		in = C_INDENT_TYPESAFE;
	} else {
		in = C_INDENT;
	}
	for (int x = 0; x < i; x++) {
		str += in;
	}
	return str;
}

static void writeTypeInfo(ostringstream& xml, octave_value val, int index,
		const char* name) {
	xml << " idx=\""<< index << "\" type=\""<< name << "\"";
	writeDimensions(xml, val.dims());
	xml << ">";
}

static void writeDimensions(ostringstream& xml, dim_vector dv) {
	int i;
	xml << " size=\"";

	for (i = 0; i < dv.length() - 1; i++) {
		xml << dv(i) << " ";
	}

	xml << dv(i++) << "\"";
	return;
}

static string trim(const string& s) {
	if (s.length() == 0)
		return s;
	int b = s.find_first_not_of(" \t\n");
	int e = s.find_last_not_of(" \t\n");
	if (b == -1) // No non-spaces
		return C_EMPTY;
	if (b == 0&& e ==0) { // Empty string
		return C_EMPTY;
	}
	return string(s, b, e - b + 1);
}

