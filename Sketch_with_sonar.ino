
#include <Servo.h>
#include <NewPing.h>

const int Left_Motor_Forward = 4;
const int Left_Motor_Backward = 5;
const int Right_Motor_Forward = 6;
const int Right_Motor_Backward = 7;

const int green_LED = 32;
const int red_LED = 34;
const int blue_LED = 36;
const int yellow_LED = 38;


#define Trigger A1
#define Echo A2

Servo servo_motor1;
Servo servo_motor2;
NewPing sonar(Trigger, Echo, 400);

int distance = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(Left_Motor_Forward, OUTPUT);
  pinMode(Left_Motor_Backward, OUTPUT);
  pinMode(Right_Motor_Forward, OUTPUT);
  pinMode(Right_Motor_Backward, OUTPUT);

  pinMode(green_LED, OUTPUT);
  pinMode(red_LED, OUTPUT);
  pinMode(blue_LED, OUTPUT);
  pinMode(yellow_LED, OUTPUT);

  servo_motor1.attach(8);
//  servo_motor2.attach(9);

  servo_motor1.write(90);
}

void loop() 
{
  distance = getDistance();
  
  int distance_Left = 0;
  int distance_Right = 0;

  if(distance <= 45)
  {
//    Serial.println("If statement entered");
    stopCar();
    delay(250);
    backward();
    delay(200);
    stopCar();
    delay(250);
    distance_Left = find_left_distance();
    delay(300);
    distance_Right = find_right_distance();
    if(distance_Right >= distance_Left)
    {
      turnRight();
      digitalWrite(blue_LED, HIGH);
      delay(150);
      digitalWrite(blue_LED, LOW);
      stopCar();
    }
    else 
    {
      turnLeft();
      digitalWrite(blue_LED, HIGH);
      delay(150);
      digitalWrite(blue_LED, LOW);
      stopCar();
    }

  }

  else 
  {
    forward();
  }
}

int getDistance() 
{  
  int temp_distance = sonar.ping_cm();
  if(temp_distance == 0) 
  {
    
    temp_distance = 400;
    
  }
  Serial.print("Distance: ");
  Serial.println(temp_distance);
  return temp_distance;
}

int find_left_distance() 
{
  
  servo_motor1.write(180);
  delay(100);
  int tempLeftDistance = getDistance();
  delay(100);
  servo_motor1.write(90);
  Serial.print("Left_distance: ");
  Serial.println(tempLeftDistance);
  return tempLeftDistance;
}

int find_right_distance() 
{
  servo_motor1.write(0);
  delay(100);
  int tempRightDistance = getDistance();
  delay(100);
  servo_motor1.write(90);
  Serial.print("Right_distance entered: ");
  Serial.println(tempRightDistance);
  return tempRightDistance;
}

void blinkLED(const int pin)
{
  digitalWrite(pin, HIGH);
  delay(500);
  digitalWrite(pin, LOW);
  delay(500);
}

void forward() 
{
  digitalWrite(Left_Motor_Forward, HIGH);
  digitalWrite(Left_Motor_Backward, LOW);
  digitalWrite(Right_Motor_Forward, HIGH);
  digitalWrite(Right_Motor_Backward, LOW);
  blinkLED(green_LED);
}

void backward() 
{
  digitalWrite(Left_Motor_Forward, LOW);
  digitalWrite(Left_Motor_Backward, HIGH);
  digitalWrite(Right_Motor_Forward, LOW);
  digitalWrite(Right_Motor_Backward, HIGH);
  blinkLED(yellow_LED);
}

void turnLeft() 
{
  digitalWrite(Left_Motor_Forward, HIGH);
  digitalWrite(Left_Motor_Backward, LOW);
  digitalWrite(Right_Motor_Forward, LOW);
  digitalWrite(Right_Motor_Backward, HIGH);
}

void turnRight() 
{
  digitalWrite(Left_Motor_Forward, LOW);
  digitalWrite(Left_Motor_Backward, HIGH);
  digitalWrite(Right_Motor_Forward, HIGH);
  digitalWrite(Right_Motor_Backward, LOW);
}

void stopCar() 
{
  digitalWrite(Left_Motor_Forward, LOW);
  digitalWrite(Left_Motor_Backward, LOW);
  digitalWrite(Right_Motor_Forward, LOW);
  digitalWrite(Right_Motor_Backward, LOW);
  blinkLED(red_LED);
}
