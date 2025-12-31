
struct Turret{
    public: // главные штуки. доступны для вызова в setup и loop
        float Cord[2] = {0,0};

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
        

}


 void setup(){


}

void loop(){


}