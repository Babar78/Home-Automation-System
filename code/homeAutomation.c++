#include <LiquidCrystal.h>
#include <Servo.h>

//Wifi 
const int HTTP_PORT = 80; 
const String THINGSPEAK_API_KEY = "X2408W0YWYAHSYR4"; // From our ThingSpeak settings.
const String CHANNEL_FEED_URI_1 = "/update?api_key=" + THINGSPEAK_API_KEY + "&field1="; // Write channel URI
const String CHANNEL_FEED_URI_2 = "/update?api_key=" + THINGSPEAK_API_KEY + "&field2=";
const String CHANNEL_FEED_URI_3 = "/update?api_key=" + THINGSPEAK_API_KEY + "&field3=";
const String CHANNEL_FEED_URI_4 = "/update?api_key=" + THINGSPEAK_API_KEY + "&field4=";

// Initialize ESP8266 for internet connectivity.
String ssid = "Simulator Wifi"; // Tinkercad uses this SSID
String password = ""; // Password is not needed.
String host = "api.thingspeak.com";


Servo motor1;
Servo motor2;
int pos = 0;
int pir = A4;
LiquidCrystal lcd(2,4,7,8,12,13);


const int trigpin = 10;
const int echopin = 9;
int gas_sensor = A3;
const int temp_sensor = A5;
const int photo_resistor = A2;

int bulb1 = 5;
int bulb2 = 6;
int fan1 = 11;
int fan2 = 3;

byte automatic[] = {
  B00000,
  B00000,
  B01110,
  B10001,
  B10101,
  B10001,
  B01110,
  B00000
};

double distancecm;
double duration;
float temp_reading;
float temp;
float temp_final;
int i;
int Room_Occupied;


int bootstrapESP8266(void) {

  Serial.begin(115200);
  Serial.println("AT");
  delay(10);
    
  if (!Serial.find("OK")) return 1;  

  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);
  
  if (!Serial.find("OK")) return 2;

  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + HTTP_PORT);
}

void setup()
{
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  motor1.attach(A0);
  motor2.attach(A1);
  motor1.write(pos);
  motor2.write(pos);
  lcd.createChar(0,automatic);
  lcd.begin(16,2);
  lcd.setCursor(4,0);
  lcd.print("Complete");
  lcd.setCursor(3,1);
  lcd.print("Automation ");
  lcd.write(byte(0));
  delay(1000);
  lcd.clear();
  Serial.begin(9600);
  bootstrapESP8266();
}

