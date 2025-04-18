#ifndef OPERATION_H
#define OPERATION_H

typedef enum { INIT, NOT_TOUCHED, TOUCHED, GAME_OVER } operation_state_t;

const bool BUZZER_ON = HIGH;
const bool OUR_LED_ON = LOW;
const bool BOWTIE_ON = LOW; // see if this is what turns the led on

const bool BUZZER_OFF = !BUZZER_ON;
const bool BOWTIE_OFF = !BOWTIE_ON;
const bool OUR_LED_OFF = !OUR_LED_ON;

const bool TWEEZERS_TOUCHED = LOW;
const bool TWEEZERS_NOT_TOUCHED = !TWEEZERS_TOUCHED;

const bool PIECE_GONE = HIGH;
const bool PIECE_PRESENT = !PIECE_GONE;
const int LAST_PIECE_DELAY = 1000; // ms

const int NUM_SMILE_LEDS = 5;
const int NUM_PIECES = 6;

// Pin assignments
// Input pins
const int hallEffectSensors[NUM_PIECES] = {36, 39, 34, 35, 32, 33};
const int tweezers = 22;

// Output pins

const int buzzer = 2;

const int eyes = 19;
const int bowtie = 15;
// smile LED rows, in order from bottom to top
const int smileLeds[NUM_SMILE_LEDS] = {18, 5, 17, 16, 4};

const int FLASH_TIME = 10;
const int STARTING_LIVES = 4;
const float FLASH_WAIT_TIME = 100; // in ms

class Operation {
private:
  uint8_t m_lives;
  bool m_dead;
  bool m_puzzleComplete;
  int m_gameWonTimer;

  uint8_t m_piecesRemaining;

  operation_state_t m_currentState;

  void updateSmileLeds();
  void blinkDeadLeds();

public:
  Operation();

  void tick();

  uint8_t getLives();
  bool getIsDead();
  bool getPuzzleComplete();
  uint8_t getPiecesRemaining();
};

#endif
