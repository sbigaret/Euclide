#! /usr/bin/octave -q

if( nargin != 2 )
    printf( "Usage: %s <xml instrument desc file> <xml hedge stats file>\n", program_name );
    return;
endif

% exec startup script: add directories, perform init if needed
source([getenv("EUCLIDE_PATH") "/src/octave/startup.m"]);

tr = false;

arg_list = argv();

% instrument definition
filename_asset = arg_list{1};
% simulation paths
filename_hedge_stats = arg_list{2};
% file of summary results
filename_hedge_summary = "SummaryStats.txt";

if(file_ok(filename_asset))
  asset = xml_load(filename_asset, 'any'); 
else
  exit(-1);
end

if(tr) disp(['Read ' filename_asset]); endif

if(file_ok(filename_hedge_stats))
  stats = xml_load(filename_hedge_stats, 'typesafe');
else
  exit(-1);
end

if(tr) disp(['Read ' filename_hedge_stats]); endif

asset.desc = asset.description{1}.TEXT;
asset.maturity = str2num(asset.maturity{1}.TEXT);
stats.asset = asset;

fid = fopen(filename_hedge_summary, "w");

ProcessTradingStats(fid, stats,"WealthPlot");
# print -deps wealthplot.eps
print -djpg wealthplot.jpg

ProcessTradingStats(fid, stats,"STATS", "Diffusion", true, "Hedge", true);
fclose(fid);
