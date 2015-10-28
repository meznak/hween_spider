/*
 * Sneaky spider Halloween prop.
 * 
 * Created 27 Oct 2015 by Nate Plamondon
 * 
 * Detects trick-or-treaters approaching my door, and lowers a giant spider to eye-level behind them.
 * When they turn around, surprise!
 * 
 * All that's needed is a PIR sensor and a continuous-rotation servo.
 * PIR: http://osepp.com/products/sensors-arduino-compatible/passive-infrared-sensor-pir-module/
 * Servo: https://www.parallax.com/product/900-00008
 * 
 * Attach a thread spool to the servo horn, and wrap it with fishing line or thread.
 * Mount creepy spider to free end of line.
 * How you mount this setup to your house is up to you.
 * 
 * You will need to modify 'turnTime' and 'hangTime' variables to suit your needs.
 * 
 * Circuit:
 * * PIR sensor sense pin to pin 7
 * * Servo control pin to pin 9
 * * If you need more servos and/or power, attach servo(s) to a separate supply.
 *   Make sure to tie supply and Arduino grounds together.
 * 
 */

#include <Servo.h>

static int pirPin = 7;
static int ledPin = 13;
static int servoPin = 9;

static unsigned long turnTime = 3000; // Time it takes to lower the spider.
static unsigned long hangTime = 5000; // Time for the spider to hang out before climbing.
                                      //   This wait time is after the spider is lowered
                                      //   and the sensor stops detecting movement.

Servo servo;
static int servoStop = 95;            // Center 'hold' speed for the servo. Yours may vary.
static int servoUp = 180;             // Full speed CW.
static int servoDown = 0;             // Full speed CCW.

boolean spiderIsDown = false;
unsigned long currentTime = 0;
unsigned long lastDetectTime = 0;
unsigned long liftTargetTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pirPin, INPUT); // PIR
  pinMode(ledPin, OUTPUT); // LED
  servo.attach(servoPin);
  servo.write(95);                    // Stop the servo initially.
  Serial.print("Started at ");
  Serial.println(millis());
}

void loop() {
  if (digitalRead(pirPin)) {
    lastDetectTime  = millis();
    liftTargetTime = lastDetectTime + turnTime + hangTime;
    Serial.print("Detect at ");
    Serial.println(lastDetectTime);
    Serial.print("Will lift at ");
    Serial.println(liftTargetTime);
    if (spiderIsDown == false) {
      dropSpider();
      spiderIsDown = true;
      Serial.println(spiderIsDown);
    }
  }
  currentTime = millis();
  if (spiderIsDown == true && currentTime > liftTargetTime) {
    liftSpider();
    spiderIsDown = false;
    Serial.println(spiderIsDown);
  }
  
  if (spiderIsDown == true) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

void dropSpider() {
  Serial.print("Lowering at ");
  Serial.println(millis());
  
  servo.write(servoDown);
  delay(turnTime);
  servo.write(servoStop);

  Serial.print("Lowered at ");
  Serial.println(millis());
}

void liftSpider() {
  Serial.print("Lifting at ");
  Serial.println(millis());
  
  servo.write(servoUp);
  delay(turnTime);
  servo.write(servoStop);

  Serial.print("Lifted at ");
  Serial.println(millis());
}
