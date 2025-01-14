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
const byte SLAVE_ADDRESS[5] = {'R', 'X', 'A', 'A', 'A'};

Adafruit_MPU6050 mpu;

struct SensorData {
   float gyroX;
   float gyroY;
   float gyroZ;
   float accelX;
   float accelY;
   float accelZ;
};
SensorData data;

void setup() {
   Serial.begin(9600);
   while(!Serial){}

   if(!radio.begin()) {
      Serial.println("======= NRF24L01 unconnected =======");
      Serial.println("[ERROR] NRF24L01 is not detected. Please check the wiring!");
      while(1);
   }
   radio.begin();
   Serial.println("======= NRF24L01 connected =======");
   Serial.println("[SUCCESS] NRF24L01 detected!");
   radio.openWritingPipe(SLAVE_ADDRESS);
   radio.setPALevel(RF24_PA_LOW);
   
   Serial.println("Checking SPI device...");
   byte response = SPI.transfer(0xFF);
   Serial.print("Response SPI: ");
   Serial.println(response, HEX);

   char text[] =  "PING";
   radio.write(&text, sizeof(text));
   bool isSuccess = radio.write(&text, sizeof(text));
   
   if(isSuccess) {
      Serial.println("PING sent!");
      delay(200);

      if(radio.isAckPayloadAvailable()) {
         char response[32] = {0}; // buffer
         radio.read(&response, sizeof(response));
         
         Serial.print("Response received: ");
         Serial.println(response);
      } else {
         Serial.println("No response from receiver...");
      }
   } else {
      Serial.println("Failed to send PING...");
   }

   if (!mpu.begin()) {
      Serial.println("======= MPU6050 unconnected =======");
      Serial.println("[ERROR] MPU6050 is not detected. Please check the wiring!");
      while (1);
   }
   mpu.begin();
   Serial.println("======= MPU6050 connected =======");
   Serial.println("[SUCCESS] MPU6050 detected and initialized.");

   mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
   mpu.setGyroRange(MPU6050_RANGE_500_DEG);
   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
   sensors_event_t a, g, temp;
   mpu.getEvent(&a, &g, &temp);

   data.gyroX = g.gyro.x;
   data.gyroY = g.gyro.y;
   data.gyroZ = g.gyro.z;
   data.accelX = a.acceleration.x;
   data.accelY = a.acceleration.y;
   data.accelZ = a.acceleration.z;

   bool success = radio.write(&data, sizeof(data));

   if(!success) {
      Serial.println("Data failed to sent! ");
   } else {
      Serial.println("Data sent: ");
      // Tampilkan data akselerometer
      Serial.println("===== Acceleration ====="); 
      Serial.print("X: "); Serial.print(a.acceleration.x); Serial.print(", "); 
      Serial.print("Y: "); Serial.print(a.acceleration.y); Serial.print(", "); 
      Serial.print("Z: "); Serial.println(a.acceleration.z); 

      // Tampilkan data gyroscope
      Serial.println("===== Gyro =====");
      Serial.print("X: "); Serial.print(g.gyro.x); Serial.print(", "); 
      Serial.print("Y: "); Serial.print(g.gyro.y); Serial.print(", "); 
      Serial.print("Z: "); Serial.println(g.gyro.z);

      // Tampilkan suhu
      Serial.println("===== Temperature ====="); 
      Serial.print("Temp: "); Serial.print(temp.temperature); Serial.println(" °C");

      Serial.println("----------------------------------------");
      delay(500); // Tunggu sebentar sebelum membaca lagi
   }

   delay(2000);
}