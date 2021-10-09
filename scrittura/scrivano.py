#!/usr/bin/env python3

import serial
import time

nomefile = input("inserire nome del file: ")

stringa_comandi = open(nomefile, "r").read()





arduino = serial.Serial('/dev/ttyUSB0',115200)
time.sleep(2)

i = 0
comando = ""
ricevuto = ""
tic = time.perf_counter()
while(i < len(stringa_comandi)): 
	while(stringa_comandi[i] != ';'):
		comando = comando + stringa_comandi[i]
		i = i+1
		
	comando = comando + ';'
	i = i+1
	print("comando inviato: " + comando)

	arduino.write(comando.encode())
	comando = ""
	while(ricevuto != "y"):
		ricevuto = ricevuto + arduino.read().decode()
	print(ricevuto)
		
	ricevuto = ""
toc = time.perf_counter()
tempo = toc - tic
print("tempo impiegato: ")
print(tempo)
