# includes
LOCAL_CPPFLAGS = -O2 -fPIC -I/usr/include/octave -I/usr/include/octave/octave -mieee-fp -O2

LOCAL_LDFLAGS = -L.//

# installation target for shared xml lib
INSTDIR = /usr/lib

# installation target for header files
HEADERDIR = /usr/include/octavexml

HEADERS = OctaveHandler.h GenericXMLHandler.h TypesafeXMLHandler.h octave_xml.h OctaveXMLReader.h OctaveXMLWriter.h

PROGRAMS = xml_save xml_load xml_help

LIB = liboctavexml.so

all: $(LIB) $(PROGRAMS) links

libs: clean $(LIB)

$(LIB):
	$(CXX) -c $(LOCAL_CPPFLAGS) octavexml/OctaveHandler.cc -o octavexml/OctaveHandler.o
	$(CXX) -c $(LOCAL_CPPFLAGS) octavexml/GenericXMLHandler.cc -o octavexml/GenericXMLHandler.o
	$(CXX) -c $(LOCAL_CPPFLAGS) octavexml/TypesafeXMLHandler.cc -o octavexml/TypesafeXMLHandler.o
	$(CXX) -c $(LOCAL_CPPFLAGS) octavexml/OctaveXMLReader.cc -o octavexml/OctaveXMLReader.o
	$(CXX) -c $(LOCAL_CPPFLAGS) octavexml/OctaveXMLWriter.cc -o octavexml/OctaveXMLWriter.o
	gcc -shared -Wl,-Bsymbolic -O2 -o $@ octavexml/OctaveHandler.o octavexml/GenericXMLHandler.o octavexml/TypesafeXMLHandler.o octavexml/OctaveXMLReader.o octavexml/OctaveXMLWriter.o -lxerces-c
	
$(PROGRAMS): 
	mkoctfile -o $@.oct $(LOCAL_LDFLAGS) $@.cc -loctavexml

links:
	ln -s xml_save.oct xml_format.oct
	ln -s xml_load.oct xml_parse.oct

clean: ; -$(RM) *.a *.o octavexml/*.o *.so core octave-core *.oct *~ ; $(RM) -r latex html 

install: install-lib install-headers

install-lib: $(LIB)
	@if [ -d $(INSTDIR) ]; \
		then \
		cp $(LIB) $(INSTDIR) && \
		chmod a+x $(INSTDIR)/$(LIB) && \
		chmod og-w $(INSTDIR)/$(LIB) && \
		echo "Installed $(LIB) in $(INSTDIR)" ;\
	else \
		echo "Could not install $(LIB) in $(INSTDIR)."; false ; \
	fi

install-headers:
	-mkdir -p $(HEADERDIR)
	@if [ -d $(HEADERDIR) ]; \
		then \
		cp octave.xsd octavexml/octave_xml.h octavexml/OctaveXMLReader.h octavexml/OctaveXMLWriter.h\
		octavexml/GenericXMLHandler.h octavexml/TypesafeXMLHandler.h octavexml/OctaveHandler.h $(HEADERDIR) && \
		chmod a+x $(HEADERDIR)/*.h && \
		chmod og-w $(HEADERDIR)/*.h && \
		echo "Installed $(HEADERS) in $(HEADERDIR)" ;\
	else \
		echo "Could not install $(HEADERS) in $(HEADERDIR)."; false ; \
	fi	

doxygen: ; doxygen doxygen.conf

tests: ; octave -q test/runtests.m
