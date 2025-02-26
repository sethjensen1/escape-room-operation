#ifndef OPERATION_H
#define OPERATION_H

typedef enum { INIT, NOT_TOUCHED, TOUCHED, COMPLETE } operation_state_t;

const bool BUZZER_ON = HIGH;
const bool BOWTIE_ON = LOW;

const bool BUZZER_OFF = !BUZZER_ON;
const bool BOWTIE_OFF = !BOWTIE_ON;

const bool TWEEZERS_TOUCHED = HIGH;
const bool TWEEZERS_NOT_TOUCHED = !TWEEZERS_TOUCHED;

// number of photodiodes tracking whether a piece is in
const int numPieces = 7;

const int photodiodes[numPieces] = {36, 39, 34, 35, 32, 33, 25}; // Input pins
const int threshold_defaults[numPieces] = {900,  900,  3800, 3900,
                                           3900, 3400, 3400}; // Input pins
const int bowtie = 23;
const int buzzer = 21;
const int tweezers = 2;

class Operation {
private:
  uint8_t m_lives;
  bool m_dead;
  bool m_puzzleComplete;

  int m_thresholds[numPieces]; // Thresholds for input pins

  uint8_t m_piecesRemaining;

  operation_state_t m_currentState;

  void updateSmileLeds();

public:
  Operation();

  void tick();

  uint8_t getLives();
  bool getIsDead();
  bool getPuzzleComplete();
  uint8_t getPiecesRemaining();
};

#endif