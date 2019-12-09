//dial pad
const int readyPin = 3;
const int pulsePin =  2;
int prevReadyState, prevPulseState;
int digit, lastPulseCount;
//reset
const int resetPin = 4;
int resetPinState, preResetPinState;
char currentPhoneNumber[12];
int currentDigitIndex = 0;
// motor
const int MOTOR_PIN = 10;
//const int MOTOR_PIN = 11;
const int RELAY_PIN = 9;
bool ringing = false, ringingState = true;
int ringingTime;

bool phoneDown = true;

const int RINGING_RESOLUTION = 20;
const int RINGING_ON = 1500;
const int RINGING_OFF = 2500;

void gotDigit(int num) {
  currentPhoneNumber[currentDigitIndex] = (num + '0');
  if (!phoneDown) {
    Serial.write(currentPhoneNumber[currentDigitIndex]); 
  }
  
  if (currentDigitIndex == 2) {
//    Serial.println(currentPhoneNumber);
  } else {
    currentDigitIndex++;
  }
}

void resetNumber() {
  memset(currentPhoneNumber, 0, sizeof(currentPhoneNumber));
  currentDigitIndex = 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(readyPin, INPUT);
  pinMode(pulsePin, INPUT);
  pinMode(resetPin, INPUT);
  digitalWrite(readyPin, HIGH);
  digitalWrite(pulsePin, HIGH);
//  Serial.println("hello");

  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);
  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  int readyState = digitalRead(readyPin);
  int pulseState = digitalRead(pulsePin);
  // read the state of the pushbutton value:
  if (readyState == HIGH) {
    // Phone wheel is not moving
    if (prevReadyState == LOW) {
      // We just finished dialing a digit
//      Serial.println(lastPulseCount);
      gotDigit(lastPulseCount % 10);
      lastPulseCount = 0;
    }
  } else {
    // Phone wheel is moving
    if (pulseState == HIGH && pulseState != prevPulseState) {
      lastPulseCount++;
    }
  }

  prevReadyState = readyState;
  prevPulseState = pulseState;

  delay(5);


  //  reset function
  resetPinState = digitalRead(resetPin);
  if (preResetPinState != resetPinState) {
    if (resetPinState == HIGH) {
      resetNumber();
      phoneDown = true;
      Serial.write('r');
    } else {
      Serial.write('s');
      phoneDown = false;
    }
    delay(10);
  }
  preResetPinState = resetPinState;

  //motor
  if (ringing) {
    ringingTime += RINGING_RESOLUTION;
    if (ringingState) {
      digitalWrite(MOTOR_PIN, HIGH);
      if (ringingTime > RINGING_ON) {
        ringingState = false;
        ringingTime = 0;
      }
    } else {
      digitalWrite(MOTOR_PIN, LOW);
      if (ringingTime > RINGING_OFF) {
        ringingState = true;
        ringingTime = 0;
      }
    }
    delay(RINGING_RESOLUTION);
  }

  if (Serial.available()) {
    char bt = Serial.read();
    if (bt == 'i') {
      ringing = true;
      ringingTime = 0;
      ringingState = true;
      digitalWrite(MOTOR_PIN, HIGH);
      digitalWrite(RELAY_PIN, LOW);
    } else if (bt == 'o') {
      digitalWrite(MOTOR_PIN, LOW);
      ringing = false;
      ringingTime = 0;
    } else if (bt == 'p') {
      digitalWrite(RELAY_PIN, HIGH);
    } else if (bt == 'l') {
      digitalWrite(RELAY_PIN, LOW);
    }
  }
}
