#!/usr/bin/env python3

import serial
import time

nomefile = input("inserire nome del file: ")

#stringa_comandi = open(nomefile, "r").read()

#print(stringa_comandi)



#arduino = serial.Serial('/dev/ttyUSB0',9600)
#time.sleep(2)

file = open(nomefile, "r")
file_mod = open(nomefile + "_mod", "w")

insieme_comandi = ""
comando_mod = ""

while 1 :
	comando = file.readline()
	if comando == "":
		break
	for c in range(len(comando)-1):	
		if comando[c] == "F" or comando[c] == ";":
			break
				
		comando_mod += comando[c]
			
		if comando[c] == "G" or comando[c] == "M":
			if comando[c+1] != "9":
				comando_mod += "0"
		
	comando_mod += ";"			
	insieme_comandi = insieme_comandi + comando_mod
	comando = ""
	comando_mod = ""

file.close()

file_mod.write(insieme_comandi)
file_mod.close()


print(insieme_comandi)
