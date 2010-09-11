#include "TypesafeXMLHandler.h"

static void tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ");
static void parseDimensions(const string size, ElementInfo* e); 

static octave_value* buildOctaveValue(ElementInfo& info, stack<pair <ElementInfo*, octave_value*> >& oct_values);


TypesafeXMLHandler::TypesafeXMLHandler() {
	// create initial element (struct)
	ElementInfo el;
	el.number = count++;
	el.type = C_STRUCT;

	dim_vector dv;
	dv.resize(2);
	dv(0) = 1;
	dv(1) = 1;
	el.vec = dv;
	
	infos.push(&el);
}

void TypesafeXMLHandler::startElement 
(
		const XMLCh* const uri,
		const XMLCh* const localname,
		const XMLCh* const qname,
		const Attributes& attrs)
{
	ElementInfo* el = new ElementInfo();
	el->name = XMLString::transcode(localname);
	el->type = extractAttribute(attrs, C_TYPE);
	parseDimensions(extractAttribute(attrs, C_SIZE), el);
	el->number = count++;
	infos.push(el);
}

void TypesafeXMLHandler::characters
(		
		const   XMLCh* const chars,
		const   unsigned int length) 
{
	if (infos.size() > 0) {
		ElementInfo* eInfo = infos.top();
		// make sure to append all contents
		eInfo->value << XMLString::transcode(chars);		
	}
}		

void TypesafeXMLHandler::endElement
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
		octave_value* value = buildOctaveValue(*info, oct_values);
		//add to oct_value stack
		pair<ElementInfo*, octave_value*> p (info, value);
		oct_values.push( p );	
		root = value;
	}
	return;
}

octave_value* buildOctaveValue(ElementInfo& info, stack<pair <ElementInfo*, octave_value*> >& oct_values) {
	
	// parsing
	// open: char/complex matrix

	octave_value* oct_val = NULL;
	
	string type = info.type;
	unsigned int number = info.number;
	
	dim_vector dims = info.vec;
	
	if (type == C_STRUCT) {
		
		if (dims(0) >  1) {
			cerr << "Loading of struct array not supported yet." << endl;
			oct_val = new octave_value( "STRUCT ARRAY NOT SUPPORTED." );
		} else {
		
			Octave_map map;
					
			// add children to map
			pair<ElementInfo*, octave_value*> p = oct_values.top();
			while ( oct_values.size() >= 1 &&  p.first->number > number ) {
				oct_values.pop();
				map.assign(p.first->name, *p.second);
				if (oct_values.size() > 0) {
					p = oct_values.top();	
				} 
			}		
			oct_val = new octave_value( map );
		}
	} else if (type == C_CELL) {
		
		if (dims(0) >  1) {
						
			pair<ElementInfo*, octave_value*> p = oct_values.top();
			
			NDArray array(dims); // init array with dimensions
			//cout << array.numel() << endl;
			int i = array.numel() - 1;
			while ( oct_values.size() >= 1 &&  p.first->number > number ) {
				oct_values.pop();
				
				double val = octave_value( *p.second ).double_value();
				
				// 40, 30, 20, 10
				array(i) = val;
				/*
				list.append(*p.second);
				*/
				if (oct_values.size() > 0) {
					p = oct_values.top();	
				} 
				i--;
				
			}
			
			
			oct_val = new octave_value( array );
		} else {
			octave_value_list list;
					
			// collect all values in a list
			pair<ElementInfo*, octave_value*> p = oct_values.top();
			while ( oct_values.size() >= 1 &&  p.first->number > number ) {
				oct_values.pop();
				list.append(*p.second);
				if (oct_values.size() > 0) {
					p = oct_values.top();	
				} 
			}	
				
			// create cell		
			list.reverse();
			oct_val = new octave_value( Cell(list) );
		}
	} else if (type == C_DOUBLE) {
				
		if (dims(0) > 1) {			
			NDArray array(dims);
			info.value >> array;				
			oct_val = new octave_value( array );
		} else {		
			// normal double value
			oct_val = new octave_value( octave_read_double( info.value ) );
		}
		
	} else if (type == C_CHAR) {
		
		if (dims(0) > 1) {
			cerr << "Loading of char matrices not supported yet." << endl;
			/*
			cout << "char matrix" << endl;
			octave_value val;
			
			//string_vector sv;

			cout << "contents: " << value[0] << endl;
				//sv.append( val.string_value() );		
			
			oct_val = new octave_value( charMatrix (value[0]) );
			*/
			oct_val = new octave_value("CHAR MATRIX NOT SUPPORTED");
		} else {		 
			oct_val = new octave_value( info.value.str() );
		}
	} else if (type == C_COMPLEX) {
		
		if (dims(0) > 1) {
			
			pair<ElementInfo*, octave_value*> p = oct_values.top();
				
			ComplexNDArray array(dims); // init array with dimensions
			cout << array.numel() << endl;
			int i = array.numel() - 1;
			while ( oct_values.size() >= 1 &&  p.first->number > number ) {
				oct_values.pop();
				
				double imag = octave_value( *p.second ).double_value();
				p = oct_values.top();
				oct_values.pop();
				double real = octave_value( *p.second ).double_value();
				
				array(i) = Complex(real, imag);
				
				if (oct_values.size() > 0) {
					p = oct_values.top();	
				} 
				i--;
				
			}
			
			/*
			<root xml_tb_version="3.2.0" idx="1" type="complex" size="2 2"> 
  				<item type="double" idx="1" size="2 2">2 4 3 5</item>
  				<item type="double" idx="2" size="2 2">4 7 5 8</item>
			</root>
			*/
			
			oct_val = new octave_value( array );
		} else {
			//octave_read_complex()
			
			octave_value real; 
			octave_value imag;
					
			// read real
			pair<ElementInfo*, octave_value*> p = oct_values.top();
			imag = *p.second;
			oct_values.pop();
			
			// imag
			p = oct_values.top();
			real = *p.second;
			oct_values.pop();

			// build complex
			Complex c(real.double_value(), imag.double_value());
			oct_val = new octave_value(c);
		}		
	} else {	
		// error case -> lets fallback to char
		oct_val = new octave_value( info.value.str() );	
	}
	
	return oct_val;
}

void tokenize(const string& str, vector<string>& tokens, const string& delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void parseDimensions(const string size, ElementInfo* e) 
{	
	vector<string> tokens;
	tokenize(size, tokens);	

	// init dim_vec with nr of dimensions
	dim_vector dv;
	dv.resize(tokens.size());
	
	for (unsigned int i = 0; i < tokens.size(); i++) {
		dv(i) = ( atoi( tokens[i].c_str() ) );
	}
	e->vec = dv;
}

