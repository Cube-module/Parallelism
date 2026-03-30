@echo off

del results.txt

echo N k T T_init >> results.txt

for %%N in (200) do (
  for %%k in (1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16) do (
    echo Running N=%%N k=%%k
    build\main1.exe %%N %%k
  )
)

pause