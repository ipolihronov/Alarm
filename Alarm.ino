/*****************INCLUDES******************/
#include <virtuabotixRTC.h>
#include <Keypad.h>
#include <string.h>

/*****************DEFINES******************/
#define PASS_LEN 6
#define TRUE 1
#define FALSE 0

int melody[] = {
  500, 1000, 499, 600, 900, 0, 300, 700};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

/*****************VARIABLES******************/
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char pass[PASS_LEN] = {'9', '7', '8', '2', '6', '*'};
char passUSR[PASS_LEN];
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//Wiring SCLK -> 6, I/O -> 7, CE -> 8
//Or CLK -> 6 , DAT -> 7, Reset -> 8

virtuabotixRTC myRTC(11, 12, 13); //If you change the wiring change the pins here also

uint8_t passCompare(char *Pass, char *Input);
int thisChar = 'a';
int Success = FALSE;

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
//   myRTC.setDS1302Time(00, 3, 15, 3, 6, 6, 2018);
}

void loop() {
  checkAlarm(17 ,0);
}

uint8_t passCompare(char *Pass, char *Input){
  uint8_t result = TRUE;
  Success = TRUE;
  for( int i = 0; i < PASS_LEN; i++){
    if( *(Pass + PASS_LEN - i - 1) != *(Input + i)){
      result = FALSE;
      Success = FALSE;
    }
  }
  return result;
}

void checkAlarm(uint8_t Hours, uint8_t Minutes){
  myRTC.updateTime();
  if( Hours == myRTC.hours && Minutes == myRTC.minutes)
  {
      Alarm();
  }
}

void Alarm(void){
  while(puzzleNotSolved()){
   puzzleNotSolved();
   Melody();
  }   
}

boolean puzzleNotSolved(){
  boolean Result = TRUE;
  char Key = keypad.getKey();
    if (Key) {
    for (int i = PASS_LEN - 1; i > 0; i--) {
      passUSR[i] = passUSR[i - 1];
    }
    passUSR[0] = Key;
    Serial.println(passUSR);
  }
  if (( 1 == passCompare(pass, passUSR)) && (TRUE == Success)) {
    digitalWrite(LED_BUILTIN, HIGH);
    Success = FALSE;
    Result = FALSE;
  }
  else{
   }
   return Result;
}

void Melody(){
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(10, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(10);
    }
}

