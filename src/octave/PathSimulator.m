function [SPaths, dt] = PathSimulator(So, nbPaths, nbSteps, T, difName,varargin)
% -------------------------------------------------------------------------
% PURPOSE : generates paths following the specified stochastic processstochastic
% -------------------------------------------------------------------------
% USAGE : [SPaths] = simulate(So, nbPaths, nbSteps, T, difName, difParams)
% -------------------------------------------------------------------------
% INPUT: So - Scalar, Stock price at time 0
%        nbPaths - number of paths 
%		nbSteps - number of time steps 
%		T - simulation horizon in fraction of years 
%		difname - identifier of the diffusion process 
%		difParams - data structure holding the parameters of the diffusion 
%--------------------------------------------------------------------------
% OUTPUT : SPaths : matrix of simulated paths: nbSteps rows, nbPaths columns. 
%--------------------------------------------------------------------------
%COMMENTS: 
%		BS: r, sigma 
%		ds/s = mu dt + sigma dW 

%		Heston: mu, a, b, sig, rho
% 		ds  = mu dt + Vt^1/2 dW_1t
% 		dVt = b(a-Vt) dt + sig Vt^1/2 dW_2t
% 		<dW_1t, dW_2t> = rho
% -------------------------------------------------------------------------

switch upper(difName),
	case {'LOGNORMAL'},
	kv = {"mu", 0, "sigma", NaN, "scheme", "exact"};
	args = KeyValueToStruct(kv, upper(difName), varargin);
	[SPaths, dt] = simBS(So, T, nbSteps, nbPaths, args );
     case {'UNKNOWNVOL'},
	kv = {"mu", 0, "sigmaMin", NaN,  "sigmaMax", NaN, "scheme", "exact"};
	args = KeyValueToStruct(kv, upper(difName), varargin);
	[SPaths, dt] = simUnknownVol(So, T, nbSteps, nbPaths, args );
     case {'HISTORICAL'},
	kv = {"frequency", 1/365, "returns", NaN};
	args = KeyValueToStruct(kv, upper(difName), varargin);
	[SPaths, dt] = simHistorical(So, T, nbSteps, nbPaths, args );
    otherwise 
	error(['diffusion ' difName ' is not recognized.']);
endswitch

