#!/bin/bash

# Удаляем старый файл
rm -f results2.txt

# Заголовок
echo "N k T mode chunk" >> results2.txt

# Режимы OpenMP
MODES=("static" "dynamic" "guided")

# Перебор N и k
for N in 3000
do
  for k in 25
  do
    # Перебор режимов
    for M in "${MODES[@]}"
    do
      # Перебор chunk от 1 до 160
      for C in 1 10 20 40 80 160
      do
        # Устанавливаем стратегию OpenMP
        export OMP_SCHEDULE="$M,$C"

        echo "Running N=$N k=$k schedule=$M chunk=$C"

        # Запуск программы
        ./build/main2 $N $k
      done
    done
  done
done