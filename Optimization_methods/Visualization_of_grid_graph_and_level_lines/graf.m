clear all
zm = 50; xx=-2:0.04:2; yy=-2:0.04:2;
lx=length(xx); ly=length(yy);
for i=1:lx
    for j=1:ly
        zz(j,i)=objfun01a([xx(i) yy(j)], zm);
    end
end
mesh(xx,yy,zz); view(-20,50);
