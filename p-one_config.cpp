/**
 * Losant Particle workshop source code for Photon.
 *
 * Copyright (c) 2016 Losant IoT. All rights reserved.
 * https://www.losant.com
 */

#include "application.h"

int ledState = LOW;

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
 * Sets the state of the LED.
 */
void setLEDState(int state) {
    digitalWrite(D0, state);
    ledState = state;

    Serial.println("led-state=" + String(ledState));
}

/**
 * Handles an event from particle cloud.
 */
void ledToggleEvent(const char *event, const char *data) {

    String command = String(event);
    if(command.compareTo(String("p-one_led-on")) == 0) {
        setLEDState(HIGH);
    }
    else if(command.compareTo(String("p-one_led-off")) == 0) {
        setLEDState(LOW);
    }
    else if(command.compareTo(String("p-one_led-toggle")) == 0) {
        setLEDState(ledState == HIGH ? LOW : HIGH);
    }
    else if(command.compareTo(String("p-one_led-blink")) == 0) {
        for (int x=0; x<100; x=x+1) {
          digitalWrite(D0, HIGH);
          delay(1000);
          digitalWrite(D0, LOW);
          delay(1000);
        }
    }
}

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

    // Subscribe to events that control the LED.
    Particle.subscribe("p-one_led-on", ledToggleEvent);
    Particle.subscribe("p-one_led-off", ledToggleEvent);
    Particle.subscribe("p-one_led-toggle", ledToggleEvent);
    Particle.subscribe("p-one_led-blink", ledToggleEvent);

}

void loop() {

}
