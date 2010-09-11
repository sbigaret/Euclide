function res = ProcessTradingStats(fid, stats, task, varargin);
% -------------------------------------------------------------------------
% PURPOSE : compute statistics and create graphs from simulation data
% -------------------------------------------------------------------------
% USAGE : 
% ProcessTradingStats(stats,"PricePlot");
% ProcessTradingStats(stats,"WealthPlot");
% res = ProcessTradingStats(stats,"STATS", "Diffusion", true, "Hedge", true);
% -------------------------------------------------------------------------
% INPUT: stats - from TradingSimulator
%		task - name of report 
%		varargin - keyword-value pairs 
% -------------------------------------------------------------------------

res = [];

	switch(upper(task))
		case {"WEALTHPLOT"}
			kv = {"histo" , 20, "title", NaN, "instrument", NaN};
			args = KeyValueToStruct(kv, upper(task), varargin);
			wealthplot(stats, args);
		case {"PRICEPLOT"}
			kv = {"histo" , 20, "title", NaN, "instrument", NaN};
			args = KeyValueToStruct(kv, upper(task), varargin);
			priceplot(stats, args);
		case {"STATS"}
			kv = {"Diffusion" , true, "Hedge", true};
			args = KeyValueToStruct(kv, upper(task), varargin);
			res  = calcstats(fid, stats, args);
		otherwise
			error(["task " task " is unknown."]);
	endswitch

function res = calcstats(fid, stats, args)
%
% Calculate and print statistics on trade and diffusion
%

if iscell(stats.asset),
	asset = stats.asset{1};
else
	asset = stats.asset;
endif

res = [];
if args.Diffusion,
	Rt = log(stats.SPaths(2:end,:) ./ stats.SPaths(1:end-1,:));
	[n,m] = size(Rt);
	Rt = reshape(Rt, n*m,1);
	fdisp(fid,' ');
	fdisp(fid,'Statistics for underlying asset');
	fdisp(fid,' ');
	r = calcsub(fid, Rt);
	res.Diffusion = r;
endif

if args.Hedge,
	[n,m] = size(stats.resW);
	Rt = reshape(stats.resW, n*m,1);
	fdisp(fid,' ');
	fdisp(fid,['Tracking statistics (Hedge-Asset) for ' stats.title ' on ' asset.desc]);
	fdisp(fid,' ');
	calcsub(fid,Rt);
	fdisp(fid,' ');
	fdisp(fid,['Tracking statistics (Hedge-Asset) at expiry  for ' stats.title ' on ' asset.desc]);
	fdisp(fid,' ');
	r = calcsub(fid, stats.resW(end,:)');
	res.Hedge = r;
endif
	
endfunction
	
function priceplot(stats, args)

if iscell(stats.asset),
	asset = stats.asset{1};
else
	asset = stats.asset;
endif

figure;
nPaths = size(stats.SPaths, 2);
nSteps = size(stats.SPaths,1);
T = asset.maturity;
plot(stats.dt, stats.SPaths);
title(["Price paths" "\n" stats.title "\n" asset.desc "\n " "NbSim: " num2str(nPaths) " dt: " num2str(T*252/nSteps) " day(s)"]); 
endfunction

function wealthplot(stats, args)

% plot distribution of residual wealth 

if iscell(stats.asset),
	asset = stats.asset{1};
else
	asset = stats.asset;
endif

y = stats.resW(end,:);
nPaths = size(stats.resW, 2);
nSteps = size(stats.resW,1);
T = asset.maturity;
mu_res = mean(y,2);
sigma_res = std(y);

[nn,xx] = hist(y, args.histo);
dx = xx(2)-xx(1);
nn = nn / sum(nn) / dx;
figure();
bar(xx,nn);
hold;
dx = (max(y)-min(y))/args.histo;
x = [min(y):dx:max(y)];
z = normpdf(x, mu_res, sigma_res);
h = plot(x,z,"-r"); 
set(h,"linewidth",3);

title(["Distribution of wealth at expiry (Hedge-Asset)" "\n" stats.title "\n" asset.desc "\n " "NbSim: " num2str(nPaths) " dt: " num2str(T*252/nSteps) " day(s)\n" "mean: " num2str(mu_res) " std: " num2str(sigma_res)]); 

endfunction

function res = calcsub(fid, Rt)

T = length(Rt);

mu   = mean(Rt);
mini = min(Rt);
maxi = max(Rt);
sig = std(Rt);
skew = skewness(Rt);
kurt = kurtosis(Rt);

res.mu   = mu;
res.mini = mini;
res.maxi = maxi;
res.sig = sig;
res.skew = skew;
res.kurt = kurt;

fdisp(fid,['Mean    : ',num2str(mu)]);
fdisp(fid,['Std err : ',num2str(sig)]);
fdisp(fid,['Minimum : ',num2str(mini)]);
fdisp(fid,['Maximum : ',num2str(maxi)]);
fdisp(fid,['Skewness: ',num2str(skew)]);
fdisp(fid,['Kurtosis: ',num2str(kurt)]);


endfunction
