#include <Servo.h>

int Left_motor_back = 8;                       // (IN1)
int Left_motor_go = 9;                         // (IN2)
int Left_motor_en = 10;                        // (EN1)
int Right_motor_go = 6;                        // (IN3)
int Right_motor_back = 7;                      // (IN4)
int Right_motor_en = 5;                        // (EN2)

Servo myservo;
int servo = 4;                                 // servo pin 4
int angle = 90;
 
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
char buffer[18];                               // Character array of serial buffer

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
    
  pinMode(Left_motor_go,    OUTPUT);           // PIN 5 (PWM)
  pinMode(Left_motor_back,  OUTPUT);           // PIN 6 (PWM)
  pinMode(Right_motor_go,   OUTPUT);           // PIN 2 (PWM)
  pinMode(Right_motor_back, OUTPUT);           // PIN 3 (PWM) 
  pinMode(Right_motor_en,    OUTPUT);
  pinMode(Left_motor_en,    OUTPUT);
  Serial.begin(9600);
  Serial.flush();
}

void loop()
{
    if(Serial.available() > 0)
    {
       int index = 0;
       delay(100);            //Delay to wait for the serial port to receive the data, otherwise it will execute the subsequent program when it receives 1 byte
       int numChar = Serial.available();  
       if(numChar > 15)       //Confirm that the data does not overflow and should be less than the buffer size
       {
         numChar = 15;
        }
       while(numChar--)
      {
          buffer[index++] = Serial.read();  //Store serial data into the buffer word by word
      }
       splitString(buffer);  //self-explanatory
    }
}

void splitString(char *data)
{
       Serial.print("Data entered:");
       Serial.println(data);
       char *parameter;
       parameter = strtok(data, " ,");    //string token，divide and intercept data according to spaces
       Serial.print("***");
       Serial.println(parameter);
while(parameter != NULL)
{
    setLED(parameter);
    parameter = strtok(NULL, " ,");      //string token，split and intercept again until the character after interception is empty
    Serial.print("---");
    Serial.println(parameter);      
}
   for(int x = 0; x < 16; x++)           //clear buffer
  {
   buffer[x] = '\0';
  }
     Serial.flush();
  }

void setLED(char *data)
{
   if((data[0] == 'O') && (data[1] == 'N')&& (data[2] == 'A'))
   {
    Serial.println("go forward!"); 
      run();
  }
  if((data[0] == 'O') && (data[1] == 'N')&& (data[2] == 'B'))
   {
    Serial.println("go back!"); 
      back(); 
 }
    if((data[0] == 'O') && (data[1] == 'N')&& (data[2] == 'C'))
   {
      Serial.println("go left!"); 
      left();
  }
    if((data[0] == 'O') && (data[1] == 'N')&& (data[2] == 'D'))
   {
     Serial.println("go right!"); 
      right(); 
  }
    if((data[0] == 'O') && (data[1] == 'N')&& (data[2] == 'E'))
   {
      Serial.println("Stop!"); 
      stop(); 
  }
    if((data[0] == 'O') && (data[1] == 'N')&& (data[2] == 'F'))
   {
    Serial.println("Stop!"); 
     stop();  
   }

}

//TEST SERIAL

/* void loop()
{
if (Serial.available() > 0)
  {
  command = Serial.read();
  switch (command) {
    case 'w':run(); delay(1000); stop(); break;
    case 's':back(); delay(1000); stop(); break;
    case 'a':spin_left(); delay(1000); stop(); break;
    case 'd':spin_right(); delay(1000); stop(); break;
    case 'x':stop();break;
  }
 }
}
*/

// DEBUG SERIAL

/*
void loop()
{
if (Serial.available()){
command = Serial.read();
value = command;
Serial.print(value);
}
}
*/

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
}
