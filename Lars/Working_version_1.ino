#include <AccelStepper.h>
#include <SoftwareSerial.h>
// структура турели, может крутиться куда ей скажут

struct Turret {
  public: // главные штуки. доступны для вызова в setup и loop
    Turret(int stepX, int dirX, int stepY, int dirY, int ms1, int ms2, int ms3, int kX, int kY):
      pin_MS1(ms1), pin_MS2(ms2), pin_MS3(ms3),
      buttonPinX(kX), buttonPinY(kY),
      stepperX(AccelStepper::DRIVER, stepX, dirX),
      stepperY(AccelStepper::DRIVER, stepY, dirY)
    {
      // Тело конструктора
      pinMode(pin_MS1, OUTPUT);
      pinMode(pin_MS2, OUTPUT);
      pinMode(pin_MS3, OUTPUT);
      pinMode(buttonPinY, INPUT);
      pinMode(buttonPinX, INPUT);
      stepperX.setMaxSpeed(1000);
      stepperY.setMaxSpeed(1000);
      stepperX.setAcceleration(1000);
      stepperY.setAcceleration(900000);
    }

    float Cord[2] = {0, 0}; // текущие углы турельки
    int pin_MS1, pin_MS2, pin_MS3, buttonPinX, buttonPinY;



    void moveTo(float angleX, float angleY) { // собственно главная функция, двигающая турельку.

      float moveAngles[2] = {0, 0};
      moveAngles[0] = calculateAngle(Cord[0], angleX); // считаем, сколько ехать по Х
      moveAngles[1] = calculateAngle(Cord[1], angleY); // считаем, сколько ехать по Y

      turnX(moveAngles[0]); // крутимся по Х
      turnY(moveAngles[1]); // крутимся по Y
    }


    void turnX(float angle) { // мотор Х с доводкой
      angleToStepsX(angle);

      setFullStepMode();
      stepperX.move(fullSteps);
      while (stepperX.distanceToGo() != 0) {
        stepperX.run();
      }
      setMicroStepMode();
      stepperX.move(microSteps);
      while (stepperX.distanceToGo() != 0) {
        stepperX.run();
      }
      Cord[0] += angle;
    }

    void turnY(float angle) {
      // Переводим угол в шаги с учетом червячной передачи
      int steps = angleToStepsY(angle);

      // Двигаем мотор
      stepperY.move(steps);
      while (stepperY.distanceToGo() != 0) {
        stepperY.run();
      }

      // Обновляем координату
      Cord[1] += angle;
    }


    void calibrateY() {
      // Вниз до кнопки
      while (digitalRead(buttonPinY) == HIGH) {
        stepperY.move(-10);
        stepperY.runToPosition();

      }

      // Устанавливаем нижний предел
      Cord[1] = yMinAngle;
      stepperY.setCurrentPosition(angleToStepsY(yMinAngle));

      // едем в центр
      turnY(-yMinAngle); // двигаемся на -yMinAngle градусов

    }
    void calibrateX() {
      stepperX.setMaxSpeed(1000);
      stepperX.setAcceleration(1000);
      // Вниз до кнопки
      setFullStepMode();

      while (digitalRead(buttonPinX) == LOW) {
        stepperX.move(-1);
        stepperX.runToPosition();

      }


      // Устанавливаем нижний предел
      Cord[0] = xMinAngle;
      //stepperY.setCurrentPosition(angleToStepsX(yMinAngle));
      stepperX.setMaxSpeed(1000);
      stepperX.setAcceleration(1000);
      // едем в центр
      turnX(-xMinAngle); // двигаемся на -yMinAngle градусов

    }

  private: // вспомогательные штуки, недоступны снаружи структуры
    AccelStepper stepperX;
    AccelStepper stepperY;

    // крайние положения
    float yMinAngle = -45;
    float xMinAngle = -45;

    // только 2 режима
    int fullSteps = 0;    // в режиме 1/1
    int microSteps = 0;   // в режиме 1/16

    // Только 2 угла шага
    float fullStepAngle = 1.8;     // 1/1
    float microStepAngle = 0.1125; // 1/16


    void setFullStepMode() {
      digitalWrite(pin_MS1, 0);
      digitalWrite(pin_MS2, 0);
      digitalWrite(pin_MS3, 0);
    }

    void setMicroStepMode() {
      digitalWrite(pin_MS1, 1);
      digitalWrite(pin_MS2, 1);
      digitalWrite(pin_MS3, 1);
    }

    void angleToStepsX(float degrees) {
      // Очищаем
      fullSteps = 0;
      microSteps = 0;



      // Берём абсолютное значение
      float absDeg = fabs(degrees);

      // Считаем полные шаги
      fullSteps = (int)(absDeg / fullStepAngle);

      // Остаток после полных шагов
      float remaining = absDeg - (fullSteps * fullStepAngle);

      // Остаток переводим в микрошаги
      microSteps = (int)(remaining / microStepAngle + 0.5);

      // Исправление переполнения микрошагов до применения знака
      if (microSteps >= 16) {
        fullSteps += (microSteps / 16);
        microSteps = microSteps % 16;
      }

      // Учитываем знак угла
      if (degrees < 0) {
        fullSteps = -fullSteps;
        microSteps = -microSteps;
      }
    }

    int angleToStepsY(float degrees) {
      // червячная передача: коэффициент 8:1
      int steps = (degrees / 1.8) * 8;
      return steps;
    }


    float calculateAngle(float curAngle, float targetAngle) {
      float diff = targetAngle - curAngle;
      if (diff > 180) diff -= 360;
      if (diff < -180) diff += 360;
      return diff;
    }
};

const String startMessage = "START";
String incomingData = "";
const int avariaPin = 34;

const int pin_dirX = 3;
const int pin_dirY = 21;
const int pin_stepX = 5;
const int pin_stepY = 23;

const int pin_MS1 = 13;
const int pin_MS2 = 11;
const int pin_MS3 = 9;

const int pin_knopkaX = 46;
const int pin_knopkaY = 36;

Turret Lazer(pin_stepX, pin_dirX, pin_stepY, pin_dirY, pin_MS1, pin_MS2, pin_MS3, pin_knopkaX, pin_knopkaY);
SoftwareSerial HC12(10, 7); // радиомодуль
void setup() {
  pinMode(avariaPin, INPUT);
  pinMode(38, OUTPUT);
  digitalWrite(38, HIGH);

  Serial.begin(9600);
  HC12.begin(9600);
  //
  //  Lazer.calibrateY();
  //  delay(3000);
  //  Lazer.moveTo(40,40);
  //  delay(1000);
  //  Lazer.moveTo(-40, 40);
  //  delay(1000);
  //  Lazer.moveTo(-40, -40);
  //  delay(1000);
  //  Lazer.moveTo(40, -40);
  //  delay(1000);
  //  Lazer.moveTo(0, 0);
  //Lazer.moveTo(10.0, 30); // крутимся в положение (10, 30) относительно 0
  delay(100);
  digitalWrite(38, 0);
  Lazer.calibrateY();
  Lazer.calibrateX();
  delay(1000);
  HC12.println("ready");
};

bool flagStart = 0; // временно 1 чтобы не тестить радиомодуль

bool mission1 = 0;
bool mission2 = 0;
bool mission3 = 0;
bool mission4 = 0;
bool finish = 0;
bool avaria = 0;

float anX = 0;
float anY = 0;

unsigned long timing;

void loop() {
  if (digitalRead(avariaPin) == LOW) {
    avaria = 1;
    digitalWrite(38, LOW);
    HC12.println("MISSION FAILED");

    for (;;);
  }
  if (flagStart == 0) {
    while (HC12.available()) {
      char c = HC12.read();
      if (c == '\n') {
        Serial.println(incomingData);
        if (incomingData == "START") {
          digitalWrite(38, HIGH);
          HC12.println("OK");
          flagStart = true;
          Lazer.moveTo(-40, 0);
          anX = -40;
          anY = 0;
          break;
        }
        incomingData = "";
      } else {
        incomingData += c;
      }
    }
  }

  if (flagStart) { // MISSION
    mission1 = 1;

  }
  if (millis() - timing >= 2500) {//не 3000, тк двигателям нужно примерно 500 мс для поворота
    if (mission1 && !(mission2 || mission3 || mission4)) { // Выполняется 1
      if (anX >= 40) {
        digitalWrite(38, 0);
        Lazer.calibrateY(); // потом сменить на общую калибровку!!!!!!!!
        Lazer.calibrateX();

        Lazer.moveTo(0, -40);
        digitalWrite(38, HIGH);
        // изменить углы, провести калибровку?
        anY = -40;
        anX = 0;

        mission2 = 1;
      }
      else {
        anX += 10;
      }
    }

    if (mission1 && mission2 && !(mission3 || mission4)) { // Выполняется 2
      if (anY >= 40) {
        digitalWrite(38, 0);
        Lazer.calibrateY(); // потом сменить на общую калибровку!!!!!!!!
        Lazer.calibrateX();

        Lazer.moveTo(-40, -40);
        digitalWrite(38, HIGH);
        // изменить углы, провести калибровку?
        anY = -40;
        anX = -40;
        mission3 = 1;
      }
      else {
        anY += 10;
      }
    }

    if (mission1 && mission2 && mission3 && !mission4) { // Выполняется 3
      if (anX >= 40 && anY >= 40) {
        digitalWrite(38, 0);
        Lazer.calibrateY(); // потом сменить на общую калибровку!!!!!!!!
        Lazer.calibrateX();
        Lazer.moveTo(-40, 40);
        digitalWrite(38, HIGH);
        anX = -40;
        anY = 40;
        mission4 = 1;
      }

      else {
        anX += 10;
        anY += 10;
      }
    }

    if (mission1 && mission2 && mission3 && mission4) { // Выполняется 4
      if (anX >= 40 && anY <= 40) {
        digitalWrite(38, LOW);
        Lazer.calibrateY(); // потом сменить на общую калибровку!!!!!!!!
        Lazer.calibrateX();
        Lazer.moveTo(0, 0);
        digitalWrite(38, HIGH);
        anX = 0;
        anY = 0;
        finish = 1;
        flagStart = 0;
      }

      else {
        anX += 10;
        anY -= 10;
      }
    }

    if (flagStart) { // движение и отправка телеметрии
      Lazer.moveTo(anX, anY);

      HC12.print("Lazer_ZATEY; ");
      HC12.print(millis());
      HC12.print("; ");
      HC12.print(anX);
      HC12.print(", ");
      HC12.print(anY);
      HC12.print("; ");

      HC12.print(flagStart);
      HC12.print("; ");
      HC12.print(mission1);
      HC12.print("; ");
      HC12.print(mission2);
      HC12.print("; ");
      HC12.print(mission3);
      HC12.print("; ");
      HC12.print(mission4);
      HC12.print("; ");
      HC12.print(finish);
      HC12.println("; ");
    }
    timing = millis();

  }


}
