#!/usr/bin/env python3

import serial
import time



	
arduino = serial.Serial('/dev/ttyUSB0',115200)
time.sleep(2)



ricevuto = ""

while(1):

	stringa_comandi = input("inserisci comando: ")

	
	if(stringa_comandi == "quit"):
		break;


				

	print("comando inviato: " + stringa_comandi)

	arduino.write(stringa_comandi.encode())

	while(ricevuto != "y"):
		ricevuto = ricevuto + arduino.read().decode()
	print(ricevuto)
			
	ricevuto = ""
