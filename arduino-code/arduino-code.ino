#include <Servo.h>
#include <Keypad.h>


//CONFIGURACION DE KEYPAD PARA CONTRASENA
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
int FAN_MAIN_ROOM = A3;
int FAN_BASIC_ROOM = A4;
int SPRINKLERS_A = A5;
int SPRINKLERS_B = A6;
int SPRINKLERS_C = A7;

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

//ESTADO DE VENTILACION DE LOS CUARTOS
int STATE_FAN_MAIN_ROOM = 0;
int STATE_FAN_BASIC_ROOM = 0;

//LECTURA ANALOGA DE TEMPERATURA
int VAL_TEMP_SENSOR;
//LECTURA DIGITAL DE LOS SENSORES DE PROXIMIDAD
int VAL_PROXIMITY_DOOR_GARDEN;
int VAL_PROXIMITY_DOOR_GARAGE;

//LECTURA DE LA PUERTA CON CERROJO DIGITAL
int VAL_DIGITAL_DOOR=0;

//ALMACENAR EL ESTADO DE LOS ASPERSORES
int VAL_SPRINKLERS = 0;

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
  pinMode(FAN_MAIN_ROOM,OUTPUT);  //CONFIGURACION DE LA SALIDA AL VENTILADOR DEL CUARTO PRINCIPAL
  pinMode(FAN_BASIC_ROOM,OUTPUT); //CONFIGURACION DE LA SALIDA AL VENTILADOR DEL CUARTO SECUNDARIO
  SERVO_GARDEN.attach(SERVO_DOOR_GARDEN); //SE CONFIGURA LA SALIDA DE LOS SERVO GARDEN Y GARAGE
  SERVO_GARAGE.attach(SERVO_DOOR_GARAGE);

    //CONFIGURACION DE SALIDAS DE LOS REGADEROS
  pinMode(SPRINKLERS_A,OUTPUT);
  pinMode(SPRINKLERS_B,OUTPUT);
  pinMode(SPRINKLERS_C,OUTPUT);

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
  evaluateDigitalDoor();  //SE LLAMA LA FUNCION QUE EVALUARA TODO CON RESPECTO A LA PUERTA DE CERROJO DIGITAL
  getTemperature(VAL_TEMP_SENSOR);    //SE LLAMA LA FUNCION DE OBTENCION DE LA TEMPERATURA
  sendData();     //FUNCION PARA ENVIAR LOS DATOS DEL ARDUINO POR EL PUERTO SERIAL
}

//EVENTO SERIAL QUE COMUNICA CON LA WEB
void serialEvent(){
  int response = Serial.read();
  switch (response) {
  case 65:
    digitalWrite(FAN_MAIN_ROOM,HIGH);
    STATE_FAN_MAIN_ROOM = 1;
    break;
  case 66:
    digitalWrite(FAN_MAIN_ROOM,LOW);
    STATE_FAN_MAIN_ROOM = 0;
    break;
  case 67:
    digitalWrite(FAN_BASIC_ROOM,HIGH);
    STATE_FAN_BASIC_ROOM = 1;
    break;
  case 68:
    digitalWrite(FAN_BASIC_ROOM,LOW);
    STATE_FAN_BASIC_ROOM = 0;
    break;
  case 69:
    digitalWrite(SPRINKLERS_A,HIGH);
    digitalWrite(SPRINKLERS_B,HIGH);
    digitalWrite(SPRINKLERS_C,HIGH);
    VAL_SPRINKLERS = 1;
    break;
  case 70:
    digitalWrite(SPRINKLERS_A,LOW);
    digitalWrite(SPRINKLERS_B,LOW);
    digitalWrite(SPRINKLERS_C,LOW);
    VAL_SPRINKLERS = 0;
    break;
  default:
    break;
}
}

//FUNCION PARA EVALUAR LA PUERTA CON CERROJO DIGITAL
void evaluateDigitalDoor(){
  VAL_DIGITAL_DOOR = 0;
  char key = keypad.getKey();
  if(key){
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

  //VENTILACION DE LOS CUARTOS
  Serial.print(",");
  Serial.print("\"mainRoom\":");
  Serial.print(STATE_FAN_MAIN_ROOM);

  Serial.print(",");
  Serial.print("\"basicRoom\":");
  Serial.print(STATE_FAN_BASIC_ROOM);

  //REGADEROS
  Serial.print(",");
  Serial.print("\"sprinklers\":");
  Serial.print(VAL_SPRINKLERS);

  //ENDING
  Serial.println("}");

  return;
}
