function f=ftest(x)
f=atan(3*x.^5-x);
global fcount
if isempty(fcount)
fcount=1;
else
fcount=fcount+1;
end