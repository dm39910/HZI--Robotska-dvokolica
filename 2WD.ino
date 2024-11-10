#include <HCSR04.h>
#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

HCSR04 hc(14, 15); //initialisation class HCSR04 (trig pin , echo pin)

float distance=0;

const int pinIRd1 = 16;
const int pinIRd2 = 17;
const int pinS1 = 18;
const int pinS2 =19;


int IR1 = 0;
int IR2 = 0;
int S1=0;
int S2=0;
int deg90 = 500;
int Mspeed=120;

void setup()
{
    Serial.begin(9600);
    motor1.setSpeed(Mspeed);
    motor1.run(RELEASE);
    motor2.setSpeed(Mspeed);
    motor2.run(RELEASE);
    pinMode(pinIRd1,INPUT);
    pinMode(pinIRd2,INPUT);
    pinMode(pinS1,INPUT);
    pinMode(pinS2,INPUT);
}

void loop()
{
  S1 = digitalRead(pinS1);
  S2 = digitalRead(pinS2);
  if((S1==1)&&(S2==1)){
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
  }
  if((S1==1)&&(S2==0)){
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    delay(1500);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    delay(200);
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    delay(deg90);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    delay(200);
  }  
  if((S1==0)&&(S2==1)){
    distance=hc.dist();
    IR1 = digitalRead(pinIRd1);
    IR2 = digitalRead(pinIRd2);
    Serial.print(distance); // return curent distance in serial
    Serial.print("\t IR1=");
    Serial.print(IR1);
    Serial.print("\t IR2=");
    Serial.println(IR2);
    delay(60);   
  }
  if((S1==0)&&(S2==0)){
    distance=hc.dist();
    IR1 = digitalRead(pinIRd1);
    IR2 = digitalRead(pinIRd2);
    Serial.print(distance); // return curent distance in serial
    Serial.print("\t IR1=");
    Serial.print(IR1);
    Serial.print("\t IR2=");
    Serial.println(IR2);
    delay(60);                 // we suggest to use over 60ms measurement cycle, in order to prevent trigger signal to the echo signal.
    
    if((distance>10.0)||(distance==0.00)){
      
      if((IR1==1)&&(IR2==1)){ //Both sensors inside black line, drive forward
        motor1.run(FORWARD);
        motor2.run(FORWARD);
      }

      if((IR1==1)&&(IR2==0)){   //Right sensor out of line, turn left
        motor1.run(RELEASE);
        motor2.run(FORWARD);
      }

      if((IR1==0)&&(IR2==1)){   //Left sensor out of line, turn right
        motor1.run(FORWARD);
        motor2.run(RELEASE);
      }

      if((IR1==0)&&(IR2==0)){   //Both sensors out of line, drive backwards
        motor1.run(BACKWARD);
        motor2.run(BACKWARD);  
      }
      
    }
    else{
      motor1.run(RELEASE);
      motor2.run(RELEASE);
    } 
  }
}
