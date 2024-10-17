#include <Arduino.h>
#include <PS4Controller.h>
#include <ESP32Servo.h>

const int MOTOR1_DIR_PIN = 1; //ピン番号入れる
const int MOTOR1_PWM_PIN = 2; //MOTOR1が右、MOTOR2が左とする
const int MOTOR2_DIR_PIN = 3;
const int MOTOR2_PWM_PIN = 4;

Servo myservo;

void setup() {
 Serial.begin(115200);
 PS4.begin(""); //PS4のアドレス入れる

 ledcSetup(1,12800,8);
 ledcSetup(2,12800,8);

 ledcAttachPin(MOTOR1_DIR_PIN,1);
 ledcAttachPin(MOTOR2_DIR_PIN,2);

 pinMode(MOTOR1_DIR_PIN, OUTPUT);
 pinMode(MOTOR2_DIR_PIN, OUTPUT);

 myservo.attach(4);//4PINにサーボモーターをつなぐ

 Serial.println("Ready.");
}

void loop() {

   if (!PS4.isConnected()){
      ledcWrite(1,0);
      ledcWrite(2,0);

      digitalWrite(MOTOR1_DIR_PIN, 0);
      digitalWrite(MOTOR2_DIR_PIN, 0);

      return;
   }

   if (PS4.Square()){
      myservo.write(90);
      delay(1000);
      myservo.write(0);
      delay(1000);
   }

   if (PS4.R2()){
      ledcWrite(1,1);
      ledcWrite(2,1);

      digitalWrite(MOTOR1_DIR_PIN, 0);
      digitalWrite(MOTOR2_DIR_PIN, 0);

   }
   else if (PS4.L2()){
      ledcWrite(1,1);
      ledcWrite(2,1);

      digitalWrite(MOTOR1_DIR_PIN, 1);
      digitalWrite(MOTOR2_DIR_PIN, 1);
   }
   else if (abs(PS4.LStickY()) <= 20){
      int ly;
      bool dir1 = 0;
      bool dir2 = 0;

      ly = PS4.LStickY() * 1.8;

      dir1 = ly < 0 ? 0 : 1;
      dir2 = ly < 0 ? 1 : 0; //どっちかのHIGHをLOWに変える

      ledcWrite(1, ly);
      ledcWrite(2, ly);

      digitalWrite(MOTOR1_DIR_PIN, dir1);
      digitalWrite(MOTOR2_DIR_PIN, dir2);
   }
   else{
      ledcWrite(1,0);
      ledcWrite(2,0);
   }
}