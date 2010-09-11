function [price,delta,vega,gamma,theta] = Vanilla(CallPutFlag,S,X,T,r,v)
%
% Computes price and greeks of vanilla payoffs:
% F:		Forward
% C/P:	European Call/Put
% DC/DP:	Digital Call/Put 
%
% Implementation is vectorized

% verify consistency of dimensions

if upper(CallPutFlag) == "S",
	[pc,dc,vc,gc,tc] = Vanilla("C",S,X,T,r,v);
	[pp,dp,vp,gp,tp] = Vanilla("P",S,X,T,r,v);
	price = pc + pp;
	delta = dc + dp;
	vega = vc + vp;
	gamma = gc + gp;
	theta = tc + tp;
	return;
endif
	
dim = [size(S,1), size(X,1), size(T,1), size(r,1), size(v,1)];

if (min(dim) != max(dim)),
	dim
	error('all arguments must have same number of rows');
endif

dim = [size(S,2), size(X,2), size(T,2), size(r,2), size(v,2)];

if (min(dim) != max(dim)),
	dim
	error('all arguments must have same number of columns');
endif

b=r;
d1 = (log(S ./ X) + (b + v .^ 2 / 2) .* T) ./ (v .* (T .^ 0.5));
d2 = d1 - v .* T .^ 0.5;

if (upper(CallPutFlag) == "C") || (upper(CallPutFlag) == "P"),
  if upper(CallPutFlag) == "C" ,
    price = S .* normcdf(d1) - X .* exp(-r .* T) .* normcdf(d2);
    if nargout>1,
	  delta=exp((b-r) .* T) .* normcdf(d1);
    endif
    if nargout == 5,
		theta_tmp1= -( S .* exp((b-r) .* T) .* normpdf(d1) .* v ) ./ (2*T .^ 0.5);
		theta_tmp2= -(b-r) .* S .* exp((b-r) .* T) .* normcdf(d1);
		theta_tmp3= -r .* X .* exp(-r .* T) .* normcdf(d2);
		theta = theta_tmp1+theta_tmp2+theta_tmp3;
    endif
  elseif upper(CallPutFlag) == "P",
    price = X .* exp(-r .* T) .* normcdf(-d2) - S .* normcdf(-d1);
    if nargout>1,
	  delta=exp((b-r) .* T) .* (normcdf(d1)-1);
    endif
    if nargout == 5,
		theta_tmp1= -( S .* exp((b-r) .* T) .* normpdf(d1) .* v ) ./ (2 * T .^ 0.5);
		theta_tmp2= (b-r) .* S .* exp((b-r) .* T) .* normcdf(-d1);
		theta_tmp3= r .* X .* exp(-r .* T) .* normcdf(-d2);
		theta=theta_tmp1+theta_tmp2+theta_tmp3;
    endif
   endif

  if nargout>3,
    gamma=(normpdf(d1) .* exp((b-r) .* T)) ./ (S .* v .* T .^ 0.5);
  endif

  if nargout>2,
    vega= S .* exp((b-r).*T).*normpdf(d1).*T.^0.5;
  endif

elseif upper(CallPutFlag) == "DC",
  price = exp(-r .* T) .* normcdf(d2);
  delta = exp(-r .* T) .* normpdf(d2) ./ (S .* v .* sqrt(T));
  gamma = NaN;
  vega = -exp(-r .* T) .* normpdf(d2) .* sqrt(T) / 2;

elseif upper(CallPutFlag) == "DP",
  price = exp(-r .* T) .* (1-normcdf(d2));
  delta = -exp(-r .* T) .* normpdf(d2) ./ (S .* v .* sqrt(T));
  gamma = NaN;
  vega = NaN;
endif
    
endfunction