#include "SplitFlapModule.h"

const char SplitFlapModule::StandardChars[37] = {' ', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
                                                 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
                                                 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

const char SplitFlapModule::ExtendedChars[48] = {
    ' ', 'A', 'B', 'C', 'D', 'E',  'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U',  'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9', '\'', ':', '?', '!', '.', '-', '/', '$', '@', '#', '%',
};

// Default Constructor
SplitFlapModule::SplitFlapModule()
    : position(0), stepNumber(0), stepsPerRot(0), magnetPosition(0), numChars(0), charSetSize(0) {}

// Parameterized Constructor
SplitFlapModule::SplitFlapModule(int stepsPerFullRotation, int stepOffset, int magnetPos, int charSetSize)
    : stepsPerRot(stepsPerFullRotation), charSetSize(charSetSize) {
    
    magnetPosition = (magnetPos + stepOffset) % stepsPerRot;
    if (magnetPosition < 0) magnetPosition += stepsPerRot;
    position = magnetPosition;

    if (charSetSize == 37) {
        chars = StandardChars;
        numChars = 37;
    } else {
        chars = ExtendedChars;
        numChars = 48;
    }
}

void SplitFlapModule::init() {
    for (int i = 0; i < numChars; i++) {
        charPositions[i] = (i * stepsPerRot) / numChars;
    }
    stop();
}

int SplitFlapModule::getCharPosition(char inputChar) {
    for (int i = 0; i < numChars; i++) {
        if (chars[i] == inputChar) return charPositions[i];
    }
    return charPositions[0]; // default to space
}

void SplitFlapModule::stop() {
    currentMotorState = 0x00; // All outputs LOW
}

void SplitFlapModule::start() {
    stepNumber = (stepNumber + 3) % 4; // effectively take one off stepNumber
    step(false);                       
}

void SplitFlapModule::step(bool updatePosition) {
    // Stepping mapping for QA=IN4, QB=IN3, QC=IN2, QD=IN1
    switch (stepNumber) {
        case 0: currentMotorState = 0x0C; break; // 0b00001100
        case 1: currentMotorState = 0x06; break; // 0b00000110
        case 2: currentMotorState = 0x03; break; // 0b00000011
        case 3: currentMotorState = 0x09; break; // 0b00001001
    }
    
    if (updatePosition) {
        position = (position + 1) % stepsPerRot;
        stepNumber = (stepNumber + 1) % 4;
    }
}

bool SplitFlapModule::isMagnetDetected(uint8_t inputByte) {
    // Check bit 0 (D0 on the 74HC165). Assuming active LOW.
    return (inputByte & 0x01) == 0; 
}