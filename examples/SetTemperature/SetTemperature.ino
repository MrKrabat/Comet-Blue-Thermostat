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
  if (!dev.connect("Test", 0)) {
    Serial.println("FAILED");
    while (1) {
      delay(100);
    }
  }
  Serial.println("SUCCESS");
  
  // Set temperature for manual mode
  Temperatures temp = dev.getTemperatures();
  temp.manual_temp = 20.0; // target temperature in degree Celsius
  dev.setTemperatures(temp);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  dev.disconnect();
}
