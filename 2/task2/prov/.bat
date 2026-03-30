@echo off

del results.txt

echo N k T >> results.txt

for %%N in (40000000) do (
  for %%k in (1 2 4 7 8 16 20 40) do (
    echo Running N=%%N k=%%k
    build\pt.exe %%N %%k
  )
)

pause