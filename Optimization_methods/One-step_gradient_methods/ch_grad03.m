% [xm,fv,exitflag,iters,vX,vZ]=ch_grad01(Fun,xb,maxiter,tolg)
% ГДЕ
% Fun -- УКАЗАТЕЛЬ НА ЦЕЛЕВУЮ ФУНКЦИЮ
% xb -- НАЧАЛЬНАЯ ТОЧКА (ВЕКТОР-СТРОКА)
% alpha -- ШАГ ОДНОМЕРНОГО ПОИСКА
% maxiter -- МАКСИМАЛЬНО ДОПУСТИМОЕ КОЛИЧЕСТВО ИТЕРАЦИЙ
% tolg -- ТОЧНОСТЬ ПО ГРАДИЕНТУ
% xm -- НАЙДЕННАЯ ТОЧКА МИНИМУМА (ИЛИ ПОСЛЕДНЯЯ ТОЧКА)
% fv -- ЗНАЧЕНИЕ ЦЕЛЕВОЙ ФУНКЦИИ В ТОЧКЕ xm
% exitflag -- КОД ЗАВЕРШЕНИЯ
% = 1 -- ПРИБЛИЖЕННОЕ РЕШЕНИЕ НАЙДЕНО
% =-1 -- ДОСТИГНУТО МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ИТЕРАЦИЙ
% iters -- КОЛИЧЕСТВО ПРОДЕЛАННЫХ ИТЕРАЦИЙ
% НЕОБЯЗАТЕЛЬНЫЕ ПАРАМЕТРЫ:
% vX -- ТРАЕКТОРИЯ ПОИСКА (ПОСЛЕДОВАТЕЛЬНОСТЬ ТОЧЕК)
% vZ -- ПОСЛЕДОВАТЕЛЬНОСТЬ СООТВЕТСТВУЮЩИХ ЗНАЧЕНИЙ ФУНКЦИИ
function [xm,fv,exitflag,iters,vX,vZ]=ch_grad01(Fun,xb,string,maxiter,tolg)
XXX(1,:)=xb; ZZZ(1)=feval(Fun,xb);
% ИНИЦИИРУЕМ МАССИВ ДЛЯ ТРАЕКТОРИИ ПОИСКА
exitflag=-1; iters=0; xk=xb; n=length(xb);
itNumFun=inline(string,'k');
for itNum=1:maxiter
[f,g]=feval(Fun,xk);
alpha=itNumFun(itNum);
if nargout>5
XXX(itNum,:)=xk; ZZZ(itNum)=f;
end
ng=norm(g);
if ng<tolg % КРИТЕРИЙ ОСТАНОВА
exitflag=1; iters=itNum; break;
end
dx=alpha*g;
while (1==1) % ОБЕСПЕЧИВАЕМ УБЫВАНИЕ ФУНКЦИИ
ft=feval(Fun,xk-dx);
if ft<f
break;
end
dx=0.5*dx; % КОРРЕКЦИЯ ШАГА
end
xk=xk-dx;
if itNum==maxiter
exitflag=0; iters=itNum;
end
end % for itNum
xm=xk; fv=f;
if nargout>5
vX=XXX; vZ=ZZZ;
end
