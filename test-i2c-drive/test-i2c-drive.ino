#include <Servo.h>
#include <Wire.h>

#define SLAVE_ADDRESS 0x04

int Left_motor_back = 8;                       // (IN1)
int Left_motor_go = 9;                         // (IN2)
int Left_motor_en = 10;                        // (EN1)
int Right_motor_go = 6;                        // (IN3)
int Right_motor_back = 7;                      // (IN4)
int Right_motor_en = 5;                        // (EN2)

Servo myservo;                                 // steering servo pin 4
int angle = 90;
Servo hservo;                                  // horizontal servo pin 11 
int hangle = 80;
Servo vservo;                                  // vertical servo pin 12 
int vangle = 70;
 
//Pin Setup
const int TrigPinR = 34;                       // left ultrasonic trig 
const int EchoPinR = 36;                       // left ultrasonic echo
const int TrigPinL = 38;                       // right ultrasonic trig
const int EchoPinL = 40;                       // right ultrasonic echo

//variables

float distanceL;
float distanceR;
float distance;
const int Setpoint = 5;                        // supposed distance between car and object
int _time = 1;

int number = 0;
int state = 0;

//function
//set the left ultrasonic sensor to get the distance
void LeftSensor();
//set the right ultrasonic sensor to get the distance 
void RightSensor();
 
void setup()
{
//  Serial.begin(115200);   
  pinMode(TrigPinL, OUTPUT);
  pinMode(EchoPinL, INPUT);
  pinMode(TrigPinR, OUTPUT);
  pinMode(EchoPinR, INPUT);

  myservo.attach(4);
  myservo.write(90);
  hservo.attach(11);
  hservo.write(80);
  vservo.attach(12);
  vservo.write(70);
      
  pinMode(Left_motor_go,    OUTPUT);           // PIN 5 (PWM)
  pinMode(Left_motor_back,  OUTPUT);           // PIN 6 (PWM)
  pinMode(Right_motor_go,   OUTPUT);           // PIN 2 (PWM)
  pinMode(Right_motor_back, OUTPUT);           // PIN 3 (PWM) 
  pinMode(Right_motor_en,    OUTPUT);
  pinMode(Left_motor_en,    OUTPUT);
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  //Wire.onRequest(sendData);
  // start serial for output 
  //Serial.begin(9600);
  //Serial.flush();
}

void loop() {
delay(100);
}

// callback for received data
void receiveData(int byteCount){

while(Wire.available()) {
number = Wire.read();
//Serial.print("data received: ");
//Serial.println(number);

  if (number == 1)
  run();
  else if (number == 2)
    stop();
  else if (number == 3)
    hleft();    
  else if (number == 4)
    vup();    
  else if (number == 5)
    vdown();    
  else if (number == 6)
    hright();
  }
}

/*
// callback for sending data
void sendData(){
Wire.write(number);
}
*/

// horizontal left servo
void hleft()
{
  hangle = (hangle + 1) * 1;
  hservo.write(hangle);
}

void vup()
{
  vangle = ( vangle + 1) * 1;
  vservo.write(vangle);
}

// horizontal right servo
void hright()
{
  hangle = ( hangle - 1) * 1;
  hservo.write(hangle);
}

void vdown()
{
  vangle = ( vangle - 1) * 1;
  vservo.write(vangle);
}

//the function to make the left ultrasonic controller calculate the distance
void LeftSensor()
{
  digitalWrite(TrigPinL, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPinL,HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPinL,LOW);
  distanceL = pulseIn(EchoPinL, HIGH)/58.00;
}


//the function to make the right ultrasonic controller calculate the distance
void RightSensor()
{
  digitalWrite(TrigPinR, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPinR,HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPinR,LOW);
  distanceR = pulseIn(EchoPinR, HIGH)/58.00;
}

void stop()
{
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
}  

void back()
{
  //run forward
  //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motro back
  analogWrite(Left_motor_en, 250);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_en, 250);

  delay(_time * 100);
}

void left()
{
  //left motor stop
  digitalWrite(Left_motor_go, LOW);    //prohibit left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motor back
  analogWrite(Left_motor_en, 0);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_en, 230);

  delay(_time * 100);
}

void right()
{
  //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motor back
  analogWrite(Left_motor_en, 230);

  //right motor stop
  digitalWrite(Right_motor_go, LOW);   //prohibit right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_en, 0);

  delay(_time * 100);
}

void spin_left()
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     //prohibit left motor go
  digitalWrite(Left_motor_back, HIGH);  //enable left motor back
  analogWrite(Left_motor_en, 230);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_en, 230);

  delay(_time * 100);
}

void spin_right()
{
  //left motor go
  digitalWrite(Left_motor_go, HIGH);    //enable left motor go
  digitalWrite(Left_motor_back, LOW);   //prohibit left motor back
  analogWrite(Left_motor_en, 230);

  //right motor back
  digitalWrite(Right_motor_go, LOW);    //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH); //enable right motor back
  analogWrite(Right_motor_en, 230);

  delay(_time * 100);
}

void run()
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     //prohibit left motor go
  digitalWrite(Left_motor_back, HIGH);  //enable lef tmotor back
  analogWrite(Left_motor_en, 250);

  //right motor back
  digitalWrite(Right_motor_go, LOW);     //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH);  //enable right motor back
  analogWrite(Right_motor_en, 250);

  //delay(_time * 100);
}
