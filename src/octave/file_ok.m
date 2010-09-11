function status = file_ok(filename),
  fid = fopen(filename,'r');
  status = (fid>0);
  if(status),
    fclose(fid);
  end
endfunction
