#include "./IRremote.h"
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long last = millis();

#define run_car     '1'
#define back_car    '2'
#define left_car    '3'
#define right_car   '4'
#define stop_car    '0'

enum {
  enSTOP = 0,
  enRUN,
  enBACK,
  enLEFT,
  enRIGHT,
  enTLEFT,
  enTRIGHT
} enCarState;

//speed level control
int control = 250;          //PWM
#define level1  0x08
#define level2  0x09
#define level3  0x0A
#define level4  0x0B
#define level5  0x0C
#define level6  0x0D
#define level7  0x0E
#define level8  0x0F

int Left_motor_back = 4;     //(IN1)
int Left_motor_go = 5;       //(IN2)
int Left_motor_en = 3;       //(ENA)
int Right_motor_go = 6;      //(IN3)
int Right_motor_back = 7;    //(IN4)
int Right_motor_en = 8;      //(ENB)

int incomingByte = 0;       //data byte
String inputString = "";
boolean newLineReceived = false;
boolean startBit  = false;
String returntemp = "";

//int BUZZER = 3;
//int LED = 12;
int KEY = 2;

int Echo = A1;  // ultrasonic echo
int Trig = A0; //  ultrasonic trigger
int Distance = 0;

const int SensorRight = A3;   	//ir line sensor
const int SensorLeft = A2;     	//ir lnie sensor
int SL;
int SR;

const int SensorRight_2 = A4;    //barrier sensor
const int SensorLeft_2 = A5;     //barrier sensor
int SL_2;
int SR_2;

int g_carstate = enSTOP;
int g_modeSelect = 0;
int g_modeComunication = 0;
int g_AllState = 0;
int g_IRRealse = 0;

int serial_putc( char c, struct __file * )
{
  Serial.write( c );
  return c;
}
void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}

void setup()
{
  pinMode(Left_motor_go, OUTPUT);    // PIN 5 (PWM)
  pinMode(Left_motor_back, OUTPUT);  // PIN 9 (PWM)
  pinMode(Right_motor_go, OUTPUT);   // PIN 6 (PWM)
  pinMode(Right_motor_back, OUTPUT); // PIN 10 (PWM)
  pinMode(Right_motor_en, OUTPUT);   // PIN 8
  pinMode(Left_motor_en, OUTPUT);    // PIN 7

//  pinMode(BUZZER, OUTPUT);
//  pinMode(LED, OUTPUT);
  pinMode(KEY, INPUT_PULLUP);

  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);

  pinMode(SensorRight, INPUT);
  pinMode(SensorLeft, INPUT);

  Serial.begin(9600);

//  digitalWrite(BUZZER, HIGH);
  digitalWrite(Left_motor_en, HIGH);
  digitalWrite(Right_motor_en, HIGH);

  g_carstate = enSTOP;
  g_modeComunication = 0;
  g_modeSelect = 0;
  irrecv.enableIRIn();
  pinMode(RECV_PIN, INPUT_PULLUP);

  printf_begin();
}

void Distance_test()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  float Fdistance = pulseIn(Echo, HIGH); 
  Fdistance = Fdistance / 58;
  Distance = Fdistance;
}

void run()
{
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, control);

  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, control);

}

void brake()
{
  digitalWrite(Left_motor_back, LOW);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
}

void right()
{
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, 180);

  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
}

void spin_left()
{
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, control);

  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_back, control);

}

void left()
{
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);

  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, 180);
}

void spin_right()
{
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_back, control);

  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, control);

}

void back()
{
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_back, control);

  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_back, control);

}
void whistle() //too much noise so comment
{
  int i;
  for (i = 0; i < 80; i++)
  {
//    digitalWrite(BUZZER, LOW);
    delay(10);
//    digitalWrite(BUZZER, HIGH);
    delay(1);
  }
  for (i = 0; i < 100; i++)
  {
//    digitalWrite(BUZZER, LOW);
    delay(20);
//    digitalWrite(BUZZER, HIGH);
    delay(2);
  }
}

void Bluetooth(void)
{
  if (newLineReceived)
  {
    if (inputString[1] == 'M' && inputString[2] == 'O' && inputString[3] == 'D' && inputString[4] == 'E')
    {
      if (inputString[6] == '0')
      {
        g_carstate = enSTOP;
        g_modeSelect = 0;
        g_AllState = 0;
//        BeepOnOffMode();
      }
      else
      {
        switch (inputString[5])
        {
          case '0': g_modeSelect = 0; break;
          case '1': g_modeSelect = 1; break;
          case '2': g_modeSelect = 2; break;
          case '3': g_modeSelect = 3; break;
          default: g_modeSelect = 0; break;
        }
        g_AllState = 0;
//         BeepOnOffMode();
      }
    }
    else if (g_modeSelect == 0 && g_AllState == 0)
    {
      switch (inputString[1])
      {
        case run_car:   g_carstate = enRUN; Serial.print("run\r\n"); break;
        case back_car:  g_carstate = enBACK;  Serial.print("back\r\n"); break;
        case left_car:  g_carstate = enLEFT; Serial.print("left\r\n"); break;
        case right_car: g_carstate = enRIGHT; Serial.print("right\r\n"); break;
        case stop_car:  g_carstate = enSTOP; Serial.print("brake\r\n"); break;
        default: g_carstate = enSTOP; break;
      }
      if (inputString[3] == '1')
      {
        spin_left();
        Serial.print("revolve\r\n");
        delay(2000);
        brake();
      }
      else if (inputString[3] == '2')
      {
        spin_right();
        Serial.print("revolve\r\n");
        delay(2000);
        brake();
      }
      if (inputString[5] == '1')
      {
        whistle();
        Serial.print("whistle\r\n");
      }
      if (inputString[7] == '1')
      {
        control += 50;
        if (control > 255)
        {
          control = 255;
        }
        Serial.print("expedite\r\n");
      }
      if (inputString[9] == '1')
      {
        control -= 50;
        if (control < 50)
        {
          control = 100;
        }
        Serial.print("reduce\r\n");
      }

      Distance_test();
      returntemp = "$0,0,0,0,0,0,0,0,0,0,0,";
      returntemp.concat(Distance);
      returntemp += "cm,8.2V#";
      Serial.print(returntemp);

    }
    inputString = "";   // clear the string
    newLineReceived = false;

  }
}

void Key_Scan(void)
{
  int val;
  while (!digitalRead(KEY))
  {
    delay(10);
    val = digitalRead(KEY);
    if (val == LOW)
    {
      if (g_modeComunication == 0)
      {
        g_modeComunication = 1;
//        digitalWrite(LED, HIGH);
      }
      else
      {
        g_modeComunication = 0;
//        digitalWrite(LED, LOW);
      }
//      digitalWrite(BUZZER, LOW);
      delay(100);
//      digitalWrite(BUZZER, HIGH);
      while (!digitalRead(KEY));
    }
    else
//      digitalWrite(BUZZER, HIGH);
        delay(10);
  }
}

void CarControl()
{
  if (g_modeSelect != 2 )
  {
    switch (g_carstate)
    {
      case enSTOP: brake(); break;
      case enRUN: run(); break;
      case enLEFT: left(); break;
      case enRIGHT: right(); break;
      case enBACK: back(); break;
      case enTLEFT: spin_left(); break;
      case enTRIGHT: spin_right(); break;
      default: brake(); break;
    }
   
  }
}

