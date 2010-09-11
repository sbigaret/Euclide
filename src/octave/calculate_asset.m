#! /usr/bin/octave -q

if( nargin != 4 )
    printf( "Usage: %s <xml asset file> <xml path file> <xml hedging strat file> <xml out file>\n", program_name );
    return;
endif

% exec startup script: add directories, perform init if needed
source([getenv("EUCLIDE_PATH") "/src/octave/startup.m"]);

arg_list = argv();
filename_asset = arg_list{1};
filename_paths = arg_list{2};
filename_hedging = arg_list{3};
filename_out = arg_list{4};

hedging_strat = xml_load(filename_hedging, 'any');
disp(['read hedging strat...']);
asset = xml_load(filename_asset,'any');
disp(['read asset definition...']);
p = xml_load(filename_paths,'typesafe');
disp(['read simulation data...']);

nSteps = size(p.SPaths,1);
nPaths = size(p.SPaths,2);

% Time to maturity at each step 
T = p.dt(end);
Ttm = T - p.dt; 

% pricing volatility is provided 
% Price and Delta of option for each path and each time step

Pr = zeros(nSteps, nPaths);
Delta = Pr;

discountRate = str2num(hedging_strat.discountRate{1}.TEXT);
hedgingVol = str2num(hedging_strat.hedgingVol{1}.TEXT);

rv = discountRate*ones(nSteps,1);
vv = hedgingVol*ones(nSteps,1);

	
  CallPutFlag = asset.payoff{1}.TEXT;
  strike = str2num(asset.strike{1}.TEXT);
  quantity = str2num(asset.quantity{1}.TEXT);
  
  Xv = strike*ones(nSteps,1);
  Q = quantity;

  for i = 1:nPaths,
	[P(:,i), D(:,i)] = Vanilla(CallPutFlag,p.SPaths(:,i),Xv,Ttm,rv,vv);
  end
  Pr += P*Q;
  Delta +=D*Q;

disp(['price & deltas computed...']);

res.Delta = D;
res.Price = P;

xml_save(filename_out, res, 'typesafe');

