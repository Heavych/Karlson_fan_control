int fanPwm = 3;             // Пин PWM для управления вентилятором
int fanTach = 2;             // Пин для чтения сигнала оборотов вентилятора
int buttonUp = 7;           // Пин кнопки увеличения скорости вентилятора
int buttonDown = 8;         // Пин кнопки уменьшения скорости вентилятора
int buttonStateUp = 0;      // Состояние кнопки увеличения скорости
int buttonStateDown = 0;    // Состояние кнопки уменьшения скорости
int pwm = 0;                // Значение ШИМ (Широтно-Импульсная Модуляция) для управления скоростью вентилятора
int pwmStep = 25;           // Шаг изменения ШИМ (10%)
int led = 13;               // Пин светодиода
unsigned long pulseDuration;
unsigned long previousMillis = 0;

const long interval = 1000;  // Интервал для вывода данных в миллисекундах (1 секунда)

void setup() {
  Serial.begin(9600);
  pinMode(fanPwm, OUTPUT);  // Установка пина управления вентилятором как выход
  pinMode(fanTach, INPUT);  // Установка пина для чтения сигнала оборотов вентилятора как вход
  digitalWrite(fanTach, HIGH);  // Включение внутреннего подтягивающего резистора для сигнала оборотов
  pinMode(led, OUTPUT);  // Установка пина светодиода как выход
  pinMode(buttonUp, INPUT_PULLUP);  // Установка пина кнопки увеличения скорости как вход с подтягивающим резистором
  pinMode(buttonDown, INPUT_PULLUP);  // Установка пина кнопки уменьшения скорости как вход с подтягивающим резистором
}

void loop() {
  buttonStateUp = digitalRead(buttonUp);
  buttonStateDown = digitalRead(buttonDown);

  pulseDuration = pulseIn(fanTach, LOW);  // Измерение длительности импульса сигнала оборотов вентилятора
  double frequency = 1000000.0 / pulseDuration;  // Расчет частоты вентилятора
  int rpm = frequency / 2 * 60;  // Расчет оборотов в минуту

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.print("Длительность импульса: ");
    Serial.println(pulseDuration);
    Serial.print("ШИМ: ");
    Serial.println(pwm);
    Serial.print("Обороты вентилятора: ");
    Serial.println(rpm);
    Serial.println(" ");
  }

  // Увеличение скорости вентилятора при нажатии кнопки увеличения
  if (buttonStateUp == 0 && pwm < 250) {
    fastBlinkLed();  // Мигание светодиода
    pwm = pwm + pwmStep;  // Увеличение ШИМ
    analogWrite(fanPwm, pwm);  // Управление скоростью вентилятора
    Serial.println(pwm);
    if (pwm >= 255) {
      pwm = 255;
    }
    delay(960);
  }

  // Уменьшение скорости вентилятора при нажатии кнопки уменьшения
  if (buttonStateDown == 0 && pwm != 0) {
    fastBlinkLed();  // Мигание светодиода
    pwm = pwm - pwmStep;  // Уменьшение ШИМ
    analogWrite(fanPwm, pwm);  // Управление скоростью вентилятора
    Serial.println(pwm);
    if (pwm <= 0) {
      pwm = 0;
    }
    delay(960);
  } else {
    analogWrite(fanPwm, pwm);
  }

  warningLed(rpm);  // Включение светодиода предупреждения в зависимости от оборотов вентилятора
}

// Включение светодиода предупреждения при низких оборотах
void warningLed(int rpm) {
  if (rpm <= 2200) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}

// Быстрое мигание светодиода
void fastBlinkLed() {
  digitalWrite(led, HIGH);
  delay(40);
  digitalWrite(led, LOW);
}
