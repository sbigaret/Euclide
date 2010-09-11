#! /usr/bin/octave -q

pwd
path

if( nargin != 2 )
    printf( "Usage: %s <xml file in> <xml file out>\n", program_name );
    return;
endif

% exec startup script: add directories, perform init if needed
source([getenv("EUCLIDE_PATH") "/src/octave/startup.m"]);


arg_list = argv();
filename_in = arg_list{1};
filename_out = arg_list{2};

p = xml_load(filename_in,'any');

So = str2num(p.S0{1}.TEXT);
nbPaths = str2num(p.nbPaths{1}.TEXT);
nbSteps = str2num(p.nbSteps{1}.TEXT);
T = str2num(p.horizon{1}.TEXT);

diffusion_type = p.diffusion{1}.ATTRS.type;

drift = str2num(p.diffusion{1}.drift{1}.TEXT);


# Execute the path generator
switch upper(diffusion_type),
	case {'LOGNORMAL'},
            simVol = str2num(p.diffusion{1}.sigma{1}.TEXT);
            [SPaths, dt] = \
            PathSimulator(So, nbPaths, nbSteps, T, diffusion_type, "Mu", drift, "Sigma", simVol, "Scheme", "exact");
        case {'UNKNOWNVOL'},
            simVolMin = str2num(p.diffusion{1}.sigmaMin{1}.TEXT);
            simVolMax = str2num(p.diffusion{1}.sigmaMax{1}.TEXT);
            [SPaths, dt] = \
            PathSimulator(So, nbPaths, nbSteps, T, diffusion_type, "Mu", drift, "SigmaMin", simVolMin,
            "SigmaMax", simVolMax, "Scheme", "exact");
endswitch           
disp(['path simulation done...']);

# attention: bug de xml_save: 
# respecter l'ordre de creation des champs

sim.SPaths = SPaths;
sim.dt = dt;

xml_save(filename_out, sim, 'typesafe');

disp(['OneDimSim xml file generated...']);
