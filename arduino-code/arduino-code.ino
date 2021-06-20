#include <Servo.h>
#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS]={
  {
    '7','8','9','A'
  },{
    '4','5','6','B'
  },{
    '1','2','3','C'
  },{
    '*','0','#','D'
  }
};

byte rowPins[ROWS]={4,7,8,9};
byte colPins[COLS]={10,11,12,13};

//CONTRASENIA
char Password[4];
int dir=0;

//INPUTS DEL ARDUINO
int TEMP_SENSOR = A0;
int PROXIMITY_DOOR_GARDEN = 2;
int PROXIMITY_DOOR_GARAGE = 3;
int SERVO_DOOR_GARDEN = 5;
int SERVO_DOOR_GARAGE = 6;

//VARIABLES DEL SERVO
Servo SERVO_GARDEN;
Servo SERVO_GARAGE;

//VARIABLES DEL KEYPAD
Keypad keypad = Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);

//DIRECCION DEL SERVO
int DIR_SERVO_GARDEN=0;
int DIR_SERVO_GARAGE=0;

//TEMPERATURA EN DECIMAL
float TEMPERATURE;

//LECTURA ANALOGA DE TEMPERATURA
int VAL_TEMP_SENSOR;
//LECTURA DIGITAL DE LOS SENSORES DE PROXIMIDAD
int VAL_PROXIMITY_DOOR_GARDEN;
int VAL_PROXIMITY_DOOR_GARAGE;

//LECTURA DE LA PUERTA CON CERROJO DIGITAL
int VAL_DIGITAL_DOOR;

//ALMACENA EL VALOR ANTERIOR DE TEMPERATURA
int PAST_TEMP_VALUE; 

//LED OPCIONES
int LED_OK = A2;
int LED_ERROR = A1; 



void setup() {
  Serial.begin(9600);
  pinMode(TEMP_SENSOR,INPUT); //DECLARAR COMO ENTRADA DE SENSOR DE TEMPERATURA 
  pinMode(PROXIMITY_DOOR_GARDEN,INPUT); //DECLARAR COMO ENTRADA EL SENSOR DE PROXIMIDAD DE LA PUERTA UNO
  pinMode(PROXIMITY_DOOR_GARAGE,INPUT);  //DECLARAR COMO ENTRADA EL SENSOR DE PROXIMIDAD DE LA PUERTA DOS
  pinMode(SERVO_DOOR_GARDEN,OUTPUT);  //CONFIGURACION DE MODO DE SALIDA EN LOS PUERTOS DE LOS SERVOS
  pinMode(SERVO_DOOR_GARAGE,OUTPUT);
  SERVO_GARDEN.attach(SERVO_DOOR_GARDEN); //SE CONFIGURA LA SALIDA DE LOS SERVO GARDEN Y GARAGE
  SERVO_GARAGE.attach(SERVO_DOOR_GARAGE);

  SERVO_GARDEN.write(0); //SE ESTABLECEN LOS SERVOS EN DIRECCION 0
  SERVO_GARAGE.write(0);
  pinMode(LED_OK,OUTPUT);   //SE ESTABLECEN LOS LEDS DE ERROR Y CORRECTO
  pinMode(LED_ERROR,OUTPUT);
}

void loop() {
  
  VAL_TEMP_SENSOR = analogRead(TEMP_SENSOR); //SE OBTIENE EL VALOR ANALOGO DEL SENSOR
  VAL_PROXIMITY_DOOR_GARDEN = digitalRead(PROXIMITY_DOOR_GARDEN);   //SE OBTIENE EL VALOR DEL SENSOR DE PROXIMIDAD
  VAL_PROXIMITY_DOOR_GARAGE = digitalRead(PROXIMITY_DOOR_GARAGE);   //SE OBTIENE EL VALOR DEL SENSOR DE PROXIMIDAD
  evaluateDoorGarden(VAL_PROXIMITY_DOOR_GARDEN);    //SE LLAMA LA FUNCION DE EVALUAR LA PUERTA DEL PATIO INTERNO
  evaluateDoorGarage(VAL_PROXIMITY_DOOR_GARAGE);    //SE LLAMA LA FUNCION DE EVALUAR LA PUERTA DE LA COCHERA
  digitalDoor();
  getTemperature(VAL_TEMP_SENSOR);    //SE LLAMA LA FUNCION DE OBTENCION DE LA TEMPERATURA
  sendData();     //FUNCION PARA ENVIAR LOS DATOS DEL ARDUINO POR EL PUERTO SERIAL
}

