@echo off

REM Удаляем старый файл
del results2.txt

REM Заголовок
echo N k T mode chunk >> results2.txt

REM Режимы OpenMP
set MODES=static dynamic guided

REM Перебор N и k (твои значения оставлены)
for %%N in (200) do (
    for %%k in (4) do (
        
        REM Перебор режимов
        for %%M in (%MODES%) do (
            REM Перебор chunk от 1 до 10
            for /L %%C in (1,1,10) do (
                REM Устанавливаем стратегию OpenMP
                set OMP_SCHEDULE=%%M,%%C

                echo Running N=%%N k=%%k schedule=%%M chunk=%%C

                REM Запуск программы и запись в файл
                build\main2.exe %%N %%k 
            )
        )
    )
)

pause