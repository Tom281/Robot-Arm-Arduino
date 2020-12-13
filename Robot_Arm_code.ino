#include <IRremote.h>
#include <Servo.h>

//Servos
Servo servo_swivel;
Servo servo_height;
Servo servo_clamp;
Servo servo_reach;
//IR reciever
const int RECV_PIN = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;
//Most recent remote button pressed:
long btnPressed;
//Servo that is currently being adjusted
Servo servo_selected;

void setup() {
    //servo attachments
    servo_swivel.attach(10);
    servo_height.attach(9);
    servo_clamp.attach(11);
    servo_reach.attach(6);
    //set selected servo
    servo_selected = servo_swivel;
    //Set arm to default position
    posDefault();
    //IR remote
    irrecv.enableIRIn();
    irrecv.blink13(true);
}


void loop() {
  //interpret signals sent from remote and take corresponding action.
  if(irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if(!(results.value == 0xFFFFFFFF)) {
      btnPressed = results.value;
    }
    switch(btnPressed) {
      // |<<
      case 0xFF22DD:
        servo_selected = servo_swivel;
        break;
      // >>|
      case 0xFF02FD:
        servo_selected = servo_height;
        break;
      // >||
      case 0xFFC23D:
        servo_selected = servo_clamp;
        break;
      // EQ
      case 0xFF906F:
        servo_selected = servo_reach;
        break;
      // -
      case 0xFFE01F:
        servo_selected.write((servo_selected.read() - 5 < 0)? 0 : servo_selected.read() - 5);
        break;
      // +
      case 0xFFA857:
        servo_selected.write((servo_selected.read() + 5 > 180)? 180 : servo_selected.read() + 5);
        break;
      // 0
      case 0xFF6897:
        posDefault();
        break;
      // 1
      case 0xFF30CF:
        extendArm();
        break;
      // 2
      case 0xFF18E7:
        retractArm();
        break;
      // 4
      case 0xFF10EF:
        clamp();
        break;
      // 5
      case 0xFF38C7:
        unClamp();
        break;
      // 7
      case 0xFF42BD:
        faceLeft();
        break;
      // 8
      case 0xFF4AB5:
        faceCenter();
        break;
      // 9
      case 0xFF52AD:
        faceRight();
        break;
    }
    irrecv.resume();
  }
}
//button 0
void posDefault() {
  servo_swivel.write(90);
  servo_height.write(20);
  servo_clamp.write(45);
  servo_reach.write(90);
}
//button 1
void extendArm() {
  servo_height.write(90);
  delay(100);
  servo_reach.write(160);
}
//button 2
void retractArm() {
  servo_reach.write(90);
  delay(100);
  servo_height.write(10);
}
//button 3
void faceLeft() {
  servo_swivel.write(180);
}
//button 4
void faceCenter() {
  servo_swivel.write(90);
}
//button 5
void faceRight() {
  servo_swivel.write(0);
}
//button 6
void clamp() {
  servo_clamp.write(120);
}
//button 7
void unClamp() {
  servo_clamp.write(45);
}
