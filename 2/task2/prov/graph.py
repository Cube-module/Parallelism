import matplotlib.pyplot as plt

threads_40 = []
times_40 = []

with open("task2/prov/results.txt") as f:
    next(f)  # пропускаем заголовок
    # идем по каждой строке файла
    for line in f: 
        N, k, T = map(float, line.split()) # распоковываем значения

        threads_40.append(int(k)) # потоки 20000
        times_40.append(T) # время 20000 связь с потоками
            

T1_40 = times_40[0]

S_40 = [T1_40 / t for t in times_40]

ideal_S = [1, 2, 4, 7, 8, 16, 20, 40]
#  ------------------------------------------------------------------------ Ускорение
plt.figure()

plt.plot(threads_40, S_40, marker='o', label="N=40000000")

# идеальная линия
plt.plot(ideal_S, ideal_S, linestyle='--', label="Ideal (S=k)")

plt.xlabel("Threads")
plt.ylabel("Speedup")
plt.title("Speedup vs Threads")
plt.legend()
plt.grid()

plt.show()

#  ------------------------------------------------------------------------ Эффективность 

E_40 = [s / k for s, k in zip(S_40, threads_40)]

ideal_E = [1, 1 , 1 ,1 ,1 ,1 ,1 ,1]

plt.figure()

plt.plot(threads_40, E_40, marker='o', label="N=40000")

# идеальная линия
plt.plot(threads_40, ideal_E, linestyle='--', label="Ideal (E=1)")

plt.xlabel("Threads")
plt.ylabel("Effectivity")
plt.title("Effectivity vs Threads")
plt.legend()
plt.grid()

plt.show()

