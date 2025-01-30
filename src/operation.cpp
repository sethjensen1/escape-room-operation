#include <Arduino.h>

const int numInputs = 7; // Number of inputs

const int sensors[numInputs] = {36, 39, 34, 35, 32, 33, 25}; // Input pins
const int threshold[numInputs] = {900, 900, 3800, 3900, 3900, 3400, 3400};
// const int led = 23;
const int speaker = 21;
const int wire = 2;
bool puzzle_complete;

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < numInputs; i++) {
    pinMode(sensors[i], INPUT);
  }
  // pinMode(led, OUTPUT);
  pinMode(speaker, OUTPUT);
  pinMode(wire, INPUT);

  // digitalWrite(led, LOW);
  digitalWrite(speaker, LOW);

  puzzle_complete = false;
}

void loop() {
  if (digitalRead(wire) == HIGH) {
    // digitalWrite(led, HIGH);
    digitalWrite(speaker, HIGH);
    Serial.println("buzz II");
    // can send an additional signal if a punishment aspect wants to be added
    // about the timer
  } else {
    // digitalWrite(led, LOW);
    digitalWrite(speaker, LOW);
  }

  // // Sensor logic
  bool allOff = true;

  // Check all inputs
  for (int i = 0; i < numInputs; i++) {
    if (analogRead(sensors[i]) < threshold[i]) {
      allOff = false;
      break;
    }
  }

  // If all inputs are not true, set output pin HIGH
  if (allOff) {
    // delay (30000);
    if (allOff) {
      puzzle_complete = true;
      // Send the signal to the main ESP
      // Serial.println("Complete");
    }
  } else {
    puzzle_complete = false;
    // Serial.println("Not done");
  }

  // Find the threshold value needed and update the array
  // int sensorValue = analogRead(36);  // Read the analog voltage from the
  // photodiode
  //   Serial.print("Sensor value: ");
  //   Serial.println(sensorValue);

  delay(100);
}
