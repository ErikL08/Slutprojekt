#include <Servo.h>

Servo myservo1;
Servo myservo2;


const int trigPin = 3;
const int echoPin = 2;
const int DETECTION_THRESHOLD = 10;

bool triggered = false;


void setup() {
  myservo1.attach(9);
  myservo2.attach(10);
  myservo2.write(0);  // Initialize to known position
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  servoSweep();
}

int getDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH, 30000);  // 30ms timeout (~5m range)
  if (duration == 0) return 999;                  // No reading
  // Calculating the distance
  int distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  return distance;
}

void servoSweep() {
  for (int pos = 0; pos <= 180; pos++) {
    myservo1.write(pos);
    delay(5);

    int distance = getDistance();

    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance < DETECTION_THRESHOLD && !triggered) {
      triggered = true;
      servo2();
    }

    if (distance >= DETECTION_THRESHOLD) {
      triggered = false;
    }
  }

  // Sweep backward
  for (int pos = 180; pos >= 0; pos--) {
    myservo1.write(pos);
    delay(5);

    int distance = getDistance();

    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance < DETECTION_THRESHOLD && !triggered) {
      triggered = true;
      servo2();
    }

    if (distance >= DETECTION_THRESHOLD) {
      triggered = false;
    }
  }
}

void servo2() {
  for (int pos2 = 0; pos2 <= 90; pos2 += 90) {
    myservo2.write(pos2);
    delay(20);
  }
  for (int pos2 = 90; pos2 >= 0; pos2 -= 1) {
    myservo2.write(pos2);
    delay(15);
  }
  delay(200);
}