#!/usr/bin/env python3 

# importing the required module
import matplotlib.pyplot as plt


def numero_sottostringa(str_ingr, posLettera):

	str_numero = ""
	posLettera = posLettera + 1
	i = posLettera
	while i < len(str_ingr):
		if str_ingr[i].isdigit() or str_ingr[i] == '.' or str_ingr[i] == '-':
			str_numero = str_numero + str_ingr[i]
		else:
			break
		i = i+1
	
	return float(str_numero)


x = []
y = []
z = []
v = []

xt = []
yt = []
zt = []
vt = []

com_file = open("com_file.txt", "r")

for line in com_file:
  x.append(numero_sottostringa(line, line.find('X')))
  y.append(numero_sottostringa(line, line.find('Y')))
  z.append(numero_sottostringa(line, line.find('Z')))
  v.append(numero_sottostringa(line, line.find('V')))

com_file.close()




 
# plotting the points
plt.plot(x, y)
 
# naming the x axis
plt.xlabel('x - axis')
# naming the y axis
plt.ylabel('y - axis')
 
# giving a title to my graph
plt.title('CNC graph!')
 
# function to show the plot
plt.show()
