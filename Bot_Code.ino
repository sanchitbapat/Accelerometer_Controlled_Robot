#include <VirtualWire.h>
const int bmotor_right1=6; //right motor positive
const int bmotor_right2=7; //right motor negative
const int bmotor_left1=4; //left motor positive
const int bmotor_left2=5; //left motor negative
const int tpin = 3; //Front proximity sensor
const int upin = 8; //Left proximity sensor
const int vpin = 9; //Right proximity sensor
const int LD = 13; //LED on arduino
void setup()
{
pinMode(bmotor_right1, OUTPUT); //initialize pins as ip/op
pinMode(bmotor_left1, OUTPUT);
pinMode(bmotor_right2, OUTPUT);
pinMode(bmotor_left2, OUTPUT);
Serial.begin(9600);
Serial.println("setup");
vw_setup(2000); //Bits per sec
vw_rx_start(); //Start the receiver PLL running
}
void slow_ahead()
{
digitalWrite(bmotor_right1, HIGH);
digitalWrite(bmotor_right2, LOW);
digitalWrite(bmotor_left1, HIGH);
digitalWrite(bmotor_left2, LOW);
}
void slow_left()
{
digitalWrite(bmotor_right1, HIGH);
digitalWrite(bmotor_right2, LOW);
digitalWrite(bmotor_left1, LOW);
digitalWrite(bmotor_left2, HIGH);
}
void slow_right()
{
digitalWrite(bmotor_right1, LOW);
digitalWrite(bmotor_right2, HIGH);
digitalWrite(bmotor_left1, HIGH);
digitalWrite(bmotor_left2, LOW);
}
void reverse()
{
digitalWrite(bmotor_right1, LOW);
digitalWrite(bmotor_right2, HIGH);
digitalWrite(bmotor_left1, LOW);
digitalWrite(bmotor_left2, HIGH);
}
void stopmotors()
{
digitalWrite(bmotor_right1, LOW);
digitalWrite(bmotor_right2, LOW);
digitalWrite(bmotor_left1, LOW);
digitalWrite(bmotor_left2, LOW);
}
void auto_control() //Function for automatic bot motion
{
int a=digitalRead(tpin);
int b=digitalRead(upin);
int c=digitalRead(vpin);
if(a==0&&b==0&&c==0)
slow_ahead(); //All Low-->Move Ahead
else
if(a==1&&b==1&&c==1) //All High--> Move Reverse
reverse();
else
if(a==1&&b==0&&c==0)
slow_left(); //Move In Any Direction
else
if(a==1&&b==1&&c==0)
slow_right(); //Move Right
else
if(a==1&&b==0&&c==1)
slow_left(); //Move Left
}
void bot_control()
{
uint8_t buf[VW_MAX_MESSAGE_LEN];
uint8_t buflen = VW_MAX_MESSAGE_LEN;
if (vw_get_message(buf, &buflen)) //Non-blocking condition
{
int i;
Serial.print("Got: "); //Message with a good checksum received, dump HEX
for (i = 0; i < buflen; i++)
{
if(buf[i]=='9')
{
Serial.println("AUTO"); //Autonomous control
auto_control();
return;
}
else //Manual/User control
{
if (buf[i]=='1')
{
Serial.println("STABLE POSITION");
stopmotors();
}
else if (buf[i]=='2')
{
Serial.println("REVERSE");
reverse();
}
else if (buf[i]=='4')
{
Serial.println("RIGHT");
slow_right();
}
else if (buf[i]=='5')
{
Serial.println("LEFT");
slow_left();
}
else if (buf[i]=='8')
{
Serial.println("AHEAD");
slow_ahead();
}
}
}
Serial.println("");
}
}
void loop()
{
bot_control();
}