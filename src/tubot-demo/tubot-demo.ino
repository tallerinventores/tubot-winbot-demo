//TuBot

#include <Servo.h> 
#include <NewPing.h>

//floor color definition
#define white 724
#define black 300
//motor definitions
#define halt 0
#define forward 1
#define reverse 2
#define turnleft 3
#define turnright 4
#define left 1
#define right 2
#define both 3
#define stopright 70
#define forwardright 45
#define reverseright 95
#define stopleft 75
#define forwardleft 95
#define reverseleft 55

NewPing  sonar(4, 5, 300);
Servo    servoright;
Servo    servoleft;

void setup() {
  Serial.begin(115200);
  servoright.attach(10);
  servoleft.attach(9);
  servoright.write(stopright);
  servoleft.write(stopleft);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
//  do {
//    Serial.println(FindOpponent());
//    delay(100);
//  } while (1);
  for(int i=0;i<25;i++) {
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    delay(100);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    delay(100);
  }
}

void loop() {
  if (!GetPositionStatus()) {
    if (Search()) Attack();
  }
  else SurviveOnTatami();
}

boolean GetPositionStatus() {
  boolean GPS_bStepOnBorder = false;
  
  if (!ReadLeftSensor(black,white)) GPS_bStepOnBorder = true;
  if (!ReadRightSensor(black,white)) GPS_bStepOnBorder = true;
  return GPS_bStepOnBorder;
}

void SurviveOnTatami() {
  boolean SOT_bLeft = false;
  boolean SOT_bRight = false;
  
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  SOT_bLeft = ReadLeftSensor(black, white);
  SOT_bRight = ReadRightSensor(black, white);
  if (SOT_bLeft == false && SOT_bRight == false) {
    Move(reverse, both, 100,1000);
    Move(reverse, left, 100,1000);
  }
  else {
    if (SOT_bLeft == false) Move(forward, left, 50,150);
    if (SOT_bRight == false) Move(forward, right, 50,150);
  }
  digitalWrite(8, LOW);
}

boolean Search() {
  boolean S_bStatus = false;
  static int S_iLastStage = 0;
  
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  if (FindOpponent()) S_bStatus = true;
  else {
    if (S_iLastStage == 0) {
      S_iLastStage = 1;
      Move(turnleft, both, 66,500);
    }
    else if (S_iLastStage == 1) {
      S_iLastStage = 2;
      Move(turnright, both, 70,500);
    }
    else if (S_iLastStage == 2) {
      S_iLastStage = 3;
      Move(turnright, both, 70,500);
    }
    else if (S_iLastStage == 3) {
      S_iLastStage = 0;
      Move(turnleft, both, 66,500);
    }
  }
  digitalWrite(7, LOW);
  return S_bStatus;
}

void Attack() {
  digitalWrite(6, HIGH);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  Move(forward, both, 80,1000);
  digitalWrite(6, LOW);
}

boolean ReadLeftSensor(int BlackThreshold, int WhiteThreshold) {
  int RLS_iLeft = 0;
  boolean RLS_bLeft = false;
  
  RLS_iLeft = analogRead(A1);
  if (RLS_iLeft < BlackThreshold) RLS_bLeft = false;
  else if (RLS_iLeft > WhiteThreshold) RLS_bLeft = true;
  //else RLS_bLeft = ReadLeftSensor(black, white);
  return RLS_bLeft;
}

boolean ReadRightSensor(int BlackThreshold, int WhiteThreshold) {
  int RRS_iRight = 0;
  boolean RRS_bRight = false;
  
  RRS_iRight = analogRead(A0);
  if (RRS_iRight < BlackThreshold) RRS_bRight = false;
  else if (RRS_iRight > WhiteThreshold) RRS_bRight = true;
  //else RRS_bRight = ReadRightSensor(black, white);
  return RRS_bRight;
}

void Move(int course, int motor, int percent, int distance) {
  int M_iSpeedLeft = 0;
  int M_iSpeedRight = 0;
  
  switch (motor){
    case left:
      if (course == halt) {
        servoleft.write(stopleft);
      }
      else if (course == forward) {
        M_iSpeedLeft = map(percent,0,100,stopleft,forwardleft);
        servoleft.write(M_iSpeedLeft);
        delay(distance);
        servoleft.write(stopleft);
      }
      else if (course == reverse) {
        M_iSpeedLeft = map(percent,0,100,stopleft,reverseleft);
        servoleft.write(M_iSpeedLeft);
        delay(distance);
        servoleft.write(stopleft);
      }
      break;
    case right:
      if (course == halt) {
        servoright.write(stopright);
      }
      else if (course == forward) {
        M_iSpeedRight = map(percent,0,100,stopright,forwardright);
        servoright.write(M_iSpeedRight);
        delay(distance);
        servoright.write(stopright);
      }
      else if (course == reverse) {
        M_iSpeedRight = map(percent,0,100,stopright,reverseright);
        servoright.write(M_iSpeedRight);
        delay(distance);
        servoright.write(stopright);
      }
      break;
    case both:
      if (course == halt) {
        servoleft.write(stopleft);
        servoright.write(stopright);
      }
      else if (course == forward) {
        M_iSpeedLeft = map(percent,0,100,stopleft,forwardleft);
        M_iSpeedRight = map(percent,0,100,stopright,forwardright);
        servoleft.write(M_iSpeedLeft);
        servoright.write(M_iSpeedRight);
        delay(distance);
        servoleft.write(stopleft);
        servoright.write(stopright);
      }
      else if (course == reverse) {
        M_iSpeedLeft = map(percent,0,100,stopleft,reverseleft);
        M_iSpeedRight = map(percent,0,100,stopright,reverseright);
        servoleft.write(M_iSpeedLeft);
        servoright.write(M_iSpeedRight);
        delay(distance);
        servoleft.write(stopleft);
        servoright.write(stopright);
      }
      else if (course == turnleft) {
        M_iSpeedLeft = map(percent,0,100,stopleft,reverseleft);
        M_iSpeedRight = map(percent,0,100,stopright,forwardright);
        servoleft.write(M_iSpeedLeft);
        servoright.write(M_iSpeedRight);
        delay(distance);
        servoleft.write(stopleft);
        servoright.write(stopright);
      }
      else if (course == turnright) {
        M_iSpeedLeft = map(percent,0,100,stopleft,forwardleft);
        M_iSpeedRight = map(percent,0,100,stopright,reverseright);
        servoleft.write(M_iSpeedLeft);
        servoright.write(M_iSpeedRight);
        delay(distance);
        servoleft.write(stopleft);
        servoright.write(stopright);
      }
  }
}

boolean FindOpponent() {
  boolean FO_bTarget = false;
  int FO_iDistance = 0;
  
  FO_iDistance = sonar.ping_cm();
  if ((FO_iDistance <= 50) && (FO_iDistance >= 3)) FO_bTarget = true;
  return FO_bTarget;
}