//EVENTO SERIAL QUE COMUNICA CON LA WEB
void serialEvent(){
  int response = Serial.read();
  switch (response) {
  case 65:
    // statements
    digitalWrite(A1,HIGH);
    break;
  case 66:
    // statements
    digitalWrite(A1,LOW);
    break;
  default:
    // statements
    break;
}
}

//FUNCION PARA EVALUAR LA PUERTA CON CERROJO DIGITAL
void digitalDoor(){
  char key = keypad.getKey();
  if(key){
    VAL_DIGITAL_DOOR = 0;
    digitalWrite(LED_OK,LOW);
    digitalWrite(LED_ERROR,LOW);
    Password[dir]=key;
    dir+=1;
    if(dir==4){
      if(Password[0] == '1' && Password[1] == '2' && Password[2] == '3' && Password[3] == '4'){
        digitalWrite(LED_OK,HIGH);
        VAL_DIGITAL_DOOR = 1;
        dir=0;
      } else{
        digitalWrite(LED_ERROR,HIGH);
        dir=0;
        VAL_DIGITAL_DOOR = 2;
      }
    }
  };
}

//FUNCION QUE OBTIENE LA TEMPERATURA
void getTemperature(int val){
  TEMPERATURE = ((val / 1023.0) * 5000)/10;
  if(PAST_TEMP_VALUE != val){
    PAST_TEMP_VALUE = val;
  }
  return;
}


//FUNCION PARA EVALUAR EL ESTADO DE LA PUERTA DEL PATIO INTERNO
void evaluateDoorGarden(int val){
  if(val>0){
      while(DIR_SERVO_GARDEN < 90){
        SERVO_GARDEN.write(DIR_SERVO_GARDEN);
        DIR_SERVO_GARDEN+=1;
        delay(2);
      }
  } else{
    while(DIR_SERVO_GARDEN > 0){
        SERVO_GARDEN.write(DIR_SERVO_GARDEN);
        DIR_SERVO_GARDEN-=1;
        delay(2);
      }
  }
}

//FUNCION PARA EVALUAR EL ESTADO DE LA PUERTA DE LA COCHERA
void evaluateDoorGarage(int val){
  if(val>0){
      while(DIR_SERVO_GARAGE < 90){
        SERVO_GARAGE.write(DIR_SERVO_GARAGE);
        DIR_SERVO_GARAGE+=1;
        delay(2);
      }
  } else{
    while(DIR_SERVO_GARAGE > 0){
        SERVO_GARAGE.write(DIR_SERVO_GARAGE);
        DIR_SERVO_GARAGE-=1;
        delay(2);
      }
  }
}

//FUNCION PARA CREAR UN STRING CON FORMATO JSON PARA PODER DECODIFICARLO EN JAVASCRIPT POSTERIORMENTE
void sendData(){
  Serial.print("{"); //INICIO DEL JSON SSTRING

  //TEMPERATURA
  Serial.print("\"temperature\":");
  Serial.print(TEMPERATURE);

  //PUERTA PATIO INTERNO

  Serial.print(",");
  Serial.print("\"gardenDoor\":");
  Serial.print(VAL_PROXIMITY_DOOR_GARDEN);

  //PUERTA COCHERA

  Serial.print(",");
  Serial.print("\"garageDoor\":");
  Serial.print(VAL_PROXIMITY_DOOR_GARAGE);

  //CERROJO DIGITAL
  Serial.print(",");
  Serial.print("\"digitalDoor\":");
  Serial.print(VAL_DIGITAL_DOOR);


  //ENDING
  Serial.println("}");

  return;
}
