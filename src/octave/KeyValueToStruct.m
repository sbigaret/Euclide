	function ins = KeyValueToStruct(kv, tp, v),
		i = 1;
		while (i<length(v))
		 	arg = v{i++};
		 	found = false;
		 	for j = 1:2:(length(kv)-1),
		 		if strcmp(upper(arg), upper(kv{j})),
		 			kv{j+1} = v{i};
		 			found = true;
		 			break;
		 		endif
		 	endfor
		 	if ~found,
		 		error(["argument " arg " is unknown."]);
		 	endif
		 	i++;
		endwhile
		
		ins = struct("id", tp);
		for j = 1:2:(length(kv)-1),
			ins = setfield(ins, kv{j}, kv{j+1});
		endfor
	endfunction
