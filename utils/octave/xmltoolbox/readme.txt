Octave XML Toolbox 0.4.8 - 2007/07/20

Copyright (C) 2007 Thomas Geiger (tom.geiger@gmail.com)

This is a test-release, bugreports and suggestions are welcome.

For further information see http://wiki.octave.org/wiki.pl?XMLToolboxPort.


Overview
--------

XML Toolbox for GNU Octave, a functional port of the GEODISE Toolbox for 
Matlab.

The current version provides the following functions:

  - xml_help, shows information for this toolbox
  - xml_load, loads XML file and converts it to a octave structure representation
  - xml_parse, converts an XML string to octave structure 
  - xml_save, saves an octave structure to XML file
  - xml_format, converts octave structure to XML string

Note that the toolbox supports two different XML formats; a 
generic one (called 'any'), where the XML will be loaded into octave keeping 
all the information necessary to store it back without information loss. This 
format might be used for interchange with any other applications.
The second XML format is the 'typesafe' format. It is based on octave.xsd and 
will persist specific attributes along with the content, so when reloading the 
data, also the appropriate datastructures (e.g. matrices, ..) will be restored.


Implementation notes
--------------------

This implementation uses the SAX2 event-based model for parsing, which shows a 
better performance during parsing as well as larger documents (~100MB) are be 
handled. 


Open Tasks
----------

  see todos.txt


Example files
-------------

Type-safe format:

<?xml version="1.0" encoding="ISO-8859-1"?>
<root xml_tb_version="3.2.0" idx="1" type="struct" size="1 1">
    <a idx="1" type="double" size="1 1">3.14</a>
    <b idx="1" type="struct" size="1 1">
        <d idx="1" type="double" size="1 1">3.1415</d>
        <e idx="1" type="char" size="1 2">PI</e>
    </b>
</root>

load it:
x = xml_load('test.xml'); (if not attribute is supplied, a typesafe file is assumed)

contents of structure:
x =
{
  a = 3.1400
  b =
  {
    d = 3.1415
    e = PI
  }
}


Any XML:
<?xml version="1.0" encoding="ISO-8859-1"?>
<book>
    <title>My First XML</title>
    <chapter>Introduction to XML
        <para>What is HTML</para>
        <para>What is XML</para>
    </chapter>
    <ISBN nr="01234567"/>
</book>


the octave structure:

b =
{
  ATTRS = 
  COMMENTS = 
  NAME = ROOT
  NAMESPACES = 
  book =

  {
    [1,1] =
    {
      ATTRS = 
      COMMENTS = 
      ISBN =

      {
        [1,1] =
        {
          ATTRS: 1x1 struct
          COMMENTS: 0x0 string
          NAME: 1x4 string
          NAMESPACES: 0x0 string
        }

      }
<snip>

This will enable you to browse the XML as following:
b.book{1}.ISBN{1}.ATTRS.nr

ans = 01234567


Install
-------

'make' should build all oct functions.

Dependencies: 
- Octave 2.1.xx or Octave 2.9.xx
- Xerces-c27

run 'make tests' and look at its output to verify that the oct 
extensions are built correctly. The warning 'warning: function name `runtests' 
does not agree with function file' can be ignored.

If you are root, liboctavexml and the header files can be installed by 
running 'make install'. Otherwise you can run the functions from the your 
current directory, after calling 'export LD_LIBRARY_PATH=.//' (so liboctavexml.so) 
will be found.


Known errors
------------

Problem: When loading file you get this:

warning: An exception occurred! Type:RuntimeException, Message:Warning: 
The primary document entity could not be opened. Id=octave.xsd at line: 0
error: Unknown element 'root' at line: 2
error: Attribute 'idx' is not declared for element 'root' at line: 2
error: Attribute 'type' is not declared for element 'root' at line: 2
error: Attribute 'size' is not declared for element 'root' at line: 2
<snip>

Solution: The parser expects the validating schema to be located at the directory 
where the XML input file resides. Make sure it is there (yes this is not nice...)


Develop/Use
-----------

Use octave_xml.h, OctaveXMLReader.h and OctaveXMLWriter.h if you want to use the conversion functions 
in your program directly.

