#include <Arduino.h>
#include "operation.h"

Operation::Operation() {
  Serial.println("Operation is being initialized in its constructor! ");

  for (int i = 0; i < numPieces; i++) {
    pinMode(photodiodes[i], INPUT);
  }

  pinMode(bowtie, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(tweezers, INPUT);

  digitalWrite(bowtie, BOWTIE_OFF);
  digitalWrite(buzzer, BUZZER_OFF);

  m_lives = lives;
  m_puzzleComplete = false;
  m_currentState = INIT;
  m_piecesRemaining = numPieces;

  for (uint8_t i = 0; i < numPieces; i++) {
    m_thresholds[i] = analogRead(photodiodes[i]);
    // m_thresholds[i] = threshold_defaults[i];
  }

  // Done above initialize photodiode thresholds
  // // Find the m_thresholds value needed and update the array
  // // int sensorValue = analogRead(36);  // Read the analog voltage from the
  // // photodiode
  // //   Serial.print("Sensor value: ");
  // //   Serial.println(sensorValue);

  // delay(100);
}

void Operation::tick() {
  bool edgeTouched = (digitalRead(tweezers) == TWEEZERS_TOUCHED);
  Serial.print("is the edge touched: ");
  Serial.println(edgeTouched);
  // Sensor logic
  // bool allOff = true;
  bool allPiecesGone = true;

  // Check all inputs, if any light values are below the set m_thresholds, then
  // all pieces are *not* gone
  for (int i = 0; i < numPieces; i++) {
    if (analogRead(photodiodes[i]) < m_thresholds[i]) {
      allPiecesGone = false;
      break;
    }
  }

  // Transition
  switch (m_currentState) {
  case INIT:
    m_currentState = NOT_TOUCHED;
    m_lives = lives;
    m_dead = false;
    m_puzzleComplete = false;
    // mealy actions (I think)
    break;
  case NOT_TOUCHED:
    // check pieces
    if (allPiecesGone) {
      m_currentState = COMPLETE;
      m_puzzleComplete = true;
    } else if (edgeTouched) {
      m_currentState = TOUCHED;
      digitalWrite(buzzer, BUZZER_ON);
      digitalWrite(bowtie, BOWTIE_ON);
      if (m_lives > 0) {
        m_lives--;
        if(m_lives == 0){
          Serial.print("The lives are at: ");
          Serial.println(m_lives);
        }
        updateSmileLeds();
      } else if (!m_dead) {
        updateSmileLeds();
        m_dead = true;
        blinkDeadLeds();
      }
    }
    break;
  case TOUCHED:
    if (!edgeTouched) {
      m_currentState = NOT_TOUCHED;
      digitalWrite(buzzer, BUZZER_OFF);
      digitalWrite(bowtie, BOWTIE_OFF);
    }
    break;
  case COMPLETE:
    break;
  }
}

void Operation::updateSmileLeds() {
  Serial.print("Update the smile LEDs for lives: ");
  Serial.println(m_lives);
  digitalWrite(smileleds[m_lives], OUR_LED_OFF); // Turns off the led at the life position

}

void Operation:: blinkDeadLeds(){
  for (int i = 0; i < flashTime; i++){
    digitalWrite(buzzer, BUZZER_ON);
    delay(flashWaitTime);
    digitalWrite(bowtie, BOWTIE_ON);
    delay(flashWaitTime);
    digitalWrite(buzzer, BUZZER_OFF);
    for (int i = 0; i < numPieces; i++){
      digitalWrite(smileleds[i], OUR_LED_ON);
    }
    delay(flashWaitTime);
    digitalWrite(bowtie, BOWTIE_OFF);
    delay(flashWaitTime);
    for (int i = 0; i < numPieces; i++){
      digitalWrite(smileleds[i], OUR_LED_OFF);
    }
  }
}

uint8_t Operation::getLives() { return m_lives; }

bool Operation::getIsDead() { return m_dead; }

bool Operation::getPuzzleComplete() { return m_puzzleComplete; }

uint8_t Operation::getPiecesRemaining() { return m_piecesRemaining; }