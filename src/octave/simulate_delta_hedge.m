#! /usr/bin/octave -q

if( nargin != 4 )
    printf( "Usage: %s <xml path file> <xml asset price and delta file> <xml hedging strat file> <xml out file>\n", program_name );
    return;
endif

% exec startup script: add directories, perform init if needed
source([getenv("EUCLIDE_PATH") "/src/octave/startup.m"]);

tr = false;

arg_list = argv();
% simulation paths
filename_paths = arg_list{1};
% hedged asset price and delta
filename_asset_price = arg_list{2};
% hedging parameters
filename_hedging = arg_list{3};
%output
filename_out = arg_list{4};

hedging_strat = xml_load(filename_hedging, 'any');
if (tr) disp(['read hedging strat...']); endif
p = xml_load(filename_paths,'typesafe');
if (tr) disp(['read simulation data...']); endif
a = xml_load(filename_asset_price,'typesafe');
if (tr) disp(['read asset data...']); endif

nSteps = size(p.SPaths,1);
nPaths = size(p.SPaths,2);

% Time to maturity at each step 
T = p.dt(end);
Ttm = T - p.dt; 

discountRate = str2num(hedging_strat.discountRate{1}.TEXT);
hedgingVol = str2num(hedging_strat.hedgingVol{1}.TEXT);
if isfield(hedging_strat, 'budget')
  budget = str2num(hedging_strat.budget{1}.TEXT);
else
  budget = NaN;
end

alpha = zeros(nSteps, nPaths);
Vt = zeros(nSteps, nPaths);
Bt = exp(-discountRate*Ttm);

% intial portfolio
if isnan(budget),
  Vt(1,:)=a.Price(1,:);
else,
  Vt(1,:)=budget;
end

alpha(1,:)=(Vt(1,:) - a.Delta(1,:) .* p.SPaths(1,:) ) / Bt(1); % units in risk-free asset
	        
for i = 2:nSteps,
	% value at time t(i)
	Vt(i,:) = alpha(i-1,:)*Bt(i) + a.Delta(i-1,:) .* p.SPaths(i,:);
	% quantity of bonds 
	alpha(i,:) = (Vt(i,:) - a.Delta(i,:) .* p.SPaths(i,:)) / Bt(i);
end	

stats.SPaths = p.SPaths;
stats.resW = Vt-a.Price;
stats.title = 'Delta Hedge';

xml_save(filename_out, stats, 'typesafe');
if(tr) disp(['hedge residuals computed...']); endif

