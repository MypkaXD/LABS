function f=objfun01a(x, zm)
ff=100*(x(2)-sin(x(1)))^2+(x(1)+x(2))^2;
if ff>zm
    ff=zm;
end
    f = ff;