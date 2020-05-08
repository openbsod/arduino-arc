int Echo3 = A1; //front
int Trig3 = A0;
int Echo2 = A3; //left
int Trig2 = A2;
int Echo1 = A5; //right
int Trig1 = A4;

int in1 = 4;
int in2 = 5;
int in3 = 6;
int in4 = 7;
int ENA = 8;
int ENB = 3;
int ABS = 250;
int Left_Distance = 0, Right_Distance = 0, Middle_Distance = 0;

void Forward(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ENA, ABS);
  analogWrite(ENB, ABS);
}

void Back(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ENA, ABS);
  analogWrite(ENB, ABS);
}

void left(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(ENA, 140);
  analogWrite(ENB, ABS);
}

void right(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(ENA, ABS);
  analogWrite(ENB, 140);
}

void Stop(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(ENA, LOW);
  analogWrite(ENB, LOW);
}

int Left_Distance_test(){
  digitalWrite(Trig2, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig2, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig2, LOW);
  float Fdistance = pulseIn(Echo2, HIGH);
  delay(10);
  Fdistance = Fdistance / 29 / 2;
  return (int)Fdistance;
}

int Middle_Distance_test(){
  digitalWrite(Trig3, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig3, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig3, LOW);
  float Fdistance = pulseIn(Echo3, HIGH);
  delay(10);
  Fdistance = Fdistance / 29 / 2;
  return (int)Fdistance;
}

int Right_Distance_test(){
  digitalWrite(Trig1, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig1, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig1, LOW);
  float Fdistance = pulseIn(Echo1, HIGH);
  delay(10);
  Fdistance = Fdistance / 29 / 2;
  return (int)Fdistance;
}

void setup() {
  pinMode(Echo1, INPUT);
  pinMode(Trig1, OUTPUT);
  pinMode(Echo2, INPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Echo3, INPUT);
  pinMode(Trig3, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  Stop();
}

void loop() {
  Left_Distance = Left_Distance_test();
  delay(10);
  Middle_Distance = Middle_Distance_test();
  delay(10);
  Right_Distance = Right_Distance_test();
  delay(10);
  if(Middle_Distance <= 15){
    if(Right_Distance > Left_Distance){
      if((Right_Distance <= 15) && (Left_Distance <= 15)){
        Stop();
        delay(50);
        Back();
        delay(100);
      }
      else{
        right();
      }
      }
      else if(Right_Distance < Left_Distance){
        if((Right_Distance <= 15) && (Left_Distance <= 15)){
        Stop();
        delay(50);
        Back();
        delay(100);
      }
      else{
        left();
      }
    }
    }
    else if(Right_Distance <= 10){
      left();
    }
    else if(Left_Distance <= 10){
      right();
    }
    else{
      Forward();
    }
}
