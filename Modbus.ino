
#include <Arduino.h>    
#include <ModbusRTU.h>  
#include <DHT.h>
#define SLAVE_ID 1
#define DHTPIN 4
#define DHTTYPE DHT11
//Globale KLasse für die bibliothek
ModbusRTU modbus_modul;
DHT dht_sensor(DHTPIN, DHTTYPE);
// MODBUS REGISTER ADRESSEN
const int REG_temperatur = 0;
const int REG_feuchtigkeit=1;
void hardware(){
  //geben vor wie der dattenfluss ausehen wird
Serial.begin(9600,SERIAL_8N1);
//DHT bibliothek
dht_sensor.begin();
// koppeln die SLave ID mit dem modbus
modbus_modul.begin(&Serial);
modbus_modul.slave(SLAVE_ID);
//sagt dem Modbus welche Register wir gewählt haben 
  modbus_modul.addHreg(REG_temperatur);
  modbus_modul.addHreg(REG_feuchtigkeit);
}
void sensor_lesen(){
  //static weil wir wollen dass das nur eunmal erstellt wird
  //unsigned damit wirr negative zahlen ignorieren und somit wenn overflowe kommt wir einfach mit 
  //Modulo weitermachen können
  static unsigned long letztes_update = 0;
// wir machen diesen ganzen Zirkus hier im if weil es FLASH spart wir müssen
//nichts vergleichen
  if (millis()-letztes_update >2000){
    letztes_update = millis();
    float temp = dht_sensor.readTemperature();
    float feucht = dht_sensor.readHumidity();
    // ich muss schauen wo man flash reduzierenen kann
     if (!isnan(temp) && !isnan(feucht)){
      uint16_t temp_ganzzahl = (uint16_t)(temp * 10.0);
      uint16_t feucht_ganzzahl = (uint16_t) (feucht);
      // wir legen hier die daten in dem Register wieder
      modbus_modul.Hreg(REG_temperatur, temp_ganzzahl);
      modbus_modul.Hreg(REG_feuchtigkeit, feucht_ganzzahl);
     }
  }
}
void verarbeite_modbus_anfragen() {
  // wichtig für CRC summe
  modbus_modul.task();
}
void setup() {
  // put your setup code here, to run once:
  hardware();
}

void loop() {
  // put your main code here, to run repeatedly:
  sensor_lesen();
  verarbeite_modbus_anfragen();
  delay(10);
}
