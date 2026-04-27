import matplotlib.pyplot as plt

threads_20 = []
times_20 = []

threads_40 = []
times_40 = []


with open("results.txt") as f:
    next(f)  # пропускаем заголовок
    # идем по каждой строке файла
    for line in f: 
        N, k, T = map(float, line.split()) # распоковываем значения


        if int(N) == 20000:
            threads_20.append(int(k)) # потоки 20000
            times_20.append(T) # время 20000 связь с потоками
        elif int(N) == 40000:
            threads_40.append(int(k))
            times_40.append(T)

T1_20 = times_20[0]
T1_40 = times_40[0]

S_20 = [T1_20 / t for t in times_20]
S_40 = [T1_40 / t for t in times_40]

ideal_S = [1, 2, 4, 7, 8, 16, 20, 40]
#  ------------------------------------------------------------------------ Ускорение
plt.figure()

plt.plot(threads_20, S_20, marker='o', label="N=20000")
plt.plot(threads_40, S_40, marker='o', label="N=40000")

# идеальная линия
plt.plot(ideal_S, ideal_S, linestyle='--', label="Ideal (S=k)")

plt.xlabel("Threads")
plt.ylabel("Speedup")
plt.title("Speedup vs Threads")
plt.legend()
plt.grid()

plt.savefig("graph1.png")
plt.show()

#  ------------------------------------------------------------------------ Эффективность 

E_20 = [si / ki for si, ki in zip(S_20, threads_20)]
E_40 = [si / ki for si, ki in zip(S_40, threads_40)]

ideal_E = [1, 1 , 1 ,1 ,1 ,1 ,1 ,1]

plt.figure()

plt.plot(threads_20, E_20, marker='o', label="N=20000")
plt.plot(threads_40, E_40, marker='o', label="N=40000")

# идеальная линия
plt.plot(threads_20, ideal_E, linestyle='--', label="Ideal (E=1)")

plt.xlabel("Threads")
plt.ylabel("Effectivity")
plt.title("Effectivity vs Threads")
plt.legend()
plt.grid()

plt.savefig("graph2.png")
plt.show()

#  ------------------------------------------------------------------------ E + T

M_20 = [s * e * 1.5 for s, e in zip(S_20, E_20)]

plt.figure()

plt.plot(threads_20, M_20, marker='o', label="M = S * E")

plt.xlabel("Threads")
plt.ylabel("Combined metric")
plt.title("Combined metric vs Threads")
plt.legend()
plt.grid()

plt.savefig("graph3.png")
plt.show()