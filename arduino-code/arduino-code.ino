int TEMP_SENSOR = A0;
int PROXIMITY_DOOR_ONE = 2;
int PROXIMITY_DOOR_TWO = 3;

int VAL_TEMP_SENSOR;
int VAL_PROXIMITY_DOOR_ONE;
int VAL_PROXIMITY_DOOR_TWO;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(TEMP_SENSOR,INPUT); //DECLARAR COMO ENTRADA DE SENSOR DE TEMPERATURA 
  pinMode(PROXIMITY_DOOR_ONE,INPUT); //DECLARAR COMO ENTRADA EL SENSOR DE PROXIMIDAD DE LA PUERTA UNO
  pinMode(PROXIMITY_DOOR_TWO,INPUT);  //DECLARAR COMO ENTRADA EL SENSOR DE PROXIMIDAD DE LA PUERTA DOS
}

void loop() {
  VAL_TEMP_SENSOR = analogRead(TEMP_SENSOR);
  VAL_PROXIMITY_DOOR_ONE = digitalRead(PROXIMITY_DOOR_ONE);
  VAL_PROXIMITY_DOOR_TWO = digitalRead(PROXIMITY_DOOR_TWO);
  evaluateDoorOne(VAL_PROXIMITY_DOOR_ONE);
  evaluateDoorTwo(VAL_PROXIMITY_DOOR_TWO);
  
  getTemperature(VAL_TEMP_SENSOR);
  delay(1000);
}

//EVENTO SERIAL QUE COMUNICA CON LA WEB
void serialEvent(){
  int response = Serial.read();

  switch (response) {
  case 65:
    // statements
    digitalWrite(2,HIGH);
    break;
  case 66:
    // statements
    digitalWrite(2,LOW);
    break;
  default:
    // statements
    break;
}
}


void getTemperature(int val){
  float temp = ((val / 1023.0) * 5000)/10;
  Serial.println(temp);
  return;
}

void evaluateDoorOne(int val){
  if(val>0){
    Serial.println("Movimiento Puerta 1 - Abriendo");
  }
}

void evaluateDoorTwo(int val){
  if(val>0){
    Serial.println("Movimiento Puerta 2 - Abriendo");
  }
}
