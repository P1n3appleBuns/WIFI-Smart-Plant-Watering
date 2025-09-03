/*WIFI Smart Plant Watering*/

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "WIFI Smart Plant Watering"
#define BLYNK_AUTH_TOKEN ""

#include <WiFi.h>
#include <WifiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

char ssid[] = "";
char pass[] = "";

//Pin Assignments
#define humtempPin 32
#define moistDPin  19
#define moistAPin  34
#define pumpPin    33
#define DHTTYPE DHT11  // DHT 11 sensor

//Virtual Pins
#define VPIN_MOISTURE  V0
#define VPIN_HUMIDITY  V1
#define VPIN_TEMP      V2
#define VPIN_THIRST    V3
#define VPIN_PUMP      V4

DHT dht(humtempPin, DHTTYPE);

// Create LED widget on Virtual Pin V1
WidgetLED ledWidget(VPIN_MOISTURE);
BlynkTimer timer;

void setup() {

  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); //Connect to Blynk
  
  //Assign the Pins
  pinMode(humtempPin, INPUT);
  pinMode(moistDPin, INPUT);
  pinMode(moistAPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, HIGH);

  //Default off for Blynk LED
  ledWidget.off();

  //Starts the DHT sensor
  dht.begin();

}

void loop() {
  
  Blynk.run();
  readSensors();

}

void readSensors() {

  moistureD();
  moistureA();
  temperature();
  humidity();

}

//Function to monitor digital moisture and turn Blynk LED on or off
void moistureD(){

  int digitalMoist = digitalRead(moistDPin);
  if ( digitalMoist == HIGH ){

    ledWidget.off();

  }else{

    ledWidget.on();

  }

}

//Function to monitor analog moisture and display value
void moistureA(){

  int sensorValue = analogRead(moistAPin);  // Read the analog value from sensor
  int outputValue = map(sensorValue, 4095, 0, 0, 100); // map the 10-bit data to 8-bit data
  Blynk.virtualWrite(VPIN_THIRST, outputValue); //Sends the analog output value to Blynk

}

//Function to monitor temperature level and display value on Blynk
void temperature(){

  int temperature = dht.readTemperature(); //Celcius

  if (!isnan(temperature)) {

  Blynk.virtualWrite(VPIN_TEMP, 0);
  
  }

  Blynk.virtualWrite(VPIN_TEMP, temperature); //Sends the temperature value to Blynk
  delay(2000);
  
}

//Function to monitor humidity level and display value on Blynk
void humidity(){

  int humidity = dht.readHumidity();

  if (!isnan(humidity)) {

  Blynk.virtualWrite(VPIN_HUMIDITY, 0);
  
  }
  
  Blynk.virtualWrite(VPIN_HUMIDITY, humidity); //Sends the humidity value to Blynk
  delay(2000);

}

//Function to turn on a water pump to water the plant
BLYNK_WRITE(VPIN_PUMP){ 

    int value = param.asInt(); // 1 or 0
    Serial.println(value);
    if (value == 1) {

      digitalWrite(pumpPin, LOW);
      delay(2000);
      digitalWrite(pumpPin, HIGH);

    }

}