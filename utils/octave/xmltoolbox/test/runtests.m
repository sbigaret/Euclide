# this script provides functional testing of all xml_ functions
function result = runtests()
  fprintf('xml toolbox functional tests\n')
  xml_help

  # construct data
  v.a = 'foo';
  v.b = 3.1415;
  v.d = { 'bar', 'baz', 'bazz' };
  v.g(1).a(1) = { 'g1a1' };
  v.h = 4 + 3*i;
  v.x = [1,2,3,4;5,6,7,8];
  
  w.ATTRS = "";
  w.COMMENTS = "";
  w.NAME = "ROOT";
  w.NAMESPACES = "";
  w.ORDER = 0;
  w.book{1}.ATTRS = "";
  w.book{1}.COMMENTS = "";
  w.book{1}.NAME = "book";
  w.book{1}.NAMESPACES = "";
  w.book{1}.ORDER = 1;
  w.book{1}.TEXT = "the book";

  #fprintf('teststructure is: \n')
  #v

   # test format
  fprintf('testing xml_format\n')
  rc = xmlstring = xml_format(v);
  if (!rc)
    fprintf('error in xml_format');
  endif

  # test save
  fprintf('testing xml_save\n')
  rc = xml_save('test_typed.xml', v);
  if (!rc)
    fprintf('error in xml_save (typesafe)');
  endif
  rc = xml_save('test_any.xml', w, 'any');
  if (!rc)
    fprintf('error in xml_save (any)');
  endif

  # load it
  fprintf('testing xml_load\n')
  a = xml_load('test_typed.xml');
  #if (a == 1)
  #  fprintf('error in xml_load (typesafe)');
  #endif
  b = xml_load('test_any.xml','any');
  #if (b == 1)
  #  fprintf('error in xml_load (any)');
  #endif

  # cleanup
  #delete('test_typed.xml');
  #delete('test_any.xml');

  # parse from string
  fprintf('testing xml_parse\n')
  x = xml_parse(xmlstring);
  #if (x == 1)
  #  fprintf('error in xml_parse');
  #endif

  fprintf('finished\n')
  exit 
endfunction
