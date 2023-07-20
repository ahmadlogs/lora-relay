#include <SoftwareSerial.h>
SoftwareSerial lora_serial(2, 3); // RX, TX

String FREQUENCY_BAND = "433000000";
String NETWORK_ID = "7";
String THIS_DEVICE = "1";
String OTHER_DEVICE = "2";

const int buttonPin = 6;
const int ledPin = 12;

bool relayState = false;

void setup() {
  Serial.begin(115200); 
  delay(1000);
  lora_serial.begin(115200); 
  delay(1000);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  lora_serial.println("AT+BAND="+ FREQUENCY_BAND);
  delay(1000);
  lora_serial.println("AT+NETWORKID="+ NETWORK_ID); 
  delay(1000);
  lora_serial.println("AT+ADDRESS="+ THIS_DEVICE); 
  delay(1000); 
}

void loop() {
  if (digitalRead(buttonPin) == LOW){ 
    relayState = !relayState;
    String command = (relayState == true) ? "a1" : "a0";
    lora_serial.println(
      "AT+SEND="+ OTHER_DEVICE+",2,"+ command);
    Serial.println(command);
    delay(500); //debounce handling
  }

  while (lora_serial.available()) { 
    String data = lora_serial.readString();
    Serial.println(data);
    if(data.indexOf("a1") > 0) { 
      digitalWrite(ledPin, HIGH);
      relayState = 1;
    } else if(data.indexOf("a0") > 0) { 
      digitalWrite(ledPin, HIGH);
      relayState = 0;
    }
  }

  delay(50);
  digitalWrite(ledPin, LOW);
}
