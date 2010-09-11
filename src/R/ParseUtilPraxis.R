
library(XML)

# parse command line arguments, returns a list
# --------------------------------------------

parse_args <- function(ca) {
  a <- list();
  for (e in ca) {
    ta = strsplit(e,"=",fixed=TRUE)
	tag = ta[[1]][1]
	# strip leading -- from tag if present
    tag = toupper(gsub("-","", tag));
	val = ta[[1]][2]
	if (is.na(val)) {
	  # just a flag, set to TRUE
	  a[[tag]] <- TRUE; 
	} else {
	  # all values are strings, will be interpreted in context
	  a[[tag]] <- val;
	}
  }
  a
}

# parse an asset
# --------------

xmlParseVanilla <- function(rootNode) {
  a <- xmlToList(rootNode)
  a$strike <- as.real(a$strike)
  a$maturity <- as.real(a$maturity)
  a$quantity <- as.real(a$quantity)
  a
  }

  xmlParseLogNormal <- function(rootNode){
	  a = list()
	  a$diffusion <- "LOGNORMAL"
	  a$drift <- as.real(xmlValue(rootNode["drift"]))
	  a$sigma <- as.real(xmlValue(rootNode["sigma"]))
	  a
  }
  
xmlParseAsset <- function(filename) {
  x <- xmlRoot(xmlTreeParse(filename, getDTD=F))
 
  if (toupper(xmlValue(x["class"]$class)) == "VANILLA"){
	a <- xmlParseVanilla(x)
  }
  else
    a <- "error"
  a
}

xmlParseHedgingStrategy <- function(filename) {
	rootNode <- xmlRoot(xmlTreeParse(filename, getDTD=F))
	a <- xmlToList(rootNode)
	a$hedgingVol <- as.real(a$hedgingVol)
	a$discountRate <- as.real(a$discountRate)
	a
}

xmlParseOneDimSimulationData <- function(filename){
	a <- list()
	rootNode <- xmlRoot(xmlTreeParse(filename, getDTD=F))
	SPaths <- rootNode[["SPaths"]]
	s <- sapply(strsplit(xmlAttrs(SPaths)[['size']], split=" "), as.numeric)
	nbRows <- s[1]
	nbCols <- s[2]
	m <- sapply(strsplit(xmlValue(SPaths), split = " "), as.numeric)
	attr(m, "dim") <- c(nbRows, nbCols)
	a$nbSteps <- nbRows
	a$nbPaths <- nbCols
	a$SPaths <- m
	
	dt <- rootNode[["dt"]]
	a$dt <- sapply(strsplit(xmlValue(dt), split = " "), as.numeric)
	attr(a$dt, "dim") <- c(nbRows, 1)
	
	a
}

xmlParseSimulationParameters <- function(filename){
	rootNode <- xmlRoot(xmlTreeParse(filename, getDTD=F))
	a <- list()
	a$horizon <- as.real(xmlValue(rootNode["horizon"]$horizon))
	# TODO
	# Initial state S0 should be part of diffusion description
	a$S0 <- as.real(xmlValue(rootNode["S0"]))
	a$nbSteps <- as.integer(xmlValue(rootNode["nbSteps"]$nbSteps))
	a$nbPaths <- as.integer(xmlValue(rootNode["nbSteps"]$nbPaths))
	
	diffusionNode <- rootNode[["diffusion"]]
	attributes <- xmlAttrs(diffusionNode)
	if (toupper(attributes[['type']]) == "LOGNORMAL"){
		a$diffusion <- xmlParseLogNormal(diffusionNode)
	}
	else
		a <- "error"
	a	
}
