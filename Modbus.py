import time

from pymodbus.client import ModbusSerialClient as ModbusClient

PORT = 'COM5'
# baudratrat Uebertragungsgeschwindigkeit 9600 bits /s muss zu esp 32 passen
# bytesize 8datenbits
#parity  N = keins
# stopbits sagt stoppt

slave = ModbusClient(port = PORT, baudrate =9600, stopbits=1, bytesize=8, parity='N')

def Modbus():
    if not slave.connect():
        print(f"FEHLER: Verbindung auf {PORT} konnte nicht hergestellt werden ESP32 eingesteckt? ")
        return
    try:
        # address= 0 ist startadresse
        # count wir nehmen die ersten 2 register
        # unsere device_id ist das erste device/slave
        result = slave.read_holding_registers(address=0, count=2, device_id=1)
        
        if not result.isError():
            temp_raw = result.registers[0]
            temperatur = temp_raw/10.0
            feuchtigkeit = result.registers[1]
            print(f" Temperatur:{temperatur} C | Feuchtigkeit {feuchtigkeit}%")
        else:
            print(f" Der ESP32 hat ein Fehler gemeldet:{result}")
        
    except Exception as e:
        print(f"Fehler bei der Kommunikation: {e}")
    finally:
        # wir schließen hier die Leitung sauber ab
        slave.close()
        
print("Starte Modbus-Abfrage an den ESP32")

while True:
    Modbus()
    time.sleep(2)

i
