
int fanPwm = 3;             // Fan PWM pin 
int fanTach   = 2;          // Fan Tach pin 
int buttonUp = 7;           // Button speed up pin 
int buttonDown = 8;         // Button speed down pin 
int buttonStateUp = 0;      // Button speed up state
int buttonStateDown = 0;    // Button speed down state
int pwm = 0;                // 0-250 Pwm duty
int pwmStep = 25;           // 0-250 Step = 10% (25)
int led = 13;               // LED pin

unsigned long pulseDuration;
unsigned long previousMillis = 0;

const long interval = 1000; // 1 sec

void setup() {
  Serial.begin(9600);
  pinMode(fanPwm, OUTPUT);
  pinMode(fanTach, INPUT);
  digitalWrite(fanTach,HIGH);
  pinMode(led, OUTPUT);
  pinMode(buttonUp,INPUT_PULLUP);
  pinMode(buttonDown,INPUT_PULLUP);
}

void loop() {
  buttonStateUp = digitalRead(buttonUp);
  buttonStateDown = digitalRead(buttonDown);
  
  pulseDuration = pulseIn(fanTach, LOW);
  double frequency = 1000000/pulseDuration;
  int rpm = frequency/2*60;                 
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("pulse duration: ");
    Serial.println(pulseDuration);
    Serial.print("PWM Duty: ");
    Serial.println(pwm);
    Serial.print("RPM: ");
    Serial.println(rpm);
    Serial.println(" ");
  }
  if (buttonStateUp == 0 && pwm < 250) {
    fastBlinkLed();
     pwm = pwm + pwmStep;
    analogWrite(fanPwm, pwm);
    Serial.println(pwm);
    if (pwm >= 255) {
      pwm = 255;
    }
    delay(960);
  }
  if (buttonStateDown == 0 && pwm != 0) {
    fastBlinkLed();
    pwm = pwm - pwmStep;
    analogWrite(fanPwm, pwm);
    Serial.println(pwm);
        if (pwm <= 0) {
      pwm = 0;
    }
    delay(960);
  }
  else {
    analogWrite(fanPwm, pwm);
  }
  warningLed(rpm);
}

void warningLed(int rpm) {
  if (rpm <= 2200) {
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
}

void fastBlinkLed() {
  digitalWrite(led, HIGH);
  delay(40);
  digitalWrite(led, LOW);
}
