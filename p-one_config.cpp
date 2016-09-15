
#include "application.h"

int ledState = LOW;


#include "SparkTime.h"

UDP UDPClient;
SparkTime rtc;

unsigned long currentTime;
//unsigned long lastTime = 0UL;
String timeStr;

/**
 * Reads the value of the photoresistor and
 * publishes the value to the particle cloud.
 */
void readPhotoResistor() {

    // Read the analog input.
    int lightlevel = analogRead(A0);

    // Publish the value to particle.
    Particle.publish("p-one_light-level", String(lightlevel));
    delay(7000);

    // Right it serial for debugging.
    Serial.println("light-level=" + String(lightlevel));
}

Timer timer(5000, readPhotoResistor);

/**
 * Called automatically by Photon once when the device
 * is powered on.
 */
void setup() {

    Serial.begin(9600);
    timer.start();

    // Power to photoresistor.
    pinMode(A4, OUTPUT);

    // Power to LED.
    pinMode(D0, OUTPUT);

    // Send power to the photoresistor.
    digitalWrite(A4, HIGH);

    rtc.begin(&UDPClient, "north-america.pool.ntp.org");
    rtc.setTimeZone(-5); // gmt offset

}

void loop() {
    currentTime = rtc.now();
    if (currentTime != lastTime) {
      byte sec = rtc.second(currentTime);
      if (sec == 10) {
	// Build Date String
	timeStr = "";
	timeStr += rtc.dayOfWeekString(currentTime);
	timeStr += ", ";
	timeStr += rtc.monthNameString(currentTime);
	timeStr += " ";
	timeStr += rtc.dayString(currentTime);
	timeStr += ", ";
	timeStr += rtc.yearString(currentTime);
	Serial.println(timeStr);
      } else if (sec == 40) {
	// Including current timezone
	Serial.println(rtc.ISODateString(currentTime));
      } else if (sec == 50) {
	// UTC or Zulu time
	Serial.println(rtc.ISODateUTCString(currentTime));
      } else {
	// Just the time in 12 hour format
	timeStr = "";
	timeStr += rtc.hour12String(currentTime);
	timeStr += ":";
	timeStr += rtc.minuteString(currentTime);
	timeStr += ":";
	timeStr += rtc.secondString(currentTime);
	timeStr += " ";
	timeStr += rtc.AMPMString(currentTime);
	Serial.println(timeStr);
      }
      lastTime = currentTime;
    }

}
