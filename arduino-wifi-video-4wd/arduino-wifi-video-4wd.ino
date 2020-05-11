/*

  Forward: press to issue ONA  release ONF
  Back: press to issue ONB  release ONF
  Turn left: press to issue ONC  release ONF
  Turn right: press to issue OND  release ONF
  Stop: press to issue ONE  release ONF
  
  WIFI The program function is to press the corresponding button to perform the operation,
  and release the button to stop

*/

int Left_motor=8;       //Left motor  (IN3)output 0 forward output 1 backward
int Left_motor_pwm=9;   //Left motor  PWM speed regulation

int Right_motor_pwm=10; //Right motor PWM speed regulation
int Right_motor=11;     //Right motor (IN1)output 0 forward output 1 backward    
char buffer[18];        //Character array of serial buffer

void setup()
{
     pinMode(Left_motor,OUTPUT);      // PIN 8  (PWM)
     pinMode(Left_motor_pwm,OUTPUT);  // PIN 9  (PWM)
     pinMode(Right_motor_pwm,OUTPUT); // PIN 10 (PWM) 
     pinMode(Right_motor,OUTPUT);     // PIN 11 (PWM)
     Serial.begin(9600);
     Serial.flush();
}
void back()
{
  digitalWrite(Right_motor,LOW);      // Right motor forward
  digitalWrite(Right_motor_pwm,HIGH); //Right motor forward PWM     
  analogWrite(Right_motor_pwm,150);   //PWM ratio 0 ~ 255 speed regulation
  
  digitalWrite(Left_motor,LOW);       // Left motor forward
  digitalWrite(Left_motor_pwm,HIGH);  //Left motor forward PWM     
  analogWrite(Left_motor_pwm,150);    //PWM ratio 0 ~ 255 speed regulation
//  delay(time * 100);                //Execution time can be adjusted  
}

void brake()
{
  
  digitalWrite(Right_motor_pwm,LOW);
  analogWrite(Right_motor_pwm,0);

  digitalWrite(Left_motor_pwm,LOW);
  analogWrite(Left_motor_pwm,0);
//  delay(time * 100);
}

void spin_left() //original left
{
   digitalWrite(Right_motor,LOW);
  digitalWrite(Right_motor_pwm,HIGH);   
  analogWrite(Right_motor_pwm,150);
  
  digitalWrite(Left_motor,LOW);
  digitalWrite(Left_motor_pwm,LOW); 
  analogWrite(Left_motor_pwm,0);
//  delay(time * 100);
}

void right()         //Turn left (Left wheel back, right wheel forward) - ex spin left
{
  digitalWrite(Right_motor,LOW);
  digitalWrite(Right_motor_pwm,HIGH);     
  analogWrite(Right_motor_pwm,150);
  
  digitalWrite(Left_motor,HIGH);
  digitalWrite(Left_motor_pwm,HIGH); 
  analogWrite(Left_motor_pwm,150);
//  delay(time * 100);
}

void spin_right() //original right
{
   digitalWrite(Right_motor,LOW);
  digitalWrite(Right_motor_pwm,LOW);     
  analogWrite(Right_motor_pwm,0);
  
 
  digitalWrite(Left_motor,LOW);
  digitalWrite(Left_motor_pwm,HIGH);    
  analogWrite(Left_motor_pwm,150);
//  delay(time * 100);
}

void left()        //Turn right (right wheel back, left wheel forward) - ex spin_right
{
  digitalWrite(Right_motor,HIGH);
  digitalWrite(Right_motor_pwm,HIGH);
  analogWrite(Right_motor_pwm,150);
  
  
  digitalWrite(Left_motor,LOW);
  digitalWrite(Left_motor_pwm,HIGH);     
  analogWrite(Left_motor_pwm,150);
//  delay(time * 100);  
}

void run()
{
  digitalWrite(Right_motor,HIGH);
  digitalWrite(Right_motor_pwm,HIGH); 
  analogWrite(Right_motor_pwm,150);
  
  digitalWrite(Left_motor,HIGH);
  digitalWrite(Left_motor_pwm,HIGH);    
  analogWrite(Left_motor_pwm,150);
//  delay(time * 100);
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
      brake(); 
  }
    if((data[0] == 'O') && (data[1] == 'N')&& (data[2] == 'F'))
   {
    Serial.println("Stop!"); 
     brake();  
   }

}
