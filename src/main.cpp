#include <Arduino.h>
#include <Wire.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

#define ENA 10
#define IN1 7
#define IN2 8

#define ENB 11
#define IN3 4
#define IN4 5

// NRF24L01
#define CE_PIN 9
#define CSN_PIN 10

// Initiate Radio
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
   Serial.begin(9600);
  

   radio.begin();
   
   if(!radio.begin()) {
      Serial.println("======= NRF24L01 is unconnected =======");
      Serial.println("[ERROR] NRF24L01 not detected. Please check the wiring!");
      while(1);
   } else {
      Serial.println("======= NRF24L01 is connected =======");
      Serial.println("[SUCCESS] NRF24L01 detected and initialized.");
      radio.openReadingPipe(0, address);
      radio.setPALevel(RF24_PA_LOW);
      
      Serial.println("Checking SPI device...");
      byte response = SPI.transfer(0xFF);
      Serial.print("Response SPI: ");
      Serial.println(response, HEX);
      
      radio.startListening();
   } 

   pinMode(ENA, OUTPUT);
   pinMode(IN1, OUTPUT);
   pinMode(IN2, OUTPUT);

   pinMode(ENB, OUTPUT);
   pinMode(IN3, OUTPUT);
   pinMode(IN4, OUTPUT);
}

void maju() {
   digitalWrite(IN1, HIGH);
   digitalWrite(IN2, LOW);
   analogWrite(ENA, 150);


   digitalWrite(IN3, HIGH);
   digitalWrite(IN4, LOW);
   analogWrite(ENB, 150);
}


void mundur() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 150);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 150);
}

void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}

void putarKanan() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 150);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 150);
}

void putarKiri() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 150);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 150);
}


void loop() {
   if(radio.available()) {
      float data[3];
      radio.read(&data, sizeof(data));

      Serial.print("Data diterima: ");
      Serial.print(data[0]); Serial.print(", ");
      Serial.print(data[1]); Serial.print(", ");
      Serial.println(data[2]);
      Serial.println("----------------------------------------");

      if(data[1] > 1.0) {
         maju();
      } else if(data[1] < -1.0) {
         mundur();
      } else if(data[0] > 1.0) {
         putarKanan();
      } else if(data[0] < -1.0) {
         putarKiri();
      } else {
         stop();
      };
   };
}