void loop()
{
  int gasSensorValue= analogRead(gas_sensor);
  int tempSensorValue=analogRead(temp_final);
  int photoSensorValue=analogRead(photo_resistor);
  Serial.println("Gas Sensor Value "+String(gasSensorValue));
  Serial.println("Temperature Sensor Value "+String(tempSensorValue));
  Serial.println("Photo Sensor Value "+String(photoSensorValue));

  String httpPacket_1 = "GET "+CHANNEL_FEED_URI_1 + String(gasSensorValue) +
                    "&" + CHANNEL_FEED_URI_2 + String(tempSensorValue) +
                    "&" + CHANNEL_FEED_URI_3 + String(photoSensorValue) + "&" + CHANNEL_FEED_URI_4 + String(Room_Occupied) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int packetLength_1 = httpPacket_1.length();

  Serial.print("AT+CIPSEND=");
  Serial.println(packetLength_1);
  delay(100);
  
  Serial.print(httpPacket_1);
  delay(100);
  
  
  if (!Serial.find("SEND OK\r\n")) return;
  
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  duration = pulseIn(echopin,HIGH);
  distancecm = (duration*0.034)/2;

  if(distancecm >305){
  Serial.println("Room is Free!");
    Room_Occupied = 0;
  }
  else{
  Room_Occupied= 1;
  }
  
  int analog_sensor = analogRead(gas_sensor);
  temp_reading = analogRead(temp_sensor);
  temp = (temp_reading/1023)*5000;
  temp_final = (temp - 500)/10;
  

  int photo = analogRead(photo_resistor);
  
  
  if(photo <= 425 & distancecm <= 305 & analog_sensor < 180)
  {
  lcd.clear();
  analogWrite(bulb1,255);
  analogWrite(bulb2,255);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2");
  lcd.setCursor(0,1);
  lcd.print("100");
  lcd.setCursor(4,1);
  lcd.print("100");
    delay(400);
  }
  else if(photo > 425 & photo <=517 & distancecm <= 305 & analog_sensor < 180)
  {
  lcd.clear();
  analogWrite(bulb1,150);
  analogWrite(bulb2,150);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2");
  lcd.setCursor(0,1);
  lcd.print("75");
  lcd.setCursor(4,1);
  lcd.print("75");
    delay(400);
  }
   else if(photo > 517 & photo <=574 & distancecm <= 305 & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(bulb1,110);
    analogWrite(bulb2,110);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2");  
  lcd.setCursor(0,1);
  lcd.print("50");
  lcd.setCursor(4,1);
  lcd.print("50");
    delay(400);
  }
   else if(photo > 574 & photo <=630 & distancecm <= 305 & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(bulb1,64);
    analogWrite(bulb2,64);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2");  
  lcd.setCursor(0,1);
  lcd.print("25");
  lcd.setCursor(4,1);
  lcd.print("25");
    delay(400);
  }
   else if(photo > 630 & distancecm <= 305 & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(bulb1,0);
    analogWrite(bulb2,0);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2");  
  lcd.setCursor(0,1);
  lcd.print("OFF");
  lcd.setCursor(4,1);
  lcd.print("OFF");
    delay(400);
  }
  else if(distancecm > 305  & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(bulb1,0);
    analogWrite(bulb2,0);
     analogWrite(fan1,0);
    analogWrite(fan2,0);
  lcd.setCursor(3,0);
  lcd.print("System off");
  
  lcd.setCursor(1,1);
  lcd.print("Nobody in Room");
 // lcd.print("OFF ");
    delay(400);
  }
  
  else if(analog_sensor >= 180)
  {
    lcd.clear();
     analogWrite(bulb1,255);
    analogWrite(bulb2,255);
     analogWrite(fan1,255);
    analogWrite(fan2,255);
    motoropen();
  
     lcd.setCursor(3,0);
  lcd.print("DANGER");
  
  lcd.setCursor(0,1);
  lcd.print("EXIT FROM ROOM");
 // lcd.print("OFF ");
    delay(400);
  }
  if(temp_final >= 20 & temp_final < 30 & distancecm <= 305 & analog_sensor < 180)
  {
    lcd.clear();
    analogWrite(fan1,64);
    analogWrite(fan2,64);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2"); 
  lcd.setCursor(8,1);
  lcd.print("25");
  lcd.setCursor(12,1);
  lcd.print("25");
    delay(400);
  }
  else if(temp_final >= 30 & temp_final < 45  & distancecm <= 305 & analog_sensor < 180)
  {
     lcd.clear();
    analogWrite(fan1,128);
    analogWrite(fan2,128);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2"); 
  lcd.setCursor(8,1);
  lcd.print("50");
  lcd.setCursor(12,1);
  lcd.print("50");
    delay(400);
  }
  else if(temp_final >= 45 & temp_final < 60  & distancecm <= 305 & analog_sensor < 180)
  {
     lcd.clear();
    analogWrite(fan1,150);
    analogWrite(fan2,150);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2"); 
  lcd.setCursor(8,1);
  lcd.print("75");
  lcd.setCursor(12,1);
  lcd.print("75");
    delay(400);
  }
   else if( temp_final < 0  & distancecm <= 305 & analog_sensor < 180)
  {
     lcd.clear();
    analogWrite(fan1,0);
    analogWrite(fan2,0);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2"); 
  lcd.setCursor(8,1);
  lcd.print("OFF");
  lcd.setCursor(12,1);
  lcd.print("OFF");
    delay(400);
  }
   else if(temp_final >= 60 & distancecm <= 305 & analog_sensor < 180)
  {
     lcd.clear();
    analogWrite(fan1,255);
    analogWrite(fan2,255);
  lcd.setCursor(0,0);
  lcd.print("BL1");
  lcd.print(" BL2");
  lcd.print(" FN1 ");
  lcd.print("FN2"); 
  lcd.setCursor(8,1);
  lcd.print("100");
  lcd.setCursor(12,1);
  lcd.print("100");
    delay(400);
  }
 
   int pirvalue = digitalRead(pir);
  if(pirvalue == HIGH)
  {
     lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Somebody Enter");
    lcd.setCursor(0,1);
    lcd.print("Gates - OPENING");
    motoropen();
    delay(400);
  }
  else if(analog_sensor < 180)
  {
   motorclose();
  }
  
  
  
  }

void motoropen()
{
 for(;pos <= 90;pos+=10)
 {
  motor1.write(pos);
   motor2.write(pos);
   delay(80);
 }
}
void motorclose()
{
 for(;pos>=0;pos-=10)
 {
  motor1.write(pos);
   motor2.write(pos);
   delay(80);
 }
  delay(100);
}

// WIP.
void close() {
  // Gracefully close the connection.
  Serial.print("AT+CIPCLOSE");
  
  // Else if fails, close anyway.
  if (!Serial.find("OK")) return;
}
