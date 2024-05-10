function [xv,fv,iters,exitflag, count]=half_div(Fun,a,b,eps,MaxIters)

%% ПРОЦЕДУРА МИНИМИЗАЦИИ УНИМОДАЛЬНОЙ ФУНКЦИИ НА ОТРЕЗКЕ
%% МЕТОДОМ ПОЛОВИННОГО ДЕЛЕНИЯ
%% ФОРМАТ ЗАПУСКА:
%% [xv,fv,iters,exitflag]=half_div(Fun,a,b,eps,MaxIters)
%% ЗДЕСЬ:
%% Fun - УКАЗАТЕЛЬ НА ФУНКЦИЮ
%% a,b - КОНЦЫ ОТРЕЗКА (НАЧАЛЬНОГО ОТРЕЗКА НЕОПРЕДЕЛЕННОСТИ)
%% eps - ТОЧНОСТЬ ПО АРГУМЕНТУ
%% MaxIters - МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ИТЕРАЦИЙ
%% xv - НАЙДЕННОЕ РЕШЕНИЕ
%% (СЕРЕДИНА ПОСЛЕДНЕГО ОТРЕЗКА НЕОПРЕДЕЛЕННОСТИ)
%% fv - ЗНАЧЕНИЕ ФУНКЦИИ Fun(xv)
%% iters - КОЛИЧЕСТВО ПРОДЕЛАННЫХ ИТЕРАЦИЙ
%% count - КОЛИЧЕСТВО ВЫЧИСЛЕНИЙ ФУНКЦИЙ
%% exitflag - КОД ЗАВЕРШЕНИЯ:
%% -1 - ДОСТИГНУТО МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ИТЕРАЦИЙ
%% (РЕШЕНИЯ НЕ НАЙДЕНО)
%% 1 - ПОЛУЧЕНО ПРИБЛИЖЕННОЕ РЕШЕНИЕ С ЗАДАННОЙ ТОЧНОСТЬЮ

exitflag=-1; iters=0;

global fcount

fcount=[];

ak=a; bk=b; 

xk=0.5*(ak+bk); 
fxk=feval(Fun,xk);

for iters=1:MaxIters

    yk=0.5*(ak+xk);
    zk=0.5*(xk+bk);

    fyk=feval(Fun,yk); 
    fzk=feval(Fun,zk);

    if fxk>=fyk
        bk=xk;
        xk=yk;
        fxk=fyk;
    else
        if fzk<fxk
            ak = xk;
            xk = zk;
            fxk = fzk;
        else
            ak=yk;
            bk=zk;
        end
    end

    d=abs(bk-ak); 

    if d<eps 
        exitflag=1;
        break;
    end
end

xv=0.5*(ak+bk);
fv=feval(Fun,xv);
count = fcount;
