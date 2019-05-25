# Comet Blue Thermostat for ESP32

This library is made for this kind of bluetooth thermostats
![RGB LED Strip Driver Module Shield](https://raw.githubusercontent.com/MrKrabat/Comet-Blue-Thermostat/master/images/cometblue.jpg "Comet Blue Thermostat with Bluetooth LE 4.0")

**Since the Bluetooth functions of the ESP32 require a lot of memory space, the memory partitioning of the ESP32 must be changed.**

Example usage:
```c
#include <CometBlue.h>

// bluetooth MAC address of blue comet
CometBlue dev = CometBlue("d0:b5:c2:xx:xx:xx");

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    delay(50);
  }

  Serial.print("Connecting to device... ");
  // device pin is 0000 => 0
  if (!dev.connect("Test", 0)) {
    Serial.println("FAILED");
    while (1) {
      delay(100);
    }
  }
  Serial.println("SUCCESS");

  Serial.print("Signal Strength: ");
  Serial.print(dev.getSignalStrength());
  Serial.print("%\nBattery Level: ");
  Serial.print(dev.getBattery());
  Serial.println("%");

  Serial.print("Device Name: ");
  Serial.println(dev.getDeviceName());
  Serial.print("Model Number: ");
  Serial.println(dev.getModelNumber());
  Serial.print("Firmware Revision: ");
  Serial.println(dev.getFirmwareRevision());
  Serial.print("Firmware Revision: ");
  Serial.println(dev.getFirmwareRevision2());
  Serial.print("Software Revision: ");
  Serial.println(dev.getSoftwareRevision());
  Serial.print("Manufacturerer: ");
  Serial.println(dev.getManufacturerName());
  
  Serial.print("Timestamp: ");
  Serial.println(dev.getDateTime());
  Serial.print("LCD Timeout: ");
  Serial.println(dev.getLCDTimeout().current);
  Serial.print("Status: ");
  Serial.println(dev.getStatus());
  
  Serial.print("Temperatures: ");
  Temperatures temp = dev.getTemperatures();
  Serial.print(temp.current_temp); Serial.print(", ");
  Serial.print(temp.manual_temp); Serial.print(", ");
  Serial.print(temp.target_temp_low); Serial.print(", ");
  Serial.print(temp.target_temp_high); Serial.print(", ");
  Serial.print(temp.offset_temp); Serial.print(", ");
  Serial.print(temp.window_open_detect); Serial.print(", ");
  Serial.println(temp.window_open_minutes);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  dev.disconnect();
}
```