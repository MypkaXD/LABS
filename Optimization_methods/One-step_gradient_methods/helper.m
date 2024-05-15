function a=helper(rp,varargin)
% helper - ПРОЦЕДУРА РИСОВАНИЯ ЛИНИИ
% В ВИДЕ ПОСЛЕДОВАТЕЛЬНОСТИ ТОЧЕК
% ФОРМАТ ЗАПУСКА:
% helper(rp,X0,Y0), ГДЕ
% rp - РАДИУС КАЖДОЙ ТОЧКИ,
% X0,Y0 - МАССИВЫ КООРДИНАТ ТОЧЕК
X0=varargin{1};
Y0=varargin{2};
NN=length(varargin{1});
for k=1:1:NN
t=0:0.1:2*pi;
xt=X0(k)+rp*cos(t);
yt=Y0(k)+rp*sin(t);
fill(xt,yt,0);
end