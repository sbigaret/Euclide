function result = testtypes()
   v.a = 'foo'
   v.b = 3.1415
   v.c = [ 0 1 2; 3 4 5 ]
   v.d = { 'bar', 'baz', 'bazz' }
   v.e = ( 0 == 1 )
   v.g(1).a(1) = { 'g1a1' }
   v.h = 4 + 3*i
   result = v;
endfunction
