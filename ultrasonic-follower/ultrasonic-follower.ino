int Left_motor_back = 6;                       //(IN1)
int Left_motor_go = 5;                         //(IN2)
//int Left_motor_en = 3;                       //(EN1)
int Right_motor_go = 2;                        //(IN3)
int Right_motor_back = 3;                      //(IN4)
//int Right_motor_en = 11;                     //(EN2)

double Input; // to calculate the Error(E) of PID controller with parameter of distance
double Output ; //the parameter which set the speed of motors

//PID parameters          
double Kp = 14 ; //constant of proportion 
double Ki = 0.0 ;//constant of integrity, Kp*Ti(the time constant)
double Kd = 0.4; //constant of differential, Kp*Td(the time constant) 
const int Setpoint = 5;// the supposed distance between the robot car and the object
double ki,kd;
 
//Pin Setup
const int TrigPinR = 13; //the left ultrasonic trig 
const int EchoPinR = 12;// the left ultrasonic echo
const int TrigPinL = 10;//the right ultrasonic trig
const int EchoPinL = 9;//the right ultrasonic echo

//variable
//lastInput and lastTime is the variable for the data of input and time before the current one.
double lastInput;
double lastTime;
//the Minimum and the Maximum of motor's speed forward and backward spesifically
const int OutMin = -150;
const int OutMax = 225; 
//set the periodicaltime to build the integrity part of PID 
int SampleTime = 1000;
double  systemtime;
//the distance between the car and the object for both left ultrasonic and right ultrasonic
float distanceL;
float distanceR;
float distance;
//the integrity of distance
double ITerm;
double error;

//function
//set the left ultrasonic sensor to get the distance
void LeftSensor();
//set the right ultrasonic sensor to get the distance 
void RightSensor();
//function to help robot car turn right/left as the object moves
void TurnRight();
void TurnLeft();
 
void setup()
{
  Serial.begin(9600);   
  pinMode(TrigPinL, OUTPUT);
  pinMode(EchoPinL, INPUT);
  pinMode(TrigPinR, OUTPUT);
  pinMode(EchoPinR, INPUT);
  pinMode(Left_motor_go,    OUTPUT);           // PIN 5 (PWM)
  pinMode(Left_motor_back,  OUTPUT);           // PIN 6 (PWM)
  pinMode(Right_motor_go,   OUTPUT);           // PIN 2 (PWM)
  pinMode(Right_motor_back, OUTPUT);           // PIN 3 (PWM)  
}

void loop()
{
  //get Distance measured  
  LeftSensor();
  RightSensor();
  //if the distance detected by two ultrasonic sensor is different significantly, which means
  //the object is not in front of the robot car, the car need to turn either right or left.
  //and here, 50 is a threshold
  if (distanceL-distanceR >40)
  {
   left();
  }
  else if (distanceR-distanceL >40)
  {
   right();   
  }
  //besides turning, the robot car is gonna go in straight line
  else
  {
  //let the reall distance equals to the mean of right and left ultrasonic sensors
  distance = (distanceL + distanceR)/2; 
  //as the range of distance (cm) measured by ultrasonic sensors is so large for a input to take, we mapped it to 0-255
  Input = map(distance, 0, 500, 0, 255);

  //set time to second
  unsigned long now = millis();
  double dtime = (now - lastTime);
  //set time in the unit of millisecond to second
  systemtime = now/1000;

  //the time constant of integrity
  ki = Ki*SampleTime;
  //the time constant of differential
  kd = Kd/SampleTime;
  //the code below is the core algorithm to compute PID
  //a time period longer than a sampletime should be counting
  if(dtime >= SampleTime)
  { 
    error = -Setpoint + Input;
    Serial.print(error);
    Serial.print("\n");
    ITerm+=(ki *error);
    double dInput = (Input - lastInput);
    Output = Kp*error + ITerm - kd*dInput;
    if(Output > OutMax) Output = OutMax;
    else if(Output < OutMin) Output = OutMin;
    if(ITerm > OutMax) ITerm= OutMax;
    else if(ITerm < OutMin) ITerm= OutMin;
    //prepare for the calculation of next interval's PID outcome value
    lastInput = Input;
    lastTime = now;
  }
  
  //Write the output as calculated by the PID function
   if (distance <= 5)
   {
    back();
   }
   //if distance is more than 400, which is kind of a threshold, it means the robot car may lost the target to follow,
   //as a result, we'd better to make it slower.
   else
   {
    run();
   }
   }
  delay(10); 
}


//the function to make the left altrasonic controller calculate the distance
void LeftSensor()
{
  digitalWrite(TrigPinL, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPinL,HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPinL,LOW);
  distanceL = pulseIn(EchoPinL, HIGH)/58.00;
}


//the function to make the right altrasonic controller calculate the distance
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

void run()
{
    //run forward
    //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motro back
  analogWrite(Left_motor_go, 250);

   //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_go, 250);
}

void runSlow()
{
    //run forward
    //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motro back
  analogWrite(Left_motor_go, 130);

   //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_go, 130);
}

void left()
{
 //left motor stop
   digitalWrite(Left_motor_go, LOW);    //prohibit left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motor back
  analogWrite(Left_motor_back, 0);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_go, 230);
}

void right()
{
  //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motor back
  analogWrite(Left_motor_go, 230);

  //right motor stop
  digitalWrite(Right_motor_go, LOW);   //prohibit right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_go, 0);
}

void spin_left(int time)
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     //prohibit left motor go
  digitalWrite(Left_motor_back, HIGH);  //enable left motor back
  analogWrite(Left_motor_back, 230);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_go, 230);

  delay(time * 100);
}

void spin_right(int time)
{
 //left motor go
  digitalWrite(Left_motor_go, HIGH);    //enable left motor go
  digitalWrite(Left_motor_back, LOW);   //prohibit left motor back
  analogWrite(Left_motor_go, 230);

  //right motor back
  digitalWrite(Right_motor_go, LOW);    //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH); //enable right motor back
  analogWrite(Right_motor_back, 230);

  delay(time * 100);
}

void back()
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     //prohibit left motor go
  digitalWrite(Left_motor_back, HIGH);  //enable lef tmotor back
  analogWrite(Left_motor_back, 250);

  //right motor back
  digitalWrite(Right_motor_go, LOW);     //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH);  //enable right motor back
  analogWrite(Right_motor_back, 250);
}
