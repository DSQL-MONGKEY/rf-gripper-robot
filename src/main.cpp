#include <Arduino.h>
#include <Wire.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>
#include <Servo.h>

#define ENA 10
#define IN1 7
#define IN2 8

#define ENB 11
#define IN3 4
#define IN4 5

// NRF24L01
#define CE_PIN 9
#define CSN_PIN 10

Servo gripperServo;
#define SERVO_PIN 3
#define OPEN_ANGLE 90
#define CLOSE_ANGLE 0

bool isRobotMode = true;

// Initiate Radio
RF24 radio(CE_PIN, CSN_PIN);
const byte SLAVE_ADDRESS[5] = {'R', 'X', 'A', 'A', 'A'};


// struct DataPackage {
//    float gyroX;
//    float gyroY;
//    float gyroZ;
// }
// DataPackage data;

void setup() {
   Serial.begin(9600);

   radio.begin();
   
   if(!radio.begin()) {
      Serial.println("======= NRF24L01 is unconnected =======");
      Serial.println("[ERROR] NRF24L01 not detected. Please check the wiring!");
      while(1);
   }

   Serial.println("======= NRF24L01 is connected =======");
   Serial.println("[SUCCESS] NRF24L01 detected and initialized.");
   radio.openReadingPipe(0, SLAVE_ADDRESS);
   radio.setPALevel(RF24_PA_LOW);

   Serial.println("Checking SPI device...");
   byte response = SPI.transfer(0xFF);
   Serial.print("Response SPI: ");
   Serial.println(response, HEX);
   
   radio.startListening();
   radio.enableAckPayload();

   if(radio.available()) {
      char receivedText[32] = {0};
      radio.read(&receivedText, sizeof(receivedText));

      Serial.print("Message received: ");
      Serial.println(receivedText);

      if(strcmp(receivedText, "PING") == 0) {
         char response[] = "PONG";
         radio.writeAckPayload(0, &response, sizeof(response));
         Serial.println("Response sent: PONG");
      }
   };

   gripperServo.attach(SERVO_PIN);
   gripperServo.write(OPEN_ANGLE);
   Serial.println("Servo Initialized");

   pinMode(ENA, OUTPUT);
   pinMode(IN1, OUTPUT);
   pinMode(IN2, OUTPUT);

   pinMode(ENB, OUTPUT);
   pinMode(IN3, OUTPUT);
   pinMode(IN4, OUTPUT);
};

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
      delay(500);
   };
   delay(2000);
}



