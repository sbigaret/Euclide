#! /usr/bin/octave -q

if( nargin != 3)
    printf( "Usage: %s <txt price file> <txt delta file> <xml asset price file> \n", program_name );
    return;
endif

% exec startup script: add directories, perform init if needed
source([getenv("EUCLIDE_PATH") "/src/startup.m"]);

arg_list = argv();
filename_price = arg_list{1};
filename_delta = arg_list{2};
filename_out = arg_list{3};

Price = load("-ascii", filename_price);
Delta = load("-ascii", filename_delta);

res.Delta = Delta;
res.Price = Price;

xml_save(filename_out, res, 'typesafe');
