int Left_motor_back = 6;     //(IN1)
int Left_motor_go = 5;       //(IN2)
//int Left_motor_en = 3;       //(EN1)
int Right_motor_go = 2;      //(IN3)
int Right_motor_back = 3;    //(IN4)
//int Right_motor_en = 11;      //(EN2)
 
// HC-SR04
#define TRIG 12
#define ECHO 13

#define DESIRED_DISTANCE 10
#define DISTANCE_TOLERANCE 1
#define MAX_DISTANCE 10
#define MAX_SPEED  255
float distance = 0.0;
int speed = 0;

// coefficient for speed calculation
float k = 2.0;
float error = 0.0;

void setup() 
{

  pinMode(TRIG, OUTPUT);  
  pinMode(ECHO, INPUT); 

  pinMode(Left_motor_go, OUTPUT);    // PIN 5 (PWM)
  pinMode(Left_motor_back, OUTPUT);  // PIN 9 (PWM)
  pinMode(Right_motor_go, OUTPUT);   // PIN 6 (PWM)
  pinMode(Right_motor_back, OUTPUT); // PIN 10 (PWM)
//  pinMode(Right_motor_en, OUTPUT);   // PIN 8
//  pinMode(Left_motor_en, OUTPUT);    // PIN 7

}

void loop()
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  distance = pulseIn(ECHO, HIGH)*0.017;

  // lets calculate error
  error = DESIRED_DISTANCE - distance;

  if (abs(error) > DISTANCE_TOLERANCE && abs(error) < MAX_DISTANCE)
  {
    // define direction by error sign
    if (error < 0)
    {
      forward();
    }
    else
    {
      back();
    }

    // convert error to motor speed
    // map is abs(error)*k)
    // from 0 to MAX_DISTANCE) 
    // and from 0 to MAX_SPEED)
    speed = map(abs(error)*k, 0, MAX_DISTANCE, 0, MAX_SPEED);
  }
  else
  {
    brake();
  }
}

void forward()
{
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, speed);

  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, speed);
}

void brake()
{
  digitalWrite(Left_motor_back, LOW);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
}

void back()
{
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_back, speed);

  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_back, speed);
}
