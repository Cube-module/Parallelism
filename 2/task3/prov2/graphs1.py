import matplotlib.pyplot as plt

threads_20 = []
times_20 = []

with open("task3/prov2/results.txt") as f:
    next(f)  # пропускаем заголовок
    # идем по каждой строке файла
    for line in f: 
        N, k, T = map(float, line.split()) # распоковываем значения

        threads_20.append(int(k)) # потоки 20000
        times_20.append(T) # время 20000 связь с потоками
            

T1_20 = times_20[0]

S_20 = [T1_20 / t for t in times_20]

ideal_S = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]
#  ------------------------------------------------------------------------ Ускорение
plt.figure()

plt.plot(threads_20, S_20, marker='o', label="N=200")

# идеальная линия
plt.plot(ideal_S, ideal_S, linestyle='--', label="Ideal (S=k)")

plt.xlabel("Threads")
plt.ylabel("Speedup")
plt.title("Speedup vs Threads")
plt.legend()
plt.grid()

plt.show()

#  ------------------------------------------------------------------------ Эффективность 

E_20 = [s / k for s, k in zip(S_20, threads_20)]

ideal_E = [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]

plt.figure()

plt.plot(threads_20, E_20, marker='o', label="N=200")

# идеальная линия
plt.plot(threads_20, ideal_E, linestyle='--', label="Ideal (E=1)")

plt.xlabel("Threads")
plt.ylabel("Effectivity")
plt.title("Effectivity vs Threads")
plt.legend()
plt.grid()

plt.show()
