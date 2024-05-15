clear all;
xx=-2:0.05:2; yy=-2:0.05:2; vv=0:100:500;
lx=length(xx); ly=length(yy);
for i=1:lx,
for j=1:ly,
zz(j,i)=objfun01([xx(i) yy(j)]);
end;
end;
hold on;
[cs,h]=contour(xx,yy,zz,vv); set(h,'LineWidth',1.5);
clabel(cs,h,'labelspacing',1500);
vv=0:5:20;
[cs,h]=contour(xx,yy,zz,vv);
clabel(cs,h,'labelspacing',5500);
colorbar;
hold off;
