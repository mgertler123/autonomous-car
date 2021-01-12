/* Disclaimer: this program contains code from the TFMini Product Manual to assist in the interpretation routine of standard output protocol of TFmini product on Arduino.
For details, refer to Product Specifications.
*/
#include <Servo.h>
#include<SoftwareSerial.h>
SoftwareSerial Serial1(2,3); //define software serial port name as Serial1 and define pin2 as RX and pin3 as TX

int dist; //distance variable measured from TFMini
int strength;//signal strength from TFMini
int check;//check value
int uart[9];//save data measured by LiDAR
const int HEADER=0x59;//frame header of data package

int distance_Left = 0;
int distance_Right = 0;

const int Left_Motor_Forward = 4;
const int Left_Motor_Backward = 5;
const int Right_Motor_Forward = 6;
const int Right_Motor_Backward = 7;

const int green_LED = 11;
const int red_LED = 12;
const int yellow_LED = 13;

bool left = false;
bool right = false;

int count = 0;

Servo servo_motor1;
Servo servo_motor2;

void setup()
{
  Serial.begin(9600); //set bit rate of serial port connecting computer with Arduino
  Serial1.begin(115200); //set bit rate of serial port connecting LiDAR with Arduino
  
  pinMode(Left_Motor_Forward, OUTPUT);
  pinMode(Left_Motor_Backward, OUTPUT);
  pinMode(Right_Motor_Forward, OUTPUT);
  pinMode(Right_Motor_Backward, OUTPUT);

  pinMode(red_LED, OUTPUT);
  pinMode(yellow_LED, OUTPUT);
  pinMode(green_LED, OUTPUT);

  digitalWrite(green_LED, HIGH); //Signal that car is on and connected

  servo_motor1.attach(8);
//  servo_motor2.attach(9);

  servo_motor1.write(90);
}

void loop() {

  if(distance_Left != 0 && distance_Right != 0)
  {
    if(distance_Right >= distance_Left)
    {
      turnRight();
      delay(250);
      stopCar();
    }
    else 
    {
      turnLeft();
      delay(250);
      stopCar();
    }
    
    distance_Left = 0;
    distance_Right = 0;

  }
  else 
  {
    if (Serial1.available())
    {
      if(Serial1.read()==HEADER)
      {
        uart[0]=HEADER;
        if(Serial1.read()==HEADER)
        {
          uart[1]=HEADER;
          for(int i = 2;i < 9; i++)
          {
            uart[i]=Serial1.read();
          }
          check=uart[0]+uart[1]+uart[2]+uart[3]+uart[4]+uart[5]+uart[6]+uart[7];
          if(uart[8]==(check&0xff))
          {
            dist=uart[2]+uart[3]*256;
            strength=uart[4]+uart[5]*256;
            if(dist == 0){
              dist = 250;
            }
            Serial.print("dist = ");
            Serial.print(dist);
            Serial.print('\t');
            Serial.print("strength = ");
            Serial.print(strength);
            Serial.print('\n');
  
            if(dist <= 45 && left == false && right == false) 
            {
              begin_processing();
              look_left();
              ++count;
            }
  
            else if(left == true && count == 20) 
            {
              distance_Left = dist;
              Serial.print("Left Distance = ");
              Serial.println(distance_Left);
              servo_motor1.write(90);
              left = false;
              look_right();
              count = 1;
            }
  
            else if(right == true && count == 20)
            {
              distance_Right = dist;
              Serial.print("Right Distance = ");
              Serial.println(distance_Right);
              servo_motor1.write(90);
              right = false;
              count = 0;
            }

            else if(left == true || right == true && count != 0) 
            {
              ++count;
            }
            
            else
            {
              forward();
            }
          }
        }
      }
    }
  }

}


void begin_processing() //Stops car and prepares for analysis on further movement
{  
    stopCar();
    delay(250);
    backward();
    delay(10);
    stopCar();
    delay(250);
}

void look_left() //Function call to rotate servo motor to have TFMini sensor look left
{
  
  servo_motor1.write(180);
  delay(500);
  left = true;
}

void look_right() //Function call to rotate servo motor to have TFMini sensor look right
{
  
  servo_motor1.write(0);
  delay(500);
  right = true;
}

void blinkLED(const int pin) //Appropriate LED behavior when blinking
{
  digitalWrite(pin, HIGH);
  delay(350);
  digitalWrite(pin, LOW);
  delay(350);
}

void forward() //Move car forward
{
  digitalWrite(Left_Motor_Forward, HIGH);
  digitalWrite(Left_Motor_Backward, LOW);
  digitalWrite(Right_Motor_Forward, HIGH);
  digitalWrite(Right_Motor_Backward, LOW);
  Serial.println("Forward");
//  blinkLED(green_LED);
}

void backward() //Move car backward and activate yellow LEDs
{
  digitalWrite(Left_Motor_Forward, LOW);
  digitalWrite(Left_Motor_Backward, HIGH);
  digitalWrite(Right_Motor_Forward, LOW);
  digitalWrite(Right_Motor_Backward, HIGH);
  Serial.println("Backward");
  blinkLED(yellow_LED);
}

void turnLeft() //Turn car left
{
  digitalWrite(Left_Motor_Forward, HIGH);
  digitalWrite(Left_Motor_Backward, LOW);
  digitalWrite(Right_Motor_Forward, LOW);
  digitalWrite(Right_Motor_Backward, HIGH);
  Serial.println("Turning Left");
}

void turnRight() //Turn car right
{
  digitalWrite(Left_Motor_Forward, LOW);
  digitalWrite(Left_Motor_Backward, HIGH);
  digitalWrite(Right_Motor_Forward, HIGH);
  digitalWrite(Right_Motor_Backward, LOW);
  Serial.println("Turning Right");
}

void stopCar() //Stop car motion and activat red LEDs
{
  digitalWrite(Left_Motor_Forward, LOW);
  digitalWrite(Left_Motor_Backward, LOW);
  digitalWrite(Right_Motor_Forward, LOW);
  digitalWrite(Right_Motor_Backward, LOW);
  blinkLED(red_LED);
  Serial.println("Stopping");
}
