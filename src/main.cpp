#include <Arduino.h>
#include <wire.h>
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
#define CE_PIN 12
#define CSN_PIN 13

// Initiate Radio
RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  if(radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    
    Serial.println("Data Diterima: " + String(text));
  }
  
  
  // Serial.println("Maju");
  // maju();
  // delay(2000);

  // Serial.println("Mundur");
  // mundur();
  // delay(2000);

  // Serial.println("Putar Kanan");
  // putarKanan();
  // delay(2000);

  // Serial.println("Putar Kiri");
  // putarKiri();
  // delay(2000);
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
