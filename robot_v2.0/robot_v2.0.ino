#include <AFMotor.h>  //библиотека для мотора
#include <Servo.h>    //библиотека для сервы

//настройки
#define servo 9       //пин сервы 9 или 10
#define echo 6        //пин hcsr 04
#define trig 2        //пин hcsr 04
#define distance_min 16  //минимальное расстояние до стены (В САНТИМЕТНРАХ)
#define distance_min_po_storonam 30 //минимальное расстояние до стены  по сторонам (В САНТИМЕТНРАХ)

#define motor1_speed 255 //скорость мотора (0 - 255)
#define motor2_speed 255 //скорость мотора (0 - 255)
#define razvorot_time 600 //в милисекундах
#define povorot_time 300 //в милисекундах

AF_DCMotor motor1(1);  //обьявляем мотор
AF_DCMotor motor2(3);  //обьявляем мотор
Servo myservo;         //обьявляем servo

int i, val, d, l, c, r; //переменные

void setup() {
  Serial.begin(9600);     //запуск общения с компом
  motor1.setSpeed(motor1_speed);   //скорость мотора
  motor1.run(RELEASE);             //останавливаем мотор
  motor2.setSpeed(motor2_speed);   //скорость мотора
  motor2.run(RELEASE);             //останавливаем мотор
  myservo.attach(servo);           //обьявляем пин сервы
  myservo.write(90);               //устанавливаем серву посередине (0 - 180)

  pinMode(echo, INPUT);            //конфигурирем цифровой вход для hcsr04
  pinMode(trig, OUTPUT);           //конфигурирем цифровой выход для hcsr04
}

void loop() {
  myservo.write(90);
  Serial.println(distance());  //отправляем рассояние на комп
  if (distance() > distance_min) {  //если расстояние больше минимального
    motor1.run(FORWARD); //включаем моторы
    motor2.run(FORWARD);
  } else { //иначе
    motor1.run(RELEASE); // останавливаемя
    motor2.run(RELEASE);
    check_on_stop(); // смотрим по сторонам
  }
}

//функция для получения расстояния
int distance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return pulseIn(echo, HIGH) / 58.2;
}

void check_on_stop() {//функция проверки при остановке
  myservo.write(45);//смотрим налево
  delay(350);//ждём
  l = distance();//записываем расстояние слева
  myservo.write(135);//смотрим направо
  delay(350);//ждём
  r = distance();//записываем расстояние справа
  myservo.write(90);//возвращаем назад

  if ((r > distance_min_po_storonam) || (l > distance_min_po_storonam)) {//если оба расстояния больше минимального
    if ((l > distance_min_po_storonam) && (l > r)) {//если левое самое большое то
      motor1.run(FORWARD); //поворачиваем налево
      motor2.run(BACKWARD);
      delay(povorot_time);//ждём
      motor1.run(RELEASE);//останавливаемся
      motor2.run(RELEASE);
    }
    if ((r > distance_min_po_storonam) && (r > l)) {//если правое больше
      motor1.run(BACKWARD);//поворачиваем направо
      motor2.run(FORWARD);
      delay(povorot_time);//ждём
      motor1.run(RELEASE);//останавлтваемся
      motor2.run(RELEASE);
    }
  } else {//иначе
    a180();//разворачиваемся
  }
}

//функция разворота
void a180() {
  motor1.run(FORWARD);//разворачиваемся
  motor2.run(BACKWARD);
  delay(razvorot_time);//ждём
  motor1.run(RELEASE);//останавливаемся
  motor2.run(RELEASE);
}
