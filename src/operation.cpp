#include <Arduino.h>
#include "operation.h"

const int DEBUG = false;

Operation::Operation() {
  // inputs
  for (int i = 0; i < NUM_PIECES; i++) {
    pinMode(hallEffectSensors[i], INPUT);
  }
  pinMode(tweezers, INPUT_PULLUP);

  // outputs
  pinMode(eyes, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(bowtie, OUTPUT);
  for (int i = 0; i < NUM_SMILE_LEDS; i++) {
    pinMode(smileLeds[i], OUTPUT);
  }

  // initialize outputs
  digitalWrite(eyes, OUR_LED_OFF);
  for (int i = 0; i < NUM_SMILE_LEDS; i++) {
    digitalWrite(smileLeds[i], OUR_LED_ON);
  }
  // updateSmileLeds();
  digitalWrite(bowtie, BOWTIE_OFF);
  digitalWrite(buzzer, BUZZER_OFF);

  m_lives = STARTING_LIVES;
  m_puzzleComplete = false;
  m_gameWonTimer = 0;
  m_currentState = INIT;
  m_piecesRemaining = NUM_PIECES;

  // delay(100);
}

void Operation::tick() {
  bool edgeTouched = (digitalRead(tweezers) == TWEEZERS_TOUCHED);
  if (DEBUG) {
    Serial.print("is the edge touched: ");
    Serial.println(edgeTouched);
  }

  // Check all piece sensor inputs, if any piece is present, then all pieces are
  // not gone all pieces are *not* gone
  bool allPiecesGone = true;
  for (int i = 0; i < NUM_PIECES; i++) {
    if (digitalRead(hallEffectSensors[i] == PIECE_PRESENT)) {
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
      if (m_gameWonTimer == 0) {
        // start gameWonTimer
        m_gameWonTimer = millis();
      } else if ((millis() - m_gameWonTimer) > LAST_PIECE_DELAY) {
        m_puzzleComplete = true;
        // disable gameWonTimer
        m_gameWonTimer = 0;
      }
    } else {
      // disable gameWonTimer
      m_gameWonTimer = 0;
    }

    if (m_puzzleComplete == true) {
      // display something with the LEDs?
      m_currentState = GAME_OVER;
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
        digitalWrite(eyes, OUR_LED_ON);
      }
    }
    break;
  case TOUCHED:
    if (!edgeTouched) {
      delay(100);
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
    digitalWrite(eyes, OUR_LED_ON);

    for (int j = 0; j < NUM_SMILE_LEDS; j++) {
      digitalWrite(smileLeds[j], OUR_LED_ON);
    }

    delay(FLASH_WAIT_TIME);

    digitalWrite(buzzer, BUZZER_OFF);
    digitalWrite(bowtie, BOWTIE_OFF);
    digitalWrite(eyes, OUR_LED_OFF);
    for (int j = 0; j < NUM_SMILE_LEDS; j++) {
      digitalWrite(smileLeds[j], OUR_LED_OFF);
    }

    delay(FLASH_WAIT_TIME);

    digitalWrite(buzzer, BUZZER_ON);
    digitalWrite(bowtie, BOWTIE_ON);
    digitalWrite(eyes, OUR_LED_ON);

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