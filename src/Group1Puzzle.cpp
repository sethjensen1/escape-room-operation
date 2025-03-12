#include <Arduino.h>
#include "escapeos.h"
#include "blinker.h"
#include "operation.h"
// Any additional library inclusions, variables, etc. can be put here. Ensure
// that variable names do not collide with variable names from escapeos.h and do
// not include libraries already included in escapeos.h
const byte mainLedPin = 23;
bool isDead =
    false; // This variable will check to see if the Gingerbread man has died
bool isComplete =
    false; // This variable will store if the puzzle has been completed.
uint8_t lifeStatus = 0;
bool DEBUG = false;
bool timeRemoved = false;
uint8_t numPiecesLeft = 0;

// Call publishPuzzleStatus once per loop in which a change is detected/made
void publishPuzzleStatus() {
  DynamicJsonDocument statusDoc(512);
  // TODO: (Assignment) Replace this static status document with one that
  // captures the full current status of your puzzle
  statusDoc["solved"] = isComplete;
  statusDoc["dead"] = isDead;
  statusDoc["lifeInformation"] = lifeStatus;
  statusDoc["timestamp"] =
      timeClient.getEpochTime(); // Always include the unix timestamp
                                 // in the status message

  publishMQTT(statusTopic(), statusDoc,
              true); // In most cases, retain (param 3) should be false. Puzzle
                     // status is the only case where it should be true
}
// Blinker onboardLedBlinker(onboardLedPin, 10, 200, 1000, ONBOARD_LED_ON);
// Blinker mainLedBlinker(mainLedPin, 10, 200, 1000, LED_ON);

void puzzleMQTTCallback(String inTopic, DynamicJsonDocument messageDoc) {}

void puzzleCommandCallback(String inTopic, DynamicJsonDocument commandDoc) {
  // Insert any logic that your puzzle should handle coming on its command topic
  // or serially (e.g., solve puzzle immediately if message === "solved")
  // // EXAMPLE CODE
  // String message = commandDoc["message"];
  // serialln(message);
}

Operation gameplay; // call and make the constructor of type operation

void setup() {
  baseEscapeOSSetup(puzzleMQTTCallback, puzzleCommandCallback); // DO NOT REMOVE
  // We may or may not need an init call here
  // Here is the initial set up for operation
  serialln("Operation successfully Initialized");

  //
  // EXAMPLE for initial verification after upload. This will cause the board to
  // send the achieveCheckpoint message once after connecting to WiFi
  // serialln("Sending achieve checkpoint message once at the end of setup");
  // achieveCheckpoint();
  // onboardLedBlinker.start();
  // mainLedBlinker.start();
  // serialln("Setup complete. Will blink indefinitely");
}

void loop() {
  baseEscapeOSLoop(); // DO NOT REMOVE
  gameplay.tick();    // This calls our game play tick function
  lifeStatus = gameplay.getLives();
  isDead = gameplay.getIsDead();
  isComplete = gameplay.getPuzzleComplete();
  numPiecesLeft = gameplay.getPiecesRemaining();
  if (DEBUG) {
    serialln("lifeStatus: " + String(lifeStatus));
  }
  publishPuzzleStatus();
  // This will complete the puzzle if they remove all the pieces
  if (isDead && !timeRemoved) {
    if (DEBUG) {
      serialln("The Gingerbread Man is Dead");
    }
    // subtract from the time
    timeRemoved = true;
  }

  if (isComplete) {
    achieveCheckpoint();
    if (DEBUG) {
      serialln("Puzzle complete");
    }
  }
  // Calls the tick function that will handle game play

  // Put your main puzzle code here. This will run indefinitely after setup
  // has completed Please follow best practices. Your loop should be able to
  // be read very clearly, almost as if it were a recipe or a story.
  // Abstract any complexity/implementation details into descriptively named
  // functions Debounce any input that should be debounced (such as
  // buttons/reed switch input). This prevents the signal from being sent
  // multiple times for one button press For any RFID puzzles, do not use
  // the UUID pre-loaded onto RFID tags. Write a string to the RFID tag's
  // first memory block and have your puzzle read these blocks instead of
  // UUID to determine behavior

  // // EXAMPLE CODE
  // printPeriodicallyLn("Blinking indefinitely...");
  // onboardLedBlinker.update();
  // mainLedBlinker.update();

  delay(10); // Do not remove this delay
}

// Any helper functions for your puzzle can be put here. Arduino will hoist
// functions in .ino files up to where they are needed unlike standard .cpp
// files