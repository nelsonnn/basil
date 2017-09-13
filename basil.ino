#define SEARCH 1
#define FOLLOW 2
#define STOP 3
#define ALIGN 4
#include <Sparki.h> // include the sparki library

void setup()
{
  sparki.servo(SERVO_CENTER);
  sparki.gripperOpen();
  delay(5000);
  sparki.gripperStop();
}

int state = SEARCH;
void loop() {
  int threshold  = 500; //For line sensors
  bool edgeLeft   = sparki.edgeLeft() < threshold;   // measure the left edge IR sensor
  bool edgeRight  = sparki.edgeRight() < threshold;  // measure the right edge IR sensor
  bool lineLeft   = sparki.lineLeft() < threshold;   // measure the left IR sensor
  bool lineCenter = sparki.lineCenter() < threshold; // measure the center IR sensor
  bool lineRight  = sparki.lineRight() < threshold;  // measure the right IR sensor
  bool cm = sparki.ping(); // measures the distance with Sparki's eyes

  //LCD stuff
  sparki.clearLCD();
  if (state == 1) sparki.print("SEARCH");
  else if (state == 2) sparki.print("FOLLOW");
  else if (state == 3) sparki.print("STOP");
  else if (state == 4) sparki.print("ALIGN");
  else sparki.print("ERROR");
  sparki.updateLCD();

  switch (state) {
    case SEARCH: {
        if (cm != -1) { // not too close or too far
          if (cm < 20) { // if the distance measured is less than 20 centimeters
            sparki.moveRight(10);
            delay(500);
            sparki.RGB(RGB_RED); // target aquired
            sparki.clearLCD();
            sparki.print("TARGET AQUIRED");
            sparki.updateLCD();
            sparki.beep(); // beep!
            sparki.moveForward(cm + 1); // move to the object
            delay(2500);
            sparki.gripperClose();
            delay(5000);
            sparki.gripperStop();
            delay(100);
            sparki.moveRight(180);
            state = ALIGN;
          } else sparki.moveRight();
        }
        break;
      }

    case ALIGN: {
        if (lineLeft  && lineRight) {
          sparki.moveForward(3);
          sparki.moveRight(90);
          delay(100);
          state = FOLLOW;
        }
 
        
        else if (lineRight || lineCenter || lineLeft) {
          
          state = FOLLOW;
        }

        else sparki.moveForward();
        break;
      }

    case FOLLOW: {
        if (lineLeft) { // if line is below left line sensor
          sparki.moveLeft(); // turn left
        }

        if (lineRight) { // if line is below right line sensor
          sparki.moveRight(); // turn right
        }


        if (lineCenter && !lineLeft && !lineRight) { // if the center line sensor is the only one reading a line
          sparki.moveForward(); // move forward
        }

        if ( lineCenter && lineRight && lineLeft ) {
          state = STOP;
        }

        break;
      }

    case STOP: {
        sparki.gripperOpen();

        delay(4000);

        while (1) {
          //sparki.beep();
          sparki.moveRight();

        }
      }
  }

  delay(100); // wait 0.1 seconds
}


