function [xv,fv,iters,exitflag,count, FN, n]=fibon(Fun,a,b,eps,delta)

%% ПРОЦЕДУРА МИНИМИЗАЦИИ УНИМОДАЛЬНОЙ ФУНКЦИИ НА ОТРЕЗКЕ
%% МЕТОДОМ ФИБОНАЧЧИ
%% ФОРМАТ ЗАПУСКА:
%% [xv,fv,iters,exitflag,count]=fibon(Fun,a,b,eps,delta)
%% ЗДЕСЬ:
%% Fun - УКАЗАТЕЛЬ НА ФУНКЦИЮ
%% a,b - КОНЦЫ ОТРЕЗКА (НАЧАЛЬНОГО ОТРЕЗКА НЕОПРЕДЕЛЕННОСТИ)
%% eps - ТОЧНОСТЬ ПО АРГУМЕНТУ
%% delta - КОНСТАНТА РАЗЛИЧИМОСТИ 0 < delta << eps
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

N = (b-a)/eps;

f(1) = 1;
f(2) = 2;

n = 2;

while (f(n) < N)
    n = n + 1;
    f(n) = f(n-1) + f(n-2);
    disp(f(n));
    disp(n);
end

N = n;

FN = f(n);

ak = a;
bk = b;

yk = ak+(bk-ak)*(f(N-2)/f(N));
zk = bk-(bk-ak)*(f(N-2)/f(N));

fyk=feval(Fun,yk);
fzk=feval(Fun,zk);

while (iters ~= N-3)

    if (fyk <= fzk)
        bk = zk;
        zk = yk;
        fzk = fyk;
        yk = ak + (bk-ak)*(f(N-iters-3)/f(N-iters-1));
        fyk = feval(Fun, yk);
    else
        ak = yk;
        yk = zk;
        fyk = fzk;
        zk = bk - (bk-ak)*(f(N-iters-3)/f(N-iters-1));
        fzk = feval(Fun, zk);
    end;
    
    iters = iters + 1;

end;  

zk = yk + delta;

if (fyk <= fzk)
    bk = zk;
else 
    ak = yk;
end

exitflag = 1;

xv=0.5*(ak+bk);
fv=feval(Fun,xv); 
count=fcount;