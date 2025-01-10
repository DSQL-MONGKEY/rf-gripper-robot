#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";
Adafruit_MPU6050 mpu;

void setup() {
   Serial.begin(9600);

   radio.begin();
   radio.openWritingPipe(address);
   radio.setPALevel(RF24_PA_LOW);
   radio.stopListening();

   if(!mpu.begin()) {
      Serial.println("MPU6050 is not found!");
      while(1);
   }

   Serial.println("MPU6050 success to initialize!");
}

void loop() {
   sensors_event_t a, g, temp;
   mpu.getEvent(&a, &g, &temp);

   float data[3] = {
      a.acceleration.x,
      a.acceleration.y,
      a.acceleration.z
   };

   bool success = radio.write(&data, sizeof(data));

   if(success) {
      Serial.print("Data sent: ");
      Serial.print(data[0]); Serial.print(", ");
      Serial.print(data[1]); Serial.print(", ");
      Serial.println(data[2]);
   } else {
      Serial.print(data[0]); Serial.print(", ");
      Serial.print(data[1]); Serial.print(", ");
      Serial.println(data[2]);
      Serial.println("Data failed to sent! ");
   }

   delay(2000);
}