from matplotlib import pyplot as plt
import numpy as np

data = np.loadtxt("/home/egor/codes/pendulum/kapitzas_thetas.txt", dtype=float)
ts = []
thetas = []
omegas = []
k = []
p = []
s = []
h = []

for element in data:
    t, theta, omega = element
    ts.append(t)
    thetas.append(theta)
    omegas.append(omega)
    h.append(1-np.cos(theta))

for theta in thetas:
    p.append(1*9.8*1*(1-np.cos(theta)))

for omega in omegas:
    k.append(1*1*(omega**2)/2)

for i in range(0, len(ts)):
    s.append(p[i]+k[i])

fig, ax = plt.subplots(figsize=(12,9), dpi=400)
ax.plot(ts, p)
ax.plot(ts, k)
fig.savefig('graph.png')
# plt.plot(ts, h)
# plt.plot(ts, s)
