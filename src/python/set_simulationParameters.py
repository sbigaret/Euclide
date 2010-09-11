#!/usr/bin/python

import sys
from optparse import OptionParser
from Util.xmlparser import XMLDocument

def set_gabillon_simulationParameters(horizon, S0, nbSteps, nbPaths, drift, sigmaS, sigmaL, rho, beta):

    # Example of dumping a database structure
    doc = XMLDocument("simulation-parameters")
    doc.add("horizon").addText(str(horizon))
    doc.add("S0").addText(str(S0))
    doc.add("nbSteps").addText(str(nbSteps))
    doc.add("nbPaths").addText(str(nbPaths))
    diff = doc.add("diffusion", type="Gabillon")
    diff.add("drift").addText(str(drift))
    diff.add("sigmaS").addText(str(sigmaS))
    diff.add("sigmaL").addText(str(sigmaL))
    diff.add("rho").addText(str(rho))
    diff.add("beta").addText(str(beta))

    return doc

def set_lognormal_simulationParameters(horizon, S0, nbSteps, nbPaths, drift, sigma):

    # Example of dumping a database structure
    doc = XMLDocument("simulation-parameters")
    doc.add("horizon").addText(str(horizon))
    doc.add("S0").addText(str(S0))
    doc.add("nbSteps").addText(str(nbSteps))
    doc.add("nbPaths").addText(str(nbPaths))
    diff = doc.add("diffusion", type="LogNormal")
    diff.add("drift").addText(str(drift))
    diff.add("sigma").addText(str(sigma))

    return doc

def set_lognormalmr_simulationParameters(horizon, S0, nbSteps, nbPaths, theta, kappa, sigma):

    # Example of dumping a database structure
    doc = XMLDocument("simulation-parameters")
    doc.add("horizon").addText(str(horizon))
    doc.add("S0").addText(str(S0))
    doc.add("nbSteps").addText(str(nbSteps))
    doc.add("nbPaths").addText(str(nbPaths))
    diff = doc.add("diffusion", type="LogNormal")
    diff.add("theta").addText(str(theta))
    diff.add("kappa").addText(str(kappa))
    diff.add("sigma").addText(str(sigma))

    return doc

def set_unknown_simulationParameters(horizon, S0, nbSteps, nbPaths, drift,
                                     sigmaMin, sigmaMax):

    # Example of dumping a database structure
    doc = XMLDocument("simulation-parameters")
    doc.add("horizon").addText(str(horizon))
    doc.add("S0").addText(str(S0))
    doc.add("nbSteps").addText(str(nbSteps))
    doc.add("nbPaths").addText(str(nbPaths))
    diff = doc.add("diffusion", type="UnknownVol")
    diff.add("drift").addText(str(drift))
    diff.add("sigmaMin").addText(str(sigmaMin))
    diff.add("sigmaMax").addText(str(sigmaMax))

    return doc

def main():
    print 'starting set_simulationParameters'
	
    parser = OptionParser()
    parser.add_option("--horizon", dest="horizon", type="float")
    parser.add_option("--S0", dest="S0", type="float")
    parser.add_option("--nbSteps", dest="nbSteps", type="int")
    parser.add_option("--nbPaths", dest="nbPaths", type="int")
    parser.add_option("--diffusion", dest="diffusion", type="string")
    parser.add_option("--mu", dest="mu", type="float")
    parser.add_option("--sigma", dest="sigma", type="float")
    parser.add_option("--sigmaMax", dest="sigmaMax", type="float")
    parser.add_option("--sigmaMin", dest="sigmaMin", type="float")
    parser.add_option("--file", dest="file", type="string")
    
    (opt, args) = parser.parse_args(sys.argv[1:])

    if opt.diffusion.upper() == 'LOGNORMAL':
        doc = set_lognormal_simulationParameters(opt.horizon, opt.S0, opt.nbSteps,
                                             opt.nbPaths, opt.mu, opt.sigma)
    elif opt.diffusion.upper() == 'UNKNOWNVOL':
        doc = set_unknown_simulationParameters(opt.horizon, opt.S0, opt.nbSteps,
                                             opt.nbPaths, opt.mu, opt.sigmaMin,
                                             opt.sigmaMax)
    elif opt.diffusion.upper() == 'GABILLON':
        doc = set_gabillon_simulationParameters(opt.horizon, opt.S0, opt.nbSteps,
                                             opt.nbPaths, opt.mu, opt.sigmaS,
                                             opt.sigmaL, opt.rho, opt.beta)
    
    elif opt.diffusion.upper() == 'LOGNORMALMR':
        doc = set_lognormalmr_simulationParameters(opt.horizon, opt.S0, opt.nbSteps,
                                             opt.nbPaths, opt.theta, opt.kappa, opt.sigma)
    

    f = open(opt.file, 'w+')
    f.write(str(doc))
    f.close()

if __name__ == '__main__':
    main()
    
