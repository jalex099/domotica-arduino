#include <Servo.h>

#include <NewPing.h>    //usamos la libreria para el sensor iltrasonico
const int UltrasonicPin = 3; //pin ultrasonic tr
const int MaxDistance = 200; //maxima distancia
Servo servoMotor;   //seclarar el servo
NewPing sonar(UltrasonicPin, 2, MaxDistance); //creacion del objeto
void setup() {
  Serial.begin(9600);
  servoMotor.attach(4); //ubicacion del ping del servo
}
void loop() {
  
  delay(1000);                      // esperar 1000ms entre pings 
  int cm = sonar.ping_cm();   //obtiene el valor en cm del sensor
  Serial.print(cm); // obtener el valor en cm (0 = fuera de rango)
  Serial.println("cm");
  if(cm >50){     //condicion si esta a 50 cm
    servoMotor.write(180);
  }else{
    servoMotor.write(0);
  }
}
