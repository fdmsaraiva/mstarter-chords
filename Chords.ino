/* @file Chords.ino
|| @version 7
|| @author Mark Stanley, Filipe Saraiva
|| @contact mstanley@technologist.com, fdmsaraiva@gmail.com
||
|| @description
|| | This code requires Mark Stanley's Keypad Library. It builds on
|| | his Multikey.ino example, which provided the base functionality
|| | of storing up to 10 simultaneous key presses.
|| | The added Chords playing functionality was also only possible
|| | with Mark's tireless help and insight, so many thanks good sir.
|| #
*/

#include <Keypad.h>

const byte ROWS = 5; //five rows
const byte COLS = 12; //twelve columns
char keys[ROWS][COLS] = {
    {'1','6','a','f','k','p','u','z','E','J','O','T'},
    {'2','7','b','g','l','q','v','A','F','K','P','U'},
    {'3','8','c','h','m','r','w','B','G','L','Q','V'},
    {'4','9','d','i','n','s','x','C','H','M','R','W'},
    {'5','0','e','j','o','t','y','D','I','N','S','X'}
};
byte rowPins[ROWS] = {42, 43, 44, 45, 46}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String notes[] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
String chords[] = {"Bass","M","m","M7m","dim"};
String msg;

unsigned long loopCount;
unsigned long startTime;
unsigned long faderDelay;
unsigned long tripleSwitchDelay;

int octaver;
int keyRow;
int keyCol;
int velFader;

void setup() {
    Serial.begin(115200);
    Serial1.begin(31250);
    loopCount = 0;
    startTime = millis();
    msg = "";
    // kpd.setHoldTime(500);
    // kpd.setDebounceTime(5);
    pinMode(22, INPUT_PULLUP);
    pinMode(23, INPUT_PULLUP);
}

void loop() {
    loopCount++;
    if ( (millis()-startTime)>5000 ) {
        Serial.print("Average loops per second = ");
        Serial.println(loopCount/5);
        startTime = millis();
        loopCount = 0;
    }

    // Is the current time, or millis(), more than 10 ms since the last time we were here?
    if ( (millis()-faderDelay)>10 )
    {
        velFader = analogRead(A0);
        velFader = map(velFader, 0, 1023, 0, 127);

        faderDelay = millis();   // Reset the timer only when we enter this block of code.
    }

    // Is the current time, or millis(), more than 10 ms since the last time we were here?
    if ( (millis()-tripleSwitchDelay)>10 )
    {
        int octavePosition1 = digitalRead(22);
        int octavePosition2 = digitalRead(23);

        if (octavePosition1 == LOW && octavePosition2 == HIGH) {
            octaver = -12;
        } else if (octavePosition1 == HIGH && octavePosition2 == LOW) {
            octaver = 12;
        } else if (octavePosition1 == HIGH && octavePosition2 == HIGH) {
            octaver = 0;
        }

        tripleSwitchDelay = millis();   // Reset the timer only when we enter this block of code.
    }

    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                keyRow = (kpd.key[i].kcode / COLS) ;
                keyCol = (kpd.key[i].kcode - (keyRow * COLS)) ;
                switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                    if (keyRow == 0) {
                        msg = " on!";
                        sendMidi(0x91, 48 + octaver + keyCol - 12, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol - 24, velFader);
                    }
                    else if (keyRow == 1) {
                        msg = " on!";
                        sendMidi(0x91, 48 + octaver + keyCol, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 4, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 7, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 12, velFader);
                    }
                    else if (keyRow == 2) {
                        msg = " on!";
                        sendMidi(0x91, 48 + octaver + keyCol, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 3, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 7, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 12, velFader);
                    }
                    else if (keyRow == 3) {
                        msg = " on!";
                        sendMidi(0x91, 48 + octaver + keyCol, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 4, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 7, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 10, velFader);
                    }
                    else if (keyRow == 4) {
                        msg = " on!";
                        sendMidi(0x91, 48 + octaver + keyCol, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 3, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 6, velFader);
                        sendMidi(0x91, 48 + octaver + keyCol + 9, velFader);
                    }
                    else {
                        msg = " PRESSED.";
                    }
                break;
                    case HOLD:
                    msg = " HOLD.";
                break;
                    case RELEASED:
                    if (keyRow == 0) {
                        msg = " off!";
                        sendMidi(0x81, 48 + octaver + keyCol - 12, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol - 24, velFader);
                    }
                    else if (keyRow == 1) {
                        msg = " off!";
                        sendMidi(0x81, 48 + octaver + keyCol, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 4, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 7, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 12, velFader);
                    }
                    else if (keyRow == 2) {
                        msg = " off!";
                        sendMidi(0x81, 48 + octaver + keyCol, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 3, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 7, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 12, velFader);
                    }
                    else if (keyRow == 3) {
                        msg = " off!";
                        sendMidi(0x81, 48 + octaver + keyCol, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 4, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 7, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 10, velFader);
                    }
                    else if (keyRow == 4) {
                        msg = " off!";
                        sendMidi(0x81, 48 + octaver + keyCol, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 3, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 6, velFader);
                        sendMidi(0x81, 48 + octaver + keyCol + 9, velFader);
                    }
                    else {
                        msg = " RELEASED.";
                    }
                break;
                    case IDLE:
                    msg = " IDLE.";
                }
                Serial.print("Key ");
                Serial.print(kpd.key[i].kcode);
                Serial.print(", Col ");
                Serial.print(keyCol);
                Serial.print(", Row ");
                Serial.print(keyRow);
                Serial.print(": ");
                Serial.print(notes[keyCol]);
                Serial.print(chords[keyRow]);
                Serial.println(msg);
            }
        }
    }
}  // End loop

//  plays a MIDI note.
void sendMidi(int cmd, int pitch, int velocity)
{
    Serial1.write(cmd);
    Serial1.write(pitch);
    Serial1.write(velocity);
}

