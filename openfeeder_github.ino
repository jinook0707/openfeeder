#include <AccelStepper.h>

const int MPIN1 = 4; // pin for SM42BYG011 stepper motor
const int MPIN2 = 5; // pin for SM42BYG011 stepper motor
const int MPIN3 = 6; // pin for SM42BYG011 stepper motor
const int PPIN1 = A0; // pin for piezo sensor (for detecting food drop on food dispense ramp)
const int fd_th = 15; // threshold of food dispense detector sensor
// to account it as a food fall

String s_msg;
int fd_state = 0; // state of food dispense detector sensor
unsigned int feed_spd = 240; // motor rotation speed when feeding
unsigned int feed_accel = 240; // motor acceleration speed when feeding

void setup() {
  Serial.begin(9600);
  pinMode(MPIN3, OUTPUT); // Enable
  digitalWrite(MPIN3,HIGH);
  pinMode(PPIN1, INPUT); // piezo sensor
  unsigned long fd_state_ = 0;
}

void loop() {
  if (Serial.available()>0) {
    s_msg = "";
    while (Serial.available()) {
      delay(10);
      char tmp = Serial.read();
      s_msg += tmp;
    }
    s_msg.replace(" ",""); // get rid of a blank
    Serial.println(s_msg);
    if (s_msg=="feed") feed();
    Serial.flush();
  }
}

void feed() {
  int num_feed = 1; // number of food dispenses to do
  for (int i=0; i<num_feed; i++) { // feed once (or multiple times)
    for (int j=0; j<3; j++) { // try 3 rotations
      bool ret = run_motor(50,feed_spd,feed_accel,false);
      Serial.println("motor rotated");
      if (ret == true) { // piezo was hit (successful food dispense)
        num_feed -= 1;
        break;
      }
    }
    delay(1000);
  }
  if (num_feed > 0) { // disk is probably stuck
    // move disk back and forth 3 times
    for (int i=0; i<3; i++) {
      bool ret = run_motor(-50,240,720,true); // rotate disk backward
      delay(100);
      ret = run_motor(100,240,720,true); // rotate disk forward further than backward distance
      delay(100);
    }
  }
}

bool run_motor(int dest, int spd, int accel, bool is_stuck) {
  bool fd = false; // whether food dispense was detected
  AccelStepper stepper(1,MPIN2,MPIN1);//initialise accelstepper for a two wire board, MPIN2 step, MPIN1 dir
  digitalWrite(MPIN3,LOW); // Set Enable low
  stepper.setMaxSpeed(spd);
  stepper.setAcceleration(accel);
  stepper.move(dest);
  if (dest > 0) {
    while ( stepper.distanceToGo() > 0 ) {
      stepper.run();
      if (is_stuck == false) { // if the disk is stuck, run without sensor checking
        fd = chk_fd_sensor();
        if (fd == true) break;
      }
    }
  }
  else if (dest < 0) {
    while ( stepper.distanceToGo() < 0 ) {
      stepper.run();
      fd = chk_fd_sensor();
      if (fd == true) break;
    }
  }
  
  // rotate slightly further to finish a whole well.
  stepper.move(5);
  while ( stepper.distanceToGo() > 0 ) stepper.run();
  digitalWrite(MPIN3,HIGH);
  return fd;
}

bool chk_fd_sensor() {
  for (int i=0;i<5;i++) { // read sensor several times
    // check food dispense (hitting piezo sensor)
    fd_state = int(analogRead(PPIN1));
    if (fd_state > 2) Serial.println(fd_state);
    if (fd_state > fd_th) {
      Serial.println(fd_state);
      return true;
    }
    delay(1);
  }
  return false;
}

