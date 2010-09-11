#!/usr/bin/python

import sys
from optparse import OptionParser
from Util.xmlparser import XMLDocument

def main():
    parser = OptionParser()
    parser.add_option("--hedgingVol", dest="hedgingVol", type="float")
    parser.add_option("--discountRate", dest="discountRate", type="float")
    parser.add_option("--budget", dest="budget", type="float")
	
    parser.add_option("--file", dest="file", type="string")
    
    (opt, args) = parser.parse_args(sys.argv[1:])

    doc = XMLDocument("hedge-strategy-definition")
    doc.add("hedgingVol").addText(str(opt.hedgingVol))
    doc.add("discountRate").addText(str(opt.discountRate))

    f = open(opt.file, 'w+')
    f.write(str(doc))
    f.close()

if __name__ == '__main__':
    main()
    
