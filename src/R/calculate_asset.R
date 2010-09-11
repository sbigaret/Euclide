# usage:
# R --slave --vanilla --args <xml asset file> <xml path file> <xml hedging strat file> <xml out file> < calculate_asset.R

library(XML)
library(fOptions)

p <- Sys.getenv("EUCLIDE_PATH")
fp <- file.path(p, "src/R/ParseUtilPraxis.R")

if(file.exists(fp)) source(fp) else stop("file ParseUtilPraxis.R not found")


do_calculate_asset <- function(filename_asset, filename_paths, filename_hedging, trace=FALSE) {
	if(trace) {
	  print(filename_asset)
	  print(filename_paths)
	  print(filename_hedging)
	}
	
	hedging_strat <- xmlParseHedgingStrategy(filename_hedging)
	if(trace) print('R: read hedging strat...')
	asset <- xmlParseAsset(filename_asset)
	if(trace) print('R: read asset definition...')
	p <- xmlParseOneDimSimulationData(filename_paths)
	if(trace) print('R: read simulation data...')
	
	nSteps = p$nbSteps
	nPaths = p$nbPaths
	
	TExp <- p$dt[-1]
	Ttm <- T - p$dt 
	
	Pr <- matrix(0, nrow=nSteps, ncol=nPaths)
	Delta <- matrix(0, nrow=nSteps, ncol=nPaths)
	
	discountRate = hedging_strat$discountRate
	hedgingVol = hedging_strat$hedgingVol
	
	CallPutFlag = asset$payoff
	strike = asset$strike
	quantity = asset$quantity
	
	for (i in 1:nSteps) {
		vx <- GBSOption(tolower(CallPutFlag),p$SPaths[i,],strike,Ttm[i],
				discountRate,discountRate, hedgingVol)
		Pr[i,] <- vx@price
		
		vx <- GBSGreeks("delta", tolower(CallPutFlag),p$SPaths[i,],strike,Ttm[i],
				discountRate,discountRate, hedgingVol)
		Delta[i,] <- vx
	}
	
	if(trace) print('R: price & deltas computed...')
	
	res = list()
	res$Delta = Delta
	res$Price = Pr
	res$nSteps = nSteps
	res$nPaths = nPaths
	
	res	
}

filename_asset <- commandArgs()[5]
filename_paths <- commandArgs()[6]
filename_hedging <- commandArgs()[7]
filename_out <- commandArgs()[8]
trace = FALSE

vx <- do_calculate_asset(filename_asset, filename_paths, filename_hedging, trace)
	
rootNode <- xmlNode("root", attrs=c(idx="1", type="struct", size="1 1"))

att <- list()
att$idx <- "1"
att$type <- "double"
att$size <- paste(c(vx$nSteps, vx$nPaths), collapse=" ")

sNode <- xmlNode("Price", 
		attrs=att,
		paste(vx$Price, collapse=" "))
rootNode <- addChildren(rootNode, sNode)

dNode <- xmlNode("Delta", 
		attrs=att,
		paste(vx$Delta, collapse=" "))
rootNode <- addChildren(rootNode, dNode)

fid = file(filename_out, "w")
if(trace) print(paste('R: writing xml file' , filename_out))
saveXML(rootNode, file=fid, prefix='<?xml version="1.0"?>\n')
close(fid)
