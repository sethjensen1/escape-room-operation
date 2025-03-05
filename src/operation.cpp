#include <Arduino.h>
#include "operation.h"

Operation::Operation() {
  Serial.println("Operation is being initialized in its constructor! ");

  // inputs
  // for (int i = 0; i < numPieces; i++) {
  // pinMode(photodiodes[i], INPUT);
  // }
  pinMode(tweezers, INPUT_PULLUP);

  // outputs
  pinMode(buzzer, OUTPUT);
  pinMode(bowtie, OUTPUT);
  for (int i = 0; i < numSmileLeds; i++) {
    pinMode(smileLeds[i], OUTPUT);
  }

  // initialize analog inputs
  // for (uint8_t i = 0; i < numPieces; i++) {
  //   m_thresholds[i] = analogRead(photodiodes[i]);
  //   // m_thresholds[i] = threshold_defaults[i];
  // }

  // initialize outputs
  for (int i = 0; i < numSmileLeds; i++) {
    digitalWrite(smileLeds[i], OUR_LED_ON);
  }
  // updateSmileLeds();
  digitalWrite(bowtie, BOWTIE_OFF);
  digitalWrite(buzzer, BUZZER_OFF);

  m_lives = startingLives;
  m_puzzleComplete = false;
  m_currentState = INIT;
  m_piecesRemaining = numPieces;

  // delay(100);
}

void Operation::tick() {
  bool edgeTouched = (digitalRead(tweezers) == TWEEZERS_TOUCHED);
  Serial.print("is the edge touched: ");
  Serial.println(edgeTouched);

  bool allPiecesGone = false;
  // bool allPiecesGone = true;

  // Check all inputs, if any light values are below the set m_thresholds, then
  // all pieces are *not* gone
  // for (int i = 0; i < numPieces; i++) {
  //   if (analogRead(photodiodes[i]) < m_thresholds[i]) {
  //     allPiecesGone = false;
  //     break;
  //   }
  // }

  // Transition
  switch (m_currentState) {
  case INIT:
    m_currentState = NOT_TOUCHED;
    m_lives = startingLives;
    m_dead = false;
    m_puzzleComplete = false;
    // mealy actions (I think)
    break;
  case NOT_TOUCHED:
    // check pieces
    if (allPiecesGone) {
      // display something with the LEDs?
      m_currentState = COMPLETE;
      m_puzzleComplete = true;
    } else if (edgeTouched) {
      m_currentState = TOUCHED;
      digitalWrite(buzzer, BUZZER_ON);
      digitalWrite(bowtie, BOWTIE_ON);
      if (m_lives > 0) {
        m_lives--;
        updateSmileLeds();
      } else if (!m_dead) {
        m_dead = true;
        updateSmileLeds();
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
  if (m_dead) {
    for (int i = 0; i < numSmileLeds; i++) {
      digitalWrite(smileLeds[i], OUR_LED_OFF);
    }
  } else {
    for (int i = 0; i < numSmileLeds; i++) {
      if (i <= m_lives) {
        digitalWrite(smileLeds[i], OUR_LED_ON);
      } else {
        digitalWrite(smileLeds[i], OUR_LED_OFF);
      }
    }
  }
}

void Operation::blinkDeadLeds() {
  for (int i = 0; i < flashTime; i++) {
    digitalWrite(buzzer, BUZZER_ON);
    digitalWrite(bowtie, BOWTIE_ON);

    for (int j = 0; j < numSmileLeds; j++) {
      digitalWrite(smileLeds[j], OUR_LED_ON);
    }

    delay(flashWaitTime);

    digitalWrite(buzzer, BUZZER_OFF);
    digitalWrite(bowtie, BOWTIE_OFF);
    for (int j = 0; j < numSmileLeds; j++) {
      digitalWrite(smileLeds[j], OUR_LED_OFF);
    }

    delay(flashWaitTime);

    digitalWrite(buzzer, BUZZER_ON);
    digitalWrite(bowtie, BOWTIE_ON);

    for (int j = 0; j < numSmileLeds; j++) {
      digitalWrite(smileLeds[j], OUR_LED_ON);
    }

    delay(flashWaitTime);

    digitalWrite(buzzer, BUZZER_OFF);
    digitalWrite(bowtie, BOWTIE_OFF);

    for (int j = 0; j < numSmileLeds; j++) {
      digitalWrite(smileLeds[j], OUR_LED_OFF);
    }
  }
}

uint8_t Operation::getLives() { return m_lives; }

bool Operation::getIsDead() { return m_dead; }

bool Operation::getPuzzleComplete() { return m_puzzleComplete; }

uint8_t Operation::getPiecesRemaining() { return m_piecesRemaining; }