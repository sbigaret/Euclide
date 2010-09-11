function [Spath, dt] = simBS(So, T, nbSteps, nbPaths, args)
% S = simBS(S_0,mu,sigma,T,dt,nSteps, nPaths)
%
% Produces N simulations of the Black-Scholes process
                          1
% S_t = S_0*exp(mu*t - sigma^2/2*t + sigma*W_t)
% over the interval (0:dt:T).
% S_0 is the starting value of S
% output -> S, array of size N x n, with n=length([0:dt:T])

% intermediate calculations

scheme = args.scheme;
mu = args.mu;
sigma = args.sigma;

Dt = T/nbSteps;
dt = [0:Dt:T]';
sqDt = sqrt(Dt);

% antithetic variates and exact process simulation
Z = randn(nbSteps,nbPaths/2);

if (upper(scheme) == 'EXACT'),
	Spath = So*[ones(1, nbPaths); 
		    cumprod(exp(mu*Dt+sigma*sqrt(Dt)*[Z -Z]))];
elseif (upper(scheme) == 'EULER'),
	Spath = So*[ones(1, nbPaths); 
		    1 + cumsum((mu+.5*sigma^2)*Dt+sigma*sqrt(Dt)*[Z -Z])];
else,
	error(['scheme ' scheme ' not found.']);
endif