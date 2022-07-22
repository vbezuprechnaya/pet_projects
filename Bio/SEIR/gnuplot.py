import matplotlib.pyplot as plt

file = open("data.txt", "r")

data = file.readlines()

s_data, e_data, i_data, r_data = [], [], [], []

for i in data:
	i = i[:-1]
	s, e, i, r = list(map(float, i.split(' ')))
	s_data.append(s)
	e_data.append(e)
	i_data.append(i)
	r_data.append(r)

steps = list(range(len(data)))

plt.plot(steps, s_data, label = "S")
plt.plot(steps, e_data, label = "E")
plt.plot(steps, i_data, label = "I")
plt.plot(steps, r_data, label = "R")
plt.legend()

plt.savefig("data.png")
