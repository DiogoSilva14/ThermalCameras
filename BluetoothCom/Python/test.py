import bluetooth
from time import time

target_name = "ESP32IR"
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

sock.send("Hello!")
milliseconds = int(time()*1000)

data = sock.recv(1000)

delay = int(time()*1000) - milliseconds

print("Delay: " + str(delay) + " ms")

print(data)
