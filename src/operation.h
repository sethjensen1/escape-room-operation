#ifndef OPERATION_H
#define OPERATION_H

typedef enum { INIT, NOT_TOUCHED, TOUCHED, COMPLETE } operation_state_t;

const bool BUZZER_ON = HIGH;
const bool BOWTIE_ON = LOW; // see if this is what turns the led on
const bool OUR_LED_ON = HIGH;

const bool BUZZER_OFF = !BUZZER_ON;
const bool BOWTIE_OFF = !BOWTIE_ON;
const bool OUR_LED_OFF = !OUR_LED_ON;

const bool TWEEZERS_TOUCHED = HIGH;
const bool TWEEZERS_NOT_TOUCHED = !TWEEZERS_TOUCHED;

// number of photodiodes tracking whether a piece is in
const int numPieces = 7;

const int photodiodes[numPieces] = {36, 39, 34, 35, 32, 33, 37}; // Input pins
const int threshold_defaults[numPieces] = {900,  900,  3800, 3900,
                                           3900, 3400, 3400}; // const thresholds for the photodiodes
const int smileleds[numPieces] = {4, 16, 17, 5, 18}; // Output pins for LEDS

const int bowtie = 19;
const int buzzer = 21;
const int tweezers = 2;
const int flashTime = 10;
const int lives = 5;
const float flashWaitTime = 0.5;

class Operation {
private:
  uint8_t m_lives;
  bool m_dead;
  bool m_puzzleComplete;

  int m_thresholds[numPieces]; // Thresholds for input pins

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