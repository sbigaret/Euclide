#! /usr/bin/octave -q

if( nargin != 3)
    printf( "Usage: %s <xml instrument desc file> <xml hedge stats file1> <xml hedge stats file2> \n", program_name );
    return;
endif

% exec startup script: add directories, perform init if needed
source([getenv("EUCLIDE_PATH") "/src/octave/startup.m"]);

arg_list = argv();
% instrument definition
filename_asset = arg_list{1};
% simulation paths-1
filename_hedge_stats_1 = arg_list{2};
% simulation paths-2
filename_hedge_stats_2 = arg_list{3};
% file of summary results
filename_hedge_summary = "SummaryStats.txt";

asset = xml_load(filename_asset, 'any');
stats_1 = xml_load(filename_hedge_stats_1, 'typesafe');
stats_2 = xml_load(filename_hedge_stats_2, 'typesafe');

asset.desc = asset.description{1}.TEXT;
asset.maturity = str2num(asset.maturity{1}.TEXT);
stats_1.asset = asset;
stats_2.asset = asset;

% plot of hedging error

args.histo=20;

y_1 = stats_1.resW(end,:);
y_2 = stats_2.resW(end,:);
y = y_1 - y_2;

nPaths = size(stats_1.resW, 2);
nSteps = size(stats_1.resW,1);
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

print -djpg hedgediffplot.jpg