void IR_Deal()
{
  if (irrecv.decode(&results)) 
  {
    //Serial.println(results.value, HEX);

    //if (((results.value >> 16) & 0x0000ffff) == 0x00ff)
    //{
    //printf("$AR,HSX,%08lX#\n", results.value);
    //  00FFA25D  
    //  00FF02FD   +
    //  00FF9867   -
    //  00FFA857   
    //  00FFE01F   
    //  00FF906F   
    //  00FF6897   0  beep
    //  00FF18E7   2  
    //  00FF10EF   4  
    //  00FF38C7   5  
    //  00FF5AA5   6  
    //  00FF4AB5   8  
    //  00FF42BD   7  
    //  00FF52AD   9  

    //  00FFE21D   MENU   1
    //  00FFC23D   Back   2
    //  00FF906F   NEXT   3
    //  00FFB04F   C      4
    switch (results.value)
    {
//      case 0x00FFA25D: g_carstate = enSTOP; g_AllState = 0; g_modeSelect = 0; BeepOnOffMode() ;break;
//      case 0x00FFA857: g_carstate = enSTOP; g_AllState = 0; BeepOnOffMode() ; break; // PLAY
//      case 0x00FFE01F: g_AllState = 1; g_modeSelect--; if (g_modeSelect == -1) g_modeSelect = 3; break;
//      case 0x00FF906F: g_AllState = 1; g_modeSelect++; if (g_modeSelect == 4) g_modeSelect = 0;  break;
//      case 0x00FFE21D: g_AllState = 1; g_modeSelect = 0;  ModeBEEP(g_modeSelect); break; // MENU  1
//      case 0x00FFC23D: g_AllState = 1; g_modeSelect = 2;  ModeBEEP(g_modeSelect); break; // NEXT  2
//      case 0x00FF906F: g_AllState = 1; g_modeSelect = 1;  ModeBEEP(g_modeSelect); break; // Back  3
//      case 0x00FFB04F: g_AllState = 1; g_modeSelect = 3;  ModeBEEP(g_modeSelect); break; // C  4

      default: break;
    }
    if (g_modeSelect == 0 && g_AllState == 0)
    {
      switch (results.value)
      {
        
        case 0x00FF02FD: control += 50; if (control > 255) control = 255; break;
        case 0x00FF9867: control -= 50; if (control < 50) control = 100; break;

        case 0x00FF6897: whistle(); break;
        case 0x00FF18E7:  g_carstate = enRUN; break;
        case 0x00FF10EF:  g_carstate = enLEFT; break;
        case 0x00FF38C7:  g_carstate = enSTOP; break;
        case 0x00FF5AA5:  g_carstate = enRIGHT; break;
        case 0x00FF4AB5:  g_carstate = enBACK; break;
        case 0x00FF42BD:  g_carstate = enTLEFT; break;
        case 0x00FF52AD:  g_carstate = enTRIGHT; break;
        default: break;

      }
     
    }

    //}
    last = millis();
    irrecv.resume();
  }
  else if (millis() - last > 120)
  {
    g_carstate = enSTOP;
    last = millis();
  }

}

//void ModeBEEP(int mode)
//{
//  for (int i = 0; i < mode + 1; i++)
//  {
//    digitalWrite(BUZZER, LOW);
//    delay(100);
//    digitalWrite(BUZZER, HIGH);
//    delay(100);
//  }
//  delay(100);
//  digitalWrite(BUZZER, HIGH);
//}

//void BeepOnOffMode()
//{
//  digitalWrite(BUZZER, LOW);
//  delay(1000);
//  digitalWrite(BUZZER, HIGH);
//}

void track()
{
  SR = digitalRead(SensorRight);
  SL = digitalRead(SensorLeft);

  if (SL == LOW && SR == LOW)
    g_carstate = enRUN;
  else if (SL == LOW & SR == HIGH)
    g_carstate = enRIGHT;
  else if (SR == LOW & SL ==  HIGH)
    g_carstate = enLEFT;
  else
    g_carstate = enSTOP;
}

void ultrason_obstacle_avoiding()
{
  Distance_test();

  if (Distance < 25)
  {
    delay(10);
    Distance_test();
    while (Distance < 25)
    {

      spin_right();
      delay(300);
      brake();
      Distance_test();
    }
  }
  else
    run();
}

void Infrared_follow()
{
  SR_2 = digitalRead(SensorRight_2);
  SL_2 = digitalRead(SensorLeft_2);
  if (SL_2 == LOW && SR_2 == LOW)
    g_carstate = enRUN;
  else if (SL_2 == HIGH & SR_2 == LOW)
    g_carstate = enRIGHT;
  else if (SR_2 == HIGH & SL_2 == LOW)
    g_carstate = enLEFT;
  else
    g_carstate = enSTOP;
}

void loop()
{
  Key_Scan();

  if (g_modeComunication == 0)
  {
    IR_Deal();
  }
  else
  {
    Bluetooth(); // have to try swap ir_deal and bt
  }

  if (g_AllState == 0)
  {
    switch (g_modeSelect)
    {
      case 1: track(); break;
      case 2: ultrason_obstacle_avoiding(); break;
      case 3: Infrared_follow(); break;
    }
  }
  CarControl();


}

void serialEvent()
{
  while (Serial.available())
  {
    incomingByte = Serial.read();
    if (incomingByte == '$')
    {
      startBit = true;
    }
    if (startBit == true)
    {
      inputString += (char) incomingByte;
    }
    if (incomingByte == '#')
    {
      newLineReceived = true;
      startBit = false;
    }
  }
}
