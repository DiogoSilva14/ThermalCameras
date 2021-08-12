import bluetooth
import cv2
import sys
import numpy as np

target_name = "Lepton IR"
target_address = None

nearby_devices = bluetooth.discover_devices()

for bdaddr in nearby_devices:
    if target_name == bluetooth.lookup_name( bdaddr ):
        target_address = bdaddr
        break

print(target_address)

port = 1

sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.connect((target_address, port))

frame = []

still_connected = True

while(still_connected):
	try:
		data = sock.recv(1000)
	except:
		still_connected = False

	for byte in data:
		frame.append(byte)
		
		
		#cv2.imshow("Lepton IR", decoded)
	
#		if byte == ord('<'):
#			frame = ''
#			print("Received BOF")
#		elif byte == ord('>'):
#			print("Received EOF")
#			
#			print(frame)
#			
#		else:
#			frame += str(byte)

decoded = cv2.imdecode(np.frombuffer(frame, np.uint8), -1)
			
print(type(decoded))
		
