@echo off

del results.txt

echo N k T T_init >> results.txt

for %%N in (20000 40000) do (
  for %%k in (1 2 4 7 8 16 20 40) do (
    echo Running N=%%N k=%%k
    build\pr.exe %%N %%k
  )
)

pause