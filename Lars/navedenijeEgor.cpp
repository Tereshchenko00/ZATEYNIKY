// че добавить: позиционирование в (0;0). хз.

struct Turret{
    public: // главные штуки. доступны для вызова в setup и loop
        float Cord[2] = {0,0}; // текущие углы турельки

        void moveTo(float angleX, float angleY){ // собственно главная функция, двигающая турельку.
            float moveAngles[2] = {0, 0};

            moveAngles[0] = calculateAngle(Cord[0], angleX); // считаем, сколько ехать по Х
            moveAngles[1] = calculateAngle(Cord[1], angleY); // считаем, сколько ехать по Y

            turnX(moveAngles[0]); // крутимся по Х
            turnY(moveAngles[1]); // крутимся по Y
        }


        void turnX(float angle){
            // заставляем мотор Х крутиться на угол angle
            // при этом мотор Y должен компенсировать вращение вокруг червяка. 
            // как именно - зависит именно от червяка
            Cord[0] += angle;
        }
        void turnY(float angle){
            // заставляем мотор Y крутиться так, чтобы 
            // червяк прокрутил шестеренку на угол angle
            Cord[1] += angle;
        }
    private: // вспомогательные штуки, недоступны снаружи структуры

        float calculateAngle(float curAngle, float targetAngle){ // вычисляем то самое смещение по 1 оси(подробно описано в документации)
            float directAngle = targetAngle - curAngle;  
            return directAngle; 
        }



        int angleToStepsX(float degrees){
            // переводит углы в шаги мотора для оси Х
            // я хз как это сделать
            int steps = 0;
            return steps;
        }

        int angleToStepsY(float degrees){
            // то же самое для мотора на оси Y 
            // нужны какие-то коэфициенты из-за червячной передачи
            int steps = 0;
            return steps;
        }
        

};

Turret Lazer; 


 void setup(){
    Lazer.turnX(45.0);  // Повернули на 45
    Lazer.turnY(30.0);  // Повернули на 30
                              // щас мы в (45, 30) относительно 0

    Lazer.moveTo(10.0, 30); // крутимся в положение (10, 30) относительно 0


};

void loop(){


};