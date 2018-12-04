// motor
const int MPINENA = 10;
const int MPIN1 = 9;
const int MPIN2 = 8;
const int PPIN1 = A4; // pin for piezo sensor (for detecting food drop on food dispense ramp)

String s_msg;
int fd_state = 0; // state of food dispense detector sensor
const int fd_th = 150; // threshold of food dispense detector sensor
// to account it as a food fall

unsigned long timestamp;
unsigned long elapsed_ms;

void setup() {
  timestamp = millis();
  
  pinMode(MPINENA, OUTPUT);
  digitalWrite(MPINENA, LOW);
  pinMode(MPIN1, OUTPUT);
  pinMode(MPIN2, OUTPUT);
  pinMode(PPIN1, INPUT); // piezo sensor
  Serial.begin(57600);
}

void loop() {
  if (stop_session == true) {
    delay(1000);
    return;
  }
  
  if (Serial.available()>0) {
    while (Serial.available()) {
      delay(2);
      char tmp = Serial.read();
      s_msg += tmp;
    }
    s_msg.replace(" ",""); // get rid of a blank
    Serial.println(s_msg);
    if (s_msg=="feed") feed();
    s_msg = "";
    Serial.flush();
  }
}

void feed() {
  int num_disp = 1; // number to dispense
  int cnt_to_disp = 3; // counter to check how many times it dispensed
  for (int i=0; i<num_disp; i++) {
    bool ret = false;
    for (int j=0; j<3; j++) {
      ret = run_motor(1200, 100, true);
      //Serial.println("motor rotated");
      if (ret == true) { // piezo was hit (successful food dispense)
        cnt_to_disp -= 1;
        break;
      }
      delay(10);
    }
    if (ret == false) { // piezo was not hit. maybe disk is stuck.
      ret = run_motor(200, 255, false); // move backward
      delay(250);      
      ret = run_motor(250, 200, true); // move forward
      Serial.println("motor rotated back and forth");
      if (ret == true) cnt_to_disp -= 1;
    }
    delay(1000);
  }
  /*
  if (cnt_to_disp == num_disp) { // failed to dispense not even once. disk is probably stuck
      stop_session = true; // stop whole session
      Serial.println("stop session");
  }
  */
}

bool run_motor(int dur, int spd, bool dir) {
  bool fd = false; // whether food dispense was detected
  // turn on motor
  if (dir == true) { // forward
    Serial.println("F");
    digitalWrite(MPIN1, HIGH);
    digitalWrite(MPIN2, LOW);
  }
  else if (dir == false) { // backward
    Serial.println("B");
    digitalWrite(MPIN1, LOW);
    digitalWrite(MPIN2, HIGH);
  }
  delay(10);
  // set speed to 255 out of possible range 0~255
  analogWrite(MPINENA, spd);
  //Serial.println(dur);
  fd = chk_fd_sensor(dur); // running the motor for 'dur' parameter or until food piece hits piezo sensor
  //delay(20); // rotate slightly further to finish a whole well.
  // turn off motors
  digitalWrite(MPINENA, LOW);
  digitalWrite(MPIN1, LOW);
  digitalWrite(MPIN2, LOW);  
  return fd;
}

bool chk_fd_sensor(int dur) {
  unsigned long start_time = millis();
  analogRead(PPIN1);
  // check food dispense (hitting piezo sensor)
  while (millis()-start_time < dur) {
    fd_state = int(analogRead(PPIN1));
    //if (fd_state>30) Serial.println(fd_state);
    if (fd_state > fd_th) return true;
  }
  return false;
}



