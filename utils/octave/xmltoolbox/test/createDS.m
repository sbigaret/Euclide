function s = createDS( NUM )
    s = struct();

    for i = 1:NUM
        s(i).string = 'test';
        s(i).int = 1;
        s(i).double = 3.1415;
        s(i).cell = cell(2);
        s(i).cell{1,1} = 1;
        s(i).cell{2,1} = 2;
        s(i).cell{1,2} = 3;
        s(i).cell{2,2} = 4;
        s(i).structure.a = 'test1';
        s(i).structure.b = 'test2';

        s(i).matrix = rand(10000,1);
        s(i).matrix1 = rand(10,10);
        s(i).matrix2 = rand(10,10,10);
        s(i).matrix3 = rand(10,10,10,10);

        s(i).complex = 3 + 4i;
        s(i).complex_mat = [ 1+2i, 2+3i; 3+4i, 4+5i ];
		s(i).complex_cell = { 1+2i, 2+3i; 3+4i, 4+5i };

		s(i).another_cell = { 'tom', 1, { 2+3i; 3+4i }, 'geiger', [ 1 2 3; 4 5 6 ], [ 'a', 'b'; 'c', 'd'] , { 'a', 'b', 'c'; 'd', 'e', 'f'; 'g', 'h', 'i' } };


        s(i).char_array = { 'hans' 'josef'; 'rudolf' 'georg' };

    end

    whos s
