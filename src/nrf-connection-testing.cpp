#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

// Pin NRF24L01
RF24 radio(9, 10); // CE=9, CSN=10

void setup() {
   Serial.begin(9600);
   Serial.println("Memulai tes koneksi NRF24L01...");

   if (!radio.begin()) {
      Serial.println("NRF24L01 tidak terdeteksi!");
      while (1); // Berhenti di sini jika modul tidak terdeteksi
   }

   Serial.println("NRF24L01 terdeteksi!");
   radio.printDetails(); // Menampilkan detail konfigurasi modul
}

void loop() {}
