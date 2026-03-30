import matplotlib.pyplot as plt

# Время для одного потока (T0)
T_0 = 0.193095

T1 = []
T2 = []
T3 = []

chank1 = []
chank2 = []
chank3 = []
cnt =0

with open("task3/prov1/results2.txt") as f:
    next(f)  # пропускаем заголовок
    for line in f: 
        N, k, T, mode, chunk = map(float, line.split())

        mode = int(mode)
        chunk = int(chunk)
        

        if mode == 11:
            chank1.append(chunk)
            T1.append(T)
        elif mode == 12:
            chank2.append(chunk)
            T2.append(T)
        elif mode == 13:
            chank3.append(chunk)
            T3.append(T)
        cnt+=1

# Рассчитаем эффективность: E = T0 / (T * k)
# Для этого нужно знать k для каждой строки, допустим фиксируем k=8 (или можно добавить список K для каждого режима)
k_val = 4  # например, если анализируем для k=8 потоков

E1 = [T_0 / (t * k_val) for t in T1]
E2 = [T_0 / (t * k_val) for t in T2]
E3 = [T_0 / (t * k_val) for t in T3]

# Построим график
plt.figure(figsize=(8,5))
plt.plot(chank1, E1, marker='o', label='Static (11)')
plt.plot(chank2, E2, marker='s', label='Dynamic (12)')
plt.plot(chank3, E3, marker='^', label='Guided (13)')

# Найдем лучший режим и chunk
all_eff = E1 + E2 + E3
all_chunks = chank1 + chank2 + chank3
all_modes = ['Static']*len(E1) + ['Dynamic']*len(E2) + ['Guided']*len(E3)

best_idx = all_eff.index(max(all_eff))
best_mode = all_modes[best_idx]
best_chunk = all_chunks[best_idx]
best_E = all_eff[best_idx]

plt.scatter(best_chunk, best_E, color='red', s=100, zorder=5)
plt.text(best_chunk, best_E + 0.02, f'Best: {best_mode} chunk={int(best_chunk)}', color='red')

plt.xlabel("Chunk size")
plt.ylabel("Efficiency")
plt.title("Efficiency vs Chunk size")
plt.grid(True)
plt.legend()
plt.show()