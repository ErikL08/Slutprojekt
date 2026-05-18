#include "U8glib.h"
#include <Servo.h>

Servo myservo;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

// defines pins numbers
const int trigPin1 = 3;
const int echoPin1 = 2;
const int trigPin2 = 5;
const int echoPin2 = 4;
// defines variables
long duration;
int distance;

void setup() {
  myservo.attach(9);
  pinMode(trigPin1, OUTPUT);  // Sets the trigPin1 as an Output
  pinMode(echoPin1, INPUT);   // Sets the echoPin1 as an Input
  pinMode(trigPin2, OUTPUT);  // Sets the trigPin2 as an Output
  pinMode(echoPin2, INPUT);   // Sets the echoPin2 as an Input
  Serial.begin(9600);         // Starts the serial communication
}
void loop() {
  int distance = getDistance();
  int height = getHeight();
  int full = getFull(height);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print("   Height: ");
  Serial.println(height);
  servoWrite(distance);
  oledWrite(full);
  delay(100);
}

int getDistance() {
  // Clears the trigPin1
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin1 on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  // Reads the echoPin1, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin1, HIGH, 30000);  // 30ms timeout (~5m range)
  if (duration == 0) return 999;              // No reading
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor

  return distance;
}

int getHeight() {
  // Clears the trigPin1
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin1 on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin1, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin2, HIGH, 30000);  // 30ms timeout (~5m range)
  if (duration == 0) return 999;              // No reading
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor

  return distance;
}

void oledWrite(int full) {
  u8g.setFont(u8g_font_helvB10);
  u8g.firstPage();
  do {
    u8g.drawStr(0, 20, ("Full: " + String(full) + "%").c_str());
  } while (u8g.nextPage());
}

int getFull(int height) {
  float full = height / 1.5;
  int procent = 100 - full * 10;
  if (height > 15) {  // If the second sensor is over the height of the trashcan return 0
    return 0;
  }
  return procent;
}

void servoWrite(int distance) {
  if (distance < 10) {
    myservo.write(90);
    delay(5000);
    myservo.write(0);
  }
}