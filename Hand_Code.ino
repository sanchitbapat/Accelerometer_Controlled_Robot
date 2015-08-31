#include <VirtualWire.h>
const int xul=250; //upper limit for x-axis
const int xll=230; //lower limit for x-axis
const int yul=250; //upper limit for y-axis
const int yll=230; //lower limit for y-axis
const int xpin = A3; //x-axis of the accelerometer
const int ypin = A2; //y-axis of the accelerometer
const int LEDPIN = 13;
const int butpin=9; //Button Pin
int x;
int y;
void setup()
{
pinMode(LEDPIN, OUTPUT); //initialize pins as ip/op
pinMode(butpin, INPUT);
Serial.begin(9600);
vw_setup(2000); //Start the transmitter PLL running
}
void sensor_values()
{
x= analogRead(xpin); //read and store the values of tilt from the accelerometer
Serial.print(x);
Serial.print("\t");
y= analogRead(ypin);
Serial.print(y);
Serial.println();
}
void auto_control()
{
digitalWrite(LEDPIN,HIGH);
const char *msg = "9";
vw_send((uint8_t *)msg, strlen(msg)); //send message for automatic control
Serial.println(msg);
delay(50);
digitalWrite(LEDPIN,LOW);
}
void bot_control()
{
const char *msg = "7";
vw_send((uint8_t *)msg, strlen(msg));
Serial.println(msg);
delay(50);
digitalWrite( LEDPIN , LOW );
if ((x<xul && x>xll) && (y<yul && y>yll)) //condition to stop the motors
{
digitalWrite( LEDPIN , LOW );
const char *msg = "1";
vw_send((uint8_t *)msg, strlen(msg));
delay(50);
}
else if((x<xll)) //condition for left turn
{
digitalWrite( LEDPIN , HIGH );
const char *msg = "4";
vw_send((uint8_t *)msg, strlen(msg));
delay(50);
}
else if((x>xul)) //condition for right turn
{
digitalWrite( LEDPIN , HIGH );
const char *msg = "5";
vw_send((uint8_t *)msg, strlen(msg));
delay(50);
}
else if((y>yul)) //condition for moving forward
{
digitalWrite( LEDPIN , HIGH );
const char *msg = "2";
vw_send((uint8_t *)msg, strlen(msg));
delay(50);
}
else if((y<yll)&&(y>180)) //condition for moving reverse
{
digitalWrite( LEDPIN , HIGH );
const char *msg = "8";
vw_send((uint8_t *)msg, strlen(msg));
delay(50);
}
}
void loop()
{
if(digitalRead(butpin)) //read the value of the button
{
sensor_values();
bot_control(); //manual/user control
}
else
{
auto_control(); //automatic control
}
delay(10);
}