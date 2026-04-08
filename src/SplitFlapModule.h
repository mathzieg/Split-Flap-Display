#pragma once

#include <Arduino.h>

class SplitFlapModule {
  public:
    SplitFlapModule(); 
    SplitFlapModule(int stepsPerFullRotation, int stepOffset, int magnetPos, int charSetSize);

    void init();

    void step(bool updatePosition = true);                   
    void stop();                                             
    void start();                                            

    int getMagnetPosition() const { return magnetPosition; } 
    int getCharPosition(char inputChar);                     
    int getPosition() const { return position; }             
    int getCharsetSize() const { return numChars; }          

    // NEW: Get the current 8-bit state for the shift register
    uint8_t getMotorState() const { return currentMotorState; }
    
    // NEW: Pass the byte received from SPI to check the D0 bit
    bool isMagnetDetected(uint8_t inputByte); 

    void magnetDetected() {
        position = magnetPosition;
    } 

  private:
    int position;                   
    int stepNumber;                 
    int stepsPerRot;                
    
    uint8_t currentMotorState = 0;  // Holds the byte to send to 74HC595

    int magnetPosition;             

    const char *chars;              
    int charPositions[48];          
    int numChars;                   
    int charSetSize;

    static const char StandardChars[37];
    static const char ExtendedChars[48];
};