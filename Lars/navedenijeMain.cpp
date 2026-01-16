#include <AccelStepper.h>



  // че добавить: позиционирование в (0;0). хз.
  // структура турели, может крутиться куда ей скажут

  struct Turret{
    public: // главные штуки. доступны для вызова в setup и loop
        Turret(int stepX, int dirX, int stepY, int dirY, int pin_MS1, int pin_MS2, int pin_MS3): 
        stepperX(AccelStepper::DRIVER, stepX, dirX), stepperY(AccelStepper::DRIVER, stepY, dirY){
        // Настройки моторов
            stepperX.setMaxSpeed(1000);
            stepperY.setMaxSpeed(1000);
            stepperX.setAcceleration(1000);
            stepperY.setAcceleration(1000);
        }

        float Cord[2] = {0,0}; // текущие углы турельки
        int pin_MS1, pin_MS2, pin_MS3;
        
        void moveTo(float angleX, float angleY){ // собственно главная функция, двигающая турельку.
             
            float moveAngles[2] = {0, 0};
            moveAngles[0] = calculateAngle(Cord[0], angleX); // считаем, сколько ехать по Х
            moveAngles[1] = calculateAngle(Cord[1], angleY); // считаем, сколько ехать по Y

            turnX(moveAngles[0]); // крутимся по Х
            turnY(moveAngles[1]); // крутимся по Y
        }


        void turnX(float angle){
            int steps = angleToStepsX(angle);
            stepperX.move(steps);
            while(stepperX.distanceToGo() != 0){
                stepperX.run();
            }
            Cord[0] += angle;
        }

        void turnY(float angle){
            // заставляем мотор Y крутиться так, чтобы 
            // червяк прокрутил шестеренку на угол angle
            Cord[1] += angle;
        }
  private: // вспомогательные штуки, недоступны снаружи структуры
        AccelStepper stepperX;
        AccelStepper stepperY;

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

        void angleToStepsX(float degrees){ // на вход подаем угол

              // шаги = градусы / угол шага мотора
              // return degrees / 1.8;


        }
        int angleToStepsY(float degrees){
            // то же самое для мотора на оси Y 
            // нужны какие-то коэфициенты из-за червячной передачи
            int steps = (degrees / 1.8) * 8;
            return steps;
        }

        float calculateAngle(float curAngle, float targetAngle){
            float diff = targetAngle - curAngle;
            if(diff > 180) diff -= 360;
            if(diff < -180) diff += 360;
            return diff;
        }
};
  
const int pin_dirX = 2;
const int pin_dirY = 3;
const int pin_stepX = 4;
const int pin_stepY = 5;

const int pin_MS1 = 6;
const int pin_MS2 = 7;
const int pin_MS3 = 8;

Turret Lazer(pin_stepX, pin_dirX, pin_stepY, pin_dirY, pin_MS1, pin_MS2, pin_MS3); 


void setup(){

  Lazer.moveTo(45.0, 0);
  delay(1000);
  Lazer.moveTo(-90.0, 0);
  delay(1000);
  Lazer.moveTo(90.0, 0.0);
  delay(2000);
  // Lazer.moveTo(10.0, 30); // крутимся в положение (10, 30) относительно 0

};

void loop(){


  Lazer.moveTo(0, 0);
  delay(1000);
  Lazer.moveTo(90, 0);
  delay(1000);
};