#include <Arduino.h>
#include "operation.h"

Operation::Operation() {
  Serial.println("Operation is being initialized in its constructor! ");

  // inputs
  for (int i = 0; i < NUM_PIECES; i++) {
    pinMode(lightSensors[i], INPUT);
  }
  pinMode(tweezers, INPUT_PULLUP);

  // outputs
  pinMode(buzzer, OUTPUT);
  pinMode(bowtie, OUTPUT);
  for (int i = 0; i < NUM_SMILE_LEDS; i++) {
    pinMode(smileLeds[i], OUTPUT);
  }

  // initialize outputs
  for (int i = 0; i < NUM_SMILE_LEDS; i++) {
    digitalWrite(smileLeds[i], OUR_LED_ON);
  }
  // updateSmileLeds();
  digitalWrite(bowtie, BOWTIE_OFF);
  digitalWrite(buzzer, BUZZER_OFF);

  m_lives = STARTING_LIVES;
  m_puzzleComplete = false;
  m_currentState = INIT;
  m_piecesRemaining = NUM_PIECES;

  // delay(100);
}

void Operation::tick() {
  bool edgeTouched = (digitalRead(tweezers) == TWEEZERS_TOUCHED);
  Serial.print("is the edge touched: ");
  Serial.println(edgeTouched);

  // Check all inputs, if any light values are below the set threshold, then
  // all pieces are *not* gone
  bool allPiecesGone = true;
  for (int i = 0; i < NUM_PIECES; i++) {
    if (analogRead(lightSensors[i]) < LIGHT_THRESHOLD) {
      allPiecesGone = false;
      break;
    }
  }

  // Transition
  switch (m_currentState) {
  case INIT:
    m_currentState = NOT_TOUCHED;
    m_lives = STARTING_LIVES;
    m_dead = false;
    m_puzzleComplete = false;
    // mealy actions (I think)
    break;
  case NOT_TOUCHED:
    // check pieces
    if (allPiecesGone) {
      // display something with the LEDs?
      m_currentState = GAME_OVER;
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
  case GAME_OVER:
    break;
  }
}

void Operation::updateSmileLeds() {
  if (m_dead) {
    for (int i = 0; i < NUM_SMILE_LEDS; i++) {
      digitalWrite(smileLeds[i], OUR_LED_OFF);
    }
  } else {
    for (int i = 0; i < NUM_SMILE_LEDS; i++) {
      if (i <= m_lives) {
        digitalWrite(smileLeds[i], OUR_LED_ON);
      } else {
        digitalWrite(smileLeds[i], OUR_LED_OFF);
      }
    }
  }
}

void Operation::blinkDeadLeds() {
  for (int i = 0; i < FLASH_TIME; i++) {
    digitalWrite(buzzer, BUZZER_ON);
    digitalWrite(bowtie, BOWTIE_ON);

    for (int j = 0; j < NUM_SMILE_LEDS; j++) {
      digitalWrite(smileLeds[j], OUR_LED_ON);
    }

    delay(FLASH_WAIT_TIME);

    digitalWrite(buzzer, BUZZER_OFF);
    digitalWrite(bowtie, BOWTIE_OFF);
    for (int j = 0; j < NUM_SMILE_LEDS; j++) {
      digitalWrite(smileLeds[j], OUR_LED_OFF);
    }

    delay(FLASH_WAIT_TIME);

    digitalWrite(buzzer, BUZZER_ON);
    digitalWrite(bowtie, BOWTIE_ON);

    for (int j = 0; j < NUM_SMILE_LEDS; j++) {
      digitalWrite(smileLeds[j], OUR_LED_ON);
    }

    delay(FLASH_WAIT_TIME);

    digitalWrite(buzzer, BUZZER_OFF);
    digitalWrite(bowtie, BOWTIE_OFF);

    for (int j = 0; j < NUM_SMILE_LEDS; j++) {
      digitalWrite(smileLeds[j], OUR_LED_OFF);
    }
  }
}

uint8_t Operation::getLives() { return m_lives; }

bool Operation::getIsDead() { return m_dead; }

bool Operation::getPuzzleComplete() { return m_puzzleComplete; }

uint8_t Operation::getPiecesRemaining() { return m_piecesRemaining; }