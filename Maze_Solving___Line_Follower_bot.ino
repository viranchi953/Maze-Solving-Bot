//finals left hand rule

#define leftFarSensor      A0
#define leftNearSensor     A1
#define leftCenterSensor   A2
#define rightCenterSensor  A3
#define rightNearSensor    A4
#define rightFarSensor     A5


int leftCenterReading;
int leftNearReading;
int leftFarReading;
int rightCenterReading;
int rightNearReading;
int rightFarReading;

int leftNudge;
int replaystage;
int rightNudge;

#define leapTime 264

#define threshold 200

#define fspeed 255
#define tspeed 210
#define taspeed 200

#define leftMotor2  3 //In 1
#define leftMotor1  9//In 2
#define rightMotor2 10 //In 3
#define rightMotor1 11//In 4

#define led 13
#define buzz 2

char path[50] = {};
int pathLength;
int readLength;

void setup() {

  pinMode(leftCenterSensor, INPUT);
  pinMode(leftNearSensor, INPUT);
  pinMode(leftFarSensor, INPUT);
  pinMode(rightCenterSensor, INPUT);
  pinMode(rightNearSensor, INPUT);
  pinMode(rightFarSensor, INPUT);
  
  pinMode(buzz, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(115200);
  digitalWrite(led, HIGH);
  delay(1000);
}


void loop() {

  readSensors();

  if (leftFarReading > threshold && rightFarReading > threshold &&
      (leftCenterReading < threshold || rightCenterReading < threshold) ) {
    straight();
  }
  else {
    leftHandWall();
  }

}

void readSensors() {

  leftCenterReading  = analogRead(leftCenterSensor);
  leftNearReading    = analogRead(leftNearSensor);
  leftFarReading     = analogRead(leftFarSensor);
  rightCenterReading = analogRead(rightCenterSensor);
  rightNearReading   = analogRead(rightNearSensor);
  rightFarReading    = analogRead(rightFarSensor);

  // serial printing below for debugging purposes

  // Serial.print("leftCenterReading: ");
  // Serial.println(leftCenterReading);
  // Serial.print("leftNearReading: ");
  // Serial.println(leftNearReading);
  // Serial.print("leftFarReading: ");
  // Serial.println(leftFarReading);

  // Serial.print("rightCenterReading: ");
  // Serial.println(rightCenterReading);
  // Serial.print("rightNearReading: ");
  // Serial.println(rightNearReading);
  // Serial.print("rightFarReading: ");
  // Serial.println(rightFarReading);
  // delay(200);


}


void leftHandWall() {


  if ( leftFarReading < threshold && rightFarReading < threshold) {
    analogWrite(leftMotor1, fspeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, fspeed);
    analogWrite(rightMotor2, 0);
    delay(leapTime);
    readSensors();

    if (leftFarReading < threshold || rightFarReading < threshold) {
      done();
    }
    if (leftFarReading > threshold && rightFarReading > threshold) {
      turnLeft();
    }

  }

  if (leftFarReading < threshold) { // if you can turn left then turn left
    analogWrite(leftMotor1, fspeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, fspeed);
    analogWrite(rightMotor2, 0);
    delay(leapTime);
    readSensors();

    if (leftFarReading > threshold && rightFarReading > threshold) {
      turnLeft();
    }
    else {
      done();
    }
  }

  if (rightFarReading < threshold) {
    analogWrite(leftMotor1, fspeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, fspeed);
    analogWrite(rightMotor2, 0);
    delay(30);
    readSensors();

    if (leftFarReading < threshold) {
      delay(leapTime - 30);
      readSensors();

      if (rightFarReading < threshold && leftFarReading < threshold) {
        done();
      }
      else {
        turnLeft();
        return;
      }
    }
    delay(leapTime - 30);
    readSensors();
    if (leftFarReading > threshold && leftCenterReading > threshold &&
        rightCenterReading > threshold && rightFarReading > threshold) {
      turnRight();
      return;
    }
    path[pathLength] = 'S';
    // Serial.println("s");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
    if (path[pathLength - 2] == 'B') {
      //Serial.println("shortening path");
      shortPath();
    }
    straight();
  }
  readSensors();
  if (leftFarReading > threshold && leftCenterReading > threshold && rightCenterReading > threshold
      && rightFarReading > threshold && leftNearReading > threshold && rightNearReading > threshold) {
    turnAround();
  }

}
void done() {
  analogWrite(leftMotor1, 0);
  analogWrite(leftMotor2, 0);
  analogWrite(rightMotor1, 0);
  analogWrite(rightMotor2, 0);
  replaystage = 1;
  path[pathLength] = 'D';
  pathLength++;
  while (analogRead(leftFarSensor) < threshold) {
    digitalWrite(led, LOW);
    delay(150);
    digitalWrite(led, HIGH);
    delay(150);
  }
  delay(7000);
  digitalWrite(buzz, HIGH);
  delay(150);
  digitalWrite(buzz, LOW);
  replay();
}

void turnLeft() {



  while (analogRead(rightCenterSensor) < threshold || analogRead(leftCenterSensor) < threshold) {
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, tspeed);
    analogWrite(rightMotor1, tspeed);
    analogWrite(rightMotor2, 0);
    delay(2);
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(1);
  }

  while (analogRead(rightCenterSensor) > threshold) {
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, tspeed);
    analogWrite(rightMotor1, tspeed);
    analogWrite(rightMotor2, 0);
    delay(2);
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(1);
  }

  if (replaystage == 0) {
    path[pathLength] = 'L';
    //Serial.println("l");
    pathLength++;
    //Serial.print("Path length: ");
    //Serial.println(pathLength);
    if (path[pathLength - 2] == 'B') {
      //Serial.println("shortening path");
      shortPath();
    }
  }
}

void turnRight() {


  while (analogRead(rightCenterSensor) < threshold) {
    analogWrite(leftMotor1, tspeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, tspeed);
    delay(2);
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(1);
  }
  while (analogRead(rightCenterSensor) > threshold) {
    analogWrite(leftMotor1, tspeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, tspeed);
    delay(2);
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(1);
  }
  while (analogRead(leftCenterSensor) > threshold) {
    analogWrite(leftMotor1, tspeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, tspeed);
    delay(2);
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(1);
  }

  if (replaystage == 0) {
    path[pathLength] = 'R';
    Serial.println("r");
    pathLength++;
    Serial.print("Path length: ");
    Serial.println(pathLength);
    if (path[pathLength - 2] == 'B') {
      Serial.println("shortening path");
      shortPath();
    }
  }

}

void straight() {
  if ( analogRead(leftCenterSensor) > threshold) {
    analogWrite(leftMotor1, fspeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, fspeed);
    analogWrite(rightMotor2, 0);
    delay(1);
    analogWrite(leftMotor1, fspeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(5);
    return;
  }
  if (analogRead(rightCenterSensor) > threshold) {
    analogWrite(leftMotor1, fspeed);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, fspeed);
    analogWrite(rightMotor2, 0);
    delay(1);
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, fspeed);
    analogWrite(rightMotor2, 0);
    delay(5);
    return;
  }

  analogWrite(leftMotor1, fspeed);
  analogWrite(leftMotor2, 0);
  analogWrite(rightMotor1, fspeed);
  analogWrite(rightMotor2, 0);
  delay(4);
  analogWrite(leftMotor1, 0);
  analogWrite(leftMotor2, 0);
  analogWrite(rightMotor1, 0);
  analogWrite(rightMotor2, 0);
  delay(1);

}

void turnAround() {
  analogWrite(leftMotor1, fspeed);
  analogWrite(leftMotor2, 0);
  analogWrite(rightMotor1, fspeed);
  analogWrite(rightMotor2, 0);
  delay(150);
  while (analogRead(leftCenterSensor) > threshold) {
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, taspeed);
    analogWrite(rightMotor1, taspeed);
    analogWrite(rightMotor2, 0);
    delay(2);
    analogWrite(leftMotor1, 0);
    analogWrite(leftMotor2, 0);
    analogWrite(rightMotor1, 0);
    analogWrite(rightMotor2, 0);
    delay(1);
  }
  path[pathLength] = 'B';
  pathLength++;
  straight();
  //Serial.println("b");
  //Serial.print("Path length: ");
  //Serial.println(pathLength);
}

void shortPath() {
  int shortDone = 0;
  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'R') {
    pathLength -= 3;
    path[pathLength] = 'B';
    //Serial.println("test1");
    shortDone = 1;
  }

  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'S' && shortDone == 0) {
    pathLength -= 3;
    path[pathLength] = 'R';
    //Serial.println("test2");
    shortDone = 1;
  }

  if (path[pathLength - 3] == 'R' && path[pathLength - 1] == 'L' && shortDone == 0) {
    pathLength -= 3;
    path[pathLength] = 'B';
    //Serial.println("test3");
    shortDone = 1;
  }


  if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'L' && shortDone == 0) {
    pathLength -= 3;
    path[pathLength] = 'R';
    //Serial.println("test4");
    shortDone = 1;
  }

  if (path[pathLength - 3] == 'S' && path[pathLength - 1] == 'S' && shortDone == 0) {
    pathLength -= 3;
    path[pathLength] = 'B';
    //Serial.println("test5");
    shortDone = 1;
  }
  if (path[pathLength - 3] == 'L' && path[pathLength - 1] == 'L' && shortDone == 0) {
    pathLength -= 3;
    path[pathLength] = 'S';
    //Serial.println("test6");
    shortDone = 1;
  }

  path[pathLength + 1] = 'D';
  path[pathLength + 2] = 'D';
  pathLength++;
  //Serial.print("Path length: ");
  //Serial.println(pathLength);
  //printPath();
}




void printPath() {
  Serial.println("+++++++++++++++++");
  int x;
  while (x <= pathLength) {
    Serial.println(path[x]);
    x++;
  }
  Serial.println("+++++++++++++++++");
}


void replay() {
  readSensors();
  if (leftFarReading > threshold && rightFarReading > threshold) {
    straight();
  }
  else {
    if (path[readLength] == 'D') {
      analogWrite(leftMotor1, fspeed);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, fspeed);
      analogWrite(rightMotor2, 0);
      delay(100);
      analogWrite(leftMotor1, 0);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, 0);
      analogWrite(rightMotor2, 0);
      endMotion();
    }
    if (path[readLength] == 'L') {
      analogWrite(leftMotor1, fspeed);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, fspeed);
      analogWrite(rightMotor2, 0);
      delay(leapTime);
      turnLeft();
    }
    if (path[readLength] == 'R') {
      analogWrite(leftMotor1, fspeed);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, fspeed);
      analogWrite(rightMotor2, 0);
      delay(leapTime);
      turnRight();
    }
    if (path[readLength] == 'S') {
      analogWrite(leftMotor1, fspeed);
      analogWrite(leftMotor2, 0);
      analogWrite(rightMotor1, fspeed);
      analogWrite(rightMotor2, 0);
      delay(leapTime);
      straight();
    }

    readLength++;
  }

  replay();

}

void endMotion() {
  digitalWrite(led, LOW);
  delay(500);
  digitalWrite(led, HIGH);
  delay(200);
  digitalWrite(led, LOW);
  delay(200);
  digitalWrite(led, HIGH);
  delay(500);
  endMotion();
  delay(5000);
  }void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
