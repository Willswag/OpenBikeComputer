
import serial
import json
import matplotlib.pyplot as plt
import numpy as np

ax = plt.axes()



plt.grid()



plt.title('How to plot a vector in matplotlib ?',fontsize=10)

#plt.show()

ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 2000000
ser.flushInput()

n = 5
i = 0
acx = 0
acy = 0
AX = 0
AY =0 



while True:
	i=i+1
	ser_bytes = ser.readline()
	
	decode_bytes = ser_bytes[0:len(ser_bytes)-1].decode("utf-8")	
	json_acceptable_string = decode_bytes	
	#print(decode_bytes)

	data = json.loads(json_acceptable_string)
	acx  = data["AcX"]+acx
	acy = data["AcY"]+acy
		
	if i%5==0:
		X=acx/5
		Y = acy/5
		ax.arrow(0.0, 0.0, X, Y, head_width=0.005, head_length=0.01, fc='lightblue', ec='black')
		plt.draw()
		#plt.autoscale()
		plt.pause(0.05)
'''		
	try:
		i=i+1
		ser_bytes = ser.readline()
	
		decode_bytes = ser_bytes[0:len(ser_bytes)-1].decode("utf-8")	
		json_acceptable_string = decode_bytes	
		print(decode_bytes)

		data = json.loads(json_acceptable_string)
		acx  = data["AcX"]+acx
		acy = data["AcY"]+acy
		
		if i==n:
			ax.arrow(0.0, 0.0, AX, AY, head_width=0.5, head_length=0.7, fc='lightblue', ec='black')
		plt.draw()
		plt.autosize()
		plt.pause(0.05)
		
	except:
		print("exiting...")
		break
		'''
