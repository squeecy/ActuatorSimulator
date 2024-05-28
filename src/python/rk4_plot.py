import matplotlib.pyplot as plt
import numpy as np

rk4_actual = []
euler_actual = []

rk4 = []
euler = []

with open("res/rk4_data.txt", "r") as file:
    for line in file:
        parts = line.strip().split(",")

        if len(parts) == 2:
            rk4_actual.append(float(parts[0]))
            rk4.append(float(parts[1]))

with open("res/euler_data.txt", "r") as file:
    for line in file:
        parts = line.strip().split(",")

        if len(parts) == 2:
            euler_actual.append(float(parts[0]))
            euler.append(float(parts[1]))

rk4_actual_plt = np.array(rk4_actual)
rk4_plt = np.array(rk4)

euler_actual_plt = np.array(euler_actual)
euler_plt = np.array(euler)


plt.plot(rk4_actual_plt, label='RK4 Actual')
plt.plot(rk4_plt, label='RK4')
plt.plot(euler_actual, label='Euler Actual')
plt.plot(euler_plt, label='Euler')
plt.legend()

plt.show()
