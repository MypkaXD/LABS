function [f,g,H] = gradgess(x)
f = 100*(x(2)- sin(x(1)))^2+(x(1)+x(2))^2;
if nargout>1
 g(1) = -200*cos(x(1))*(x(2)- sin(x(1)))+2*(x(1)+x(2));
 g(2) = 200*(x(2)- sin(x(1)))+2*(x(1)+x(2));
end;
if nargout>2
 H(1,1) = 200*sin(x(1))*(x(2)- sin(x(1)))+200*cos(x(1))*cos(x(1))+2;
 H(1,2) = -200*cos(x(1))+2;
 H(2,1) = H(1,2);
 H(2,2) = 200+2;
end;
end 
