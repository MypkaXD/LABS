function [xv,fv,iters,fevs,exitflag]=ch_quadint(Fun,xb,h,eps,delta,MaxIters)

%% (C) ЧЕРНОВ А.В., 2009
%% ПРОЦЕДУРА МИНИМИЗАЦИИ УНИМОДАЛЬНОЙ ФУНКЦИИ
%% МЕТОДОМ КВАДРАТИЧНОЙ ИНТЕРПОЛЯЦИИ
%% ФОРМАТ ЗАПУСКА:
%% [xv,fv,iters,fevs,exitflag]=ch_quadint(Fun,xb,h,eps,delta,MaxIters)
%% ЗДЕСЬ:
%% Fun - УКАЗАТЕЛЬ НА ФУНКЦИЮ
%% xb - НАЧАЛЬНАЯ ПРОБНАЯ ТОЧКА
%% h - НАЧАЛЬНЫЙ ШАГ СМЕЩЕНИЯ ПО x
%% eps - КОНСТАНТА РАЗЛИЧИМОСТИ ПО ФУНКЦИИ (ПАРАМЕТР МЕТОДА)
%% delta - ТОЧНОСТЬ ПО АРГУМЕНТУ
%% MaxIters - МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ИТЕРАЦИЙ
%% xv - НАЙДЕННОЕ РЕШЕНИЕ
%% (СЕРЕДИНА ПОСЛЕДНЕГО ОТРЕЗКА НЕОПРЕДЕЛЕННОСТИ)
%% fv - ЗНАЧЕНИЕ ФУНКЦИИ Fun(xv)
%% iters - КОЛИЧЕСТВО ПРОДЕЛАННЫХ ИТЕРАЦИЙ
%% fevs - КОЛИЧЕСТВО ВЫЧИСЛЕНИЙ ФУНКЦИИ
%% exitflag - КОД ЗАВЕРШЕНИЯ:
%% -1 - ДОСТИГНУТО МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ИТЕРАЦИЙ
%% (РЕШЕНИЯ НЕ НАЙДЕНО)
%% 1 - ПОЛУЧЕНО ПРИБЛИЖЕННОЕ РЕШЕНИЕ С ЗАДАННОЙ ТОЧНОСТЬЮ

exitflag=-1; iters=0;

x=0; f=0; % ИНИЦИИРУЕМ ПЕРЕМЕННЫЕ

x(1)=xb; % НАЧАЛЬНАЯ ПРОБНАЯ ТОЧКА

OpNum=0; % НОМЕР ОПЕРАЦИИ, ОПРЕДЕЛЯЕТ РАЗВЕТВЛЕНИЕ АЛГОРИТМА

fevs=0;

for k=1:MaxIters,
    iters=iters+1;

    if iters>MaxIters,
        break;
    end;

    x(2)=x(1)+h; % ПРОБНЫЕ ТОЧКИ
    f(1)=feval(Fun,x(1)); %% ЗНАЧЕНИЕ ФУНКЦИИ В ПРОБНЫХ ТОЧКАХ
    f(2)=feval(Fun,x(2));
    
    if f(1)>f(2),
        x(3)=x(2)+h;
    else
        x(3)=x(1)-h;
    end;

    f(3)=feval(Fun,x(3));
    fevs=fevs+3;

    while 1==1,

        [fmin,imin]=min(f);
        xmin=x(imin);
        %% ВЫЧИСЛЯЕМ ТОЧКУ МИНИМУМА ИНТЕРПОЛЯЦИОННОГО ПОЛИНОМА
        %% МОЖНО ПОНИМАТЬ ЕЕ КАК ПРЕДПОЛАГАЕМУЮ ТОЧКУ МИНИМУМА
        alpha=0;
        beta=0;

        for j=1:3,
            j2=j+1;

            if j2>3, 
                j2=1; 
            end;

            j3=j2+1;

            if j3>3, 
                j3=1; 
            end;

            alpha=alpha+f(j)*(x(j2)^2-x(j3)^2);
            beta=beta+f(j)*(x(j2)-x(j3));

        end;

        if beta==0,
            x(1)=xmin;
            OpNum=1;
            break;
        end;

        xp=(0.5*alpha)/beta
        fp=feval(Fun,xp);
        fevs=fevs+1;

        %% ПРОВЕРЯЕМ КРИТЕРИЙ ОСТАНОВА:
        afp=abs(fp);
        
        if afp<1,
            afp=1;
        end;

        axp=abs(xp);

        if axp<1,
            axp=1;
        end;

        Cond1=(abs(fmin-fp)<eps);
        Cond2=(abs(xmin-xp)<delta);

        if (Cond1&Cond2),
            OpNum=2;
            exitflag=1;
            break;
        end;

        %% ГРУППИРУЕМ ТОЧКИ В ПОРЯДКЕ ВОЗРАСТАНИЯ:
        [x,xInd]=sort(x);
        f=f(xInd);

        if (xp<x(1))|(xp>x(3)),
            x(1)=xp; OpNum=1;
            break;
        end;

        if xp<x(2),
            x(3)=x(2); f(3)=f(2);
            x(2)=xp; f(2)=fp;
        elseif xp==x(2),
            f(2)=fp;
        else
            x(1)=x(2); 
            f(1)=f(2);
            x(2)=xp;
            f(2)=fp;
        end;

        iters=iters+1;

        if iters>MaxIters,
            OpNum=2;
            break;
        end;

    end; %% while

    if OpNum==2,
        break;
    end;

end; %% for k

xv=xp; fv=fp; fevs=fevs+1;
fv=feval(Fun,xp);