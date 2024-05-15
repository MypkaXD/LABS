[xm,fm,exitflag,iters,vX,vZ]=ch_grad03(@grad,[-2 2],'1/(1+log(k))',15000,0.01);
vX=XX(:,1); vY=XX(:,2); nL=0; Lz=length(vZ);
for k=1:1:min([10 iters])
 nL=nL+1;
 vZZ(nL)=vZ(k);
end
for k=11:100:iters
 nL=nL+1;
 vZZ(nL)=vZ(k);
end
xx=-2.5:0.05:2.5;
yy=-2.5:0.05:2.5;
lx=length(xx);
ly=length(yy);
for i=1:lx
for j=1:ly
 zz(j,i)=grad([xx(i) yy(j)]);
end
end
hold on;
[cs,h]=contour(xx,yy,zz,vZZ);
clabel(cs,h,'labelspacing',5000);
set(h,'LineWidth',1.5,'LineStyle','-');
helper(0.02,vX,vY);
line(vX,vY,'Color','r','LineWidth',1.7);
hold off; 
