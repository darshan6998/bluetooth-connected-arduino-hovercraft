#include<Servo.h>
#include<SoftwareSerial.h>

boolean highSpeed = true;

//BLDCs
int bldc_pins[2]={5,9};
int pulseTime[2] = {1000,1000},currentTime[2]={0,0};
int timeP[] = {1000,1100,1200,1300,1400,1500,1600,1700,1800};
byte ip[4]={0};

int msb=0,lsb=0,data=0;

Servo bldc1,bldc2;

//Servo
Servo myServo;
int servoPos=0;

int pinServo = 3;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(10);

  bldc1.attach(bldc_pins[0]);
  bldc2.attach(bldc_pins[1]);

  myServo.attach(pinServo);
  myServo.write(90);
  delay(15);
  Serial.println("Enter 0-9 to adjust motor speed.");
  Serial.println("Enter h or l to change the speed range.");

}

void loop() {
  digitalWrite(bldc_pins[0], HIGH); 
  delayMicroseconds(pulseTime[0]);  
  digitalWrite(bldc_pins[0], LOW);

  
  digitalWrite(bldc_pins[1], HIGH);
  delayMicroseconds(pulseTime[1]);
  digitalWrite(bldc_pins[1], LOW); 
  
  //delay(10.1-(pulseTime[0]/1000));

  
  // put your main code here, to run repeatedly:
  int servoFlag = 0;
  ip[0] = -1;
  ip[1] = -1;
  ip[2] = -1;
  ip[3] = -1;
  
  
  if(Serial.available() > 0){
     Serial.readBytes(ip,4);
     if(ip[0]=='S' || ip[0]=='s'){
        servoFlag=1;
        servoPos = 0;
        for(int i=1;i<4;i++){
          if(ip[i]==255)break;
          servoPos = servoPos*10 + (ip[i] - '0');
        }
     }else{
        msb = ip[0] - '0';
        lsb = ip[1] - '0';
     }
  }

  
  if(servoFlag==1){
    Serial.print("Servo : ");
    Serial.println(servoPos);
    
    
    int prevPulse = currentTime[0];
    controlBldc(1,0);
    
    myServo.write(servoPos);
    delay(15);
    
  }else if(msb!=-1){
    switch(msb){
      case 0:
        
        controlBldc(1,0);
        controlBldc(2,0);
        
        break;
      case 1:
      case 2:
        controlBldc(msb,lsb);
        break;
    }
  }
  msb = -1;
  lsb=-1;
}

void controlBldc(int pin,int s){
  pin = pin-1;
  
  setPulseTime(pin,timeP[s]);
  currentTime[pin] = s;
  
  Serial.print("Throttle for BLDC#");
  Serial.print(pin);
  Serial.print(" : ");
  Serial.print((pulseTime[pin]-1000)/8);
  Serial.println("%");
}

void setPulseTime(int pin,int timeP){
 if(highSpeed){
    pulseTime[pin] = timeP;
    
  }else {
    pulseTime[pin] = 930 + (timeP/10);
  }
}

