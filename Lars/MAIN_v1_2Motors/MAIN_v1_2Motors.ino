#include <AccelStepper.h>



  // че добавить: позиционирование в (0;0). хз.
  // структура турели, может крутиться куда ей скажут

  struct Turret{
      public: // главные штуки. доступны для вызова в setup и loop
          // конструктор
          Turret(int stepX, int dirX, int stepY, int dirY): stepperX(AccelStepper::DRIVER, stepX, dirX), stepperY(AccelStepper::DRIVER, stepY, dirY){
          // Настройки моторов
              stepperX.setMaxSpeed(1000);
              stepperY.setMaxSpeed(1000);
              stepperX.setAcceleration(1000);
              stepperY.setAcceleration(1000);
          }

          float Cord[2] = {0,0}; // текущие углы турельки

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
              int steps = angleToStepsY(angle);
              stepperY.move(steps);
              while(stepperY.distanceToGo() != 0){
                  stepperY.run();
              }
              Cord[1] += angle;
          }
      private: // вспомогательные штуки, недоступны снаружи структуры
          AccelStepper stepperX;
          AccelStepper stepperY;
          float calculateAngle(float curAngle, float targetAngle){
              float diff = targetAngle - curAngle;
                                                      // Нормализация угла (если нужно)
              if(diff > 180) diff -= 360;
              if(diff < -180) diff += 360;
              return diff;
}



          int angleToStepsX(float degrees){
              // шаги = градусы / угол шага мотора

              return degrees / 1.8; // Простой пример
          }

          int angleToStepsY(float degrees){
              // то же самое для мотора на оси Y 
              // нужны какие-то коэфициенты из-за червячной передачи
              int steps = (degrees / 1.8) * 8;
              return steps;
          }
          

  };
  
  const int pin_dirX = 3;
  const int pin_dirY = 21;
  const int pin_stepX = 5;
  const int pin_stepY = 23;

  Turret Lazer(pin_stepX, pin_dirX, pin_stepY, pin_dirY); 


  void setup(){
    pinMode(36, OUTPUT);
    digitalWrite(36, HIGH);
    Lazer.moveTo(45.0, 30);
    delay(1000);
    Lazer.moveTo(-90.0, 0);
    delay(1000);
    Lazer.moveTo(90.0, 60.0);
    delay(2000);
    // Lazer.moveTo(10.0, 30); // крутимся в положение (10, 30) относительно 0


  };

  void loop(){


    Lazer.moveTo(0, 90);
    delay(1000);
    Lazer.moveTo(90, 0);
    delay(1000);


  };
