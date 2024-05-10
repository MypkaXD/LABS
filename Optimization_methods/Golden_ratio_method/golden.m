function [xv,fv,iters,exitflag,count]=golden(Fun,a,b,eps,MaxIters)

%% ПРОЦЕДУРА МИНИМИЗАЦИИ УНИМОДАЛЬНОЙ ФУНКЦИИ НА ОТРЕЗКЕ
%% МЕТОДОМ ЗОЛОТОГО СЕЧЕНИЯ
%% ФОРМАТ ЗАПУСКА:
%% [xv,fv,iters,exitflag,count]=golden(Fun,a,b,eps,MaxIters)
%% ЗДЕСЬ:
%% Fun - УКАЗАТЕЛЬ НА ФУНКЦИЮ
%% a,b - КОНЦЫ ОТРЕЗКА (НАЧАЛЬНОГО ОТРЕЗКА НЕОПРЕДЕЛЕННОСТИ)
%% eps - ТОЧНОСТЬ ПО АРГУМЕНТУ
%% MaxIters - МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ИТЕРАЦИЙ
%% xv - НАЙДЕННОЕ РЕШЕНИЕ
%% (СЕРЕДИНА ПОСЛЕДНЕГО ОТРЕЗКА НЕОПРЕДЕЛЕННОСТИ)
%% fv - ЗНАЧЕНИЕ ФУНКЦИИ Fun(x_)
%% iters - КОЛИЧЕСТВО ПРОДЕЛАННЫХ ИТЕРАЦИЙ
%% exitflag - КОД ЗАВЕРШЕНИЯ:
%% count - КОЛИЧЕСТВО ВЫЧИСЛЕНИЙ ФУНКЦИЙ
%% -1 - ДОСТИГНУТО МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ИТЕРАЦИЙ
%% (РЕШЕНИЯ НЕ НАЙДЕНО)
%% 1 - ПОЛУЧЕНО ПРИБЛИЖЕННОЕ РЕШЕНИЕ С ЗАДАННОЙ ТОЧНОСТЬЮ

exitflag = -1;
iters = 0;
global fcount;
fcount = 0;

ak=a; bk=b; %% НАЧАЛЬНЫЙ ОТРЕЗОК НЕОПРЕДЕЛЕННОСТИ

tau = (1+sqrt(5))/2;

yk = bk-(bk-ak)/tau;
zk = ak+(bk-ak)/tau;

fyk=feval(Fun,yk);
fzk=feval(Fun,zk);

for iters=1:MaxIters,

    if fyk<=fzk,
        bk=zk;
        zk=yk;
        fzk=fyk;
        yk=bk-(bk-ak)/tau;
        fyk=feval(Fun,yk);
    else
        ak=yk;
        yk=zk;
        fyk=fzk;
        zk=ak+(bk-ak)/tau;
        fzk=feval(Fun,zk);
    end;

    d=abs(bk-ak); %% ВЫЧИСЛЯЕМ ЕГО ДЛИНУ

    if d<eps, %% ПРОВЕРЯЕМ КРИТЕРИЙ ОСТАНОВА
        exitflag=1;
    break;
        end;
        
end;  

xv=0.5*(ak+bk);
fv=feval(Fun,xv); 
count=fcount;