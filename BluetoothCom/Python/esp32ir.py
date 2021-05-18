import bluetooth

class melexisIR:
    address = ""

    def __init__(self):
        nearby_devices = bluetooth.discover_devices()
        for bdaddr in nearby_devices:
            nearby_devices = bluetooth.discover_devices()
            if "ESP32IR" == bluetooth.lookup_name(bdaddr):
                self.address = bdaddr
                break