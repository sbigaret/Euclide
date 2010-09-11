#! /usr/bin/octave -q

if( nargin != 6 )
    nargin
    argv()
    printf( "Usage: %s <xml asset file> <xml path file> <xml hedging strat file> <asset file> <path file> <hedging strat file>\n", program_name );
    return;
endif

% exec startup script: add directories, perform init if needed
source([getenv("EUCLIDE_PATH") "/src/startup.m"]);

arg_list = argv();
filename_asset = arg_list{1};
filename_paths = arg_list{2};
filename_hedging = arg_list{3};

filename_asset_out = arg_list{4};
filename_paths_out = arg_list{5};
filename_hedging_out = arg_list{6}

disp(['asset file: ' filename_asset]);
asset = xml_load(filename_asset,'any');
disp(['read asset definition...']);

  CallPutFlag = asset.payoff{1}.TEXT;
  strike = str2num(asset.strike{1}.TEXT);
  quantity = str2num(asset.quantity{1}.TEXT);
  maturity = str2num(asset.maturity{1}.TEXT);
  
fid = fopen(filename_asset_out, "w");
fprintf(fid, "CallPutFlag = \"%s\";", CallPutFlag);
fprintf(fid, "Strike = %g;", strike);
fprintf(fid, "Quantity = %g;", quantity);
fprintf(fid, "Maturity = %g;\n", maturity);
fclose(fid);

p = xml_load(filename_paths,'typesafe');
disp(['read simulation data...']);

disp(['size of simulation: ' num2str(size(p.SPaths))]);

z = [p.dt p.SPaths];
save("-ascii", filename_paths_out, "z");

hedging_strat = xml_load(filename_hedging, 'any');
disp(['read hedging strat...']);

discountRate = str2num(hedging_strat.discountRate{1}.TEXT);
hedgingVol = str2num(hedging_strat.hedgingVol{1}.TEXT);

fid = fopen(filename_hedging_out, "w");
fprintf(fid, "DiscountRate = %g;", discountRate);
fprintf(fid, "HedgingVol = %g;\n", hedgingVol);
fclose(fid);
