#!/usr/bin/python

import sys
from optparse import OptionParser
from Util.xmlparser import XMLDocument

def set_vanilla(payoff, strike, maturity, quantity):
    
    # Example of dumping a database structure
    doc = XMLDocument("asset-definition")
    doc.add("class").addText("vanilla")
    doc.add("payoff").addText(str(payoff))
    doc.add("strike").addText(str(strike))
    doc.add("maturity").addText(str(maturity))
    doc.add("quantity").addText(str(quantity))
    desc = 'Payoff: ' + str(payoff) + ' Strike: ' + str(strike) + ' Maturity: ' + str(maturity) + ' Quantity: ' + str(quantity)
    doc.add("description").addText(desc)

    return doc

def main():
    parser = OptionParser()
    parser.add_option("--class", dest="Class", type="string")
    parser.add_option("--payoff", dest="payoff", type="string")
    parser.add_option("--strike", dest="strike", type="float")
    parser.add_option("--maturity", dest="maturity", type="float")
    parser.add_option("--quantity", dest="quantity", type="float")
    parser.add_option("--file", dest="file", type="string")
    
    (opt, args) = parser.parse_args(sys.argv[1:])

    if opt.Class.upper() == 'VANILLA':
        doc = set_vanilla(opt.payoff, opt.strike, opt.maturity, opt.quantity)
    else:
        pass

    f = open(opt.file, 'w+')
    f.write(str(doc))
    f.close()

if __name__ == '__main__':
    main()
    
