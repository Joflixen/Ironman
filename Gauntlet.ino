/*
*/
// constants //
const int buttonPin = 13;          // PIN for switch to ground
const int tonePin = 3;            // PIN for Speaker to ground through 100ohm resistor
const int ledPin =  9;            // PIN for LED OUTPUT
const int ledBoard =  13;         // Onboard LED (indicates top of ramp)
const int delaySpeed = 4;         // Time between next step of the change       
const int sprayDuration = 2000;    // Intervals the spray-jet is on for (2sec ~= 400)
const int pwmIntervals = 300;     // Howmany times it changes brightness/sound frequency
const int toneFreqLow   = 3250;   // Tone Freqency at bottom of Ramp-up
const int toneFreqHigh  = 5500;   // Tone Frequency at top of Ramp-Up

// --------------------------------------------------------------------------------------//
// variables //
int buttonState = 0;            // variable for reading the pushbutton status
int fadeValue = 0;              // variable for storing the PWM duty
int pwmInt = 0;                 // variable for which interval we are at 
int toneValue = toneFreqLow;    // Preset the starting tone value to lower ramp frequency
int sprayDuty = sprayDuration;  // Remainder of Spray Intervals
float Rled;                     // Log curve calulation for LED
int rampDown = 0;

// --------------------------------------------------------------------------------------//
// Initialise stuff //
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(tonePin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Rled = (pwmIntervals * log10(2))/(log10(255));
}

// Run the Stuff //
void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    // FADE UP //
    if (pwmInt < pwmIntervals){
      pwmInt+=1.5;
      fadeValue = pow(2, (pwmInt / Rled)) -1;
      setTone(pwmInt);
      rampDown = 1;
    } else {
      // FIRE //
      rampDown = 0;
      noTone(tonePin);
      // Handle the Sprayer
      if (sprayDuty > 1){
        digitalWrite(ledBoard, HIGH);
        sprayDuty--;
      } else {
        // FIRE //
        digitalWrite(ledBoard, LOW);
      }
    }
  } else {
    // FADE OUT //
    digitalWrite(ledBoard, LOW);
    sprayDuty = sprayDuration;
    if (pwmInt > 0 ){
      pwmInt--;
      fadeValue = pow(2, (pwmInt / Rled)) -1;
      if (rampDown > 0){
          setTone(pwmInt);
      }
    } else {
      noTone(tonePin);
    }
  }
  analogWrite(ledPin, fadeValue);
  delay(delaySpeed);
}
// Helper function sets the tone frequency based on interval-step
void setTone(int interval){
  int freqRange = (toneFreqHigh-toneFreqLow);
  toneValue = toneFreqLow + ( (freqRange/pwmIntervals) * interval );
  tone(tonePin, toneValue);
}
