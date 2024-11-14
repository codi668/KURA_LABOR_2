#include <Arduino.h>
// Konfiguration
const int numPins = 4;          // Anzahl der Pins für das R-2R Netzwerk
const int pins[numPins] = {8, 9, 10, 11};  // Arduino-Pins für R-2R Netzwerk
const int numSamples = 100;     // Anzahl der Schritte für eine Sinusperiode
const int delayTime = 10;       // Zeitverzögerung in ms zwischen den Samples

// Array zur Speicherung der Sinuswerte
int sinusValues[numSamples];

void setup() {
  // Setze die Pins auf OUTPUT
  for (int i = 0; i < numPins; i++) {
    pinMode(pins[i], OUTPUT);
  }

  // Berechne die Sinuswerte und speichere sie im Array
  for (int i = 0; i < numSamples; i++) {
    float angle = (2.0 * PI * i) / numSamples;           // Winkel in Bogenmaß
    float sinValue = 0.5 + 0.5 * sin(angle);             // Sinus (0-1 Bereich)
    sinusValues[i] = int(sinValue * (1 << numPins) - 1); // Skalieren auf 4 Bit
  }
}

void loop() {
  // Sinuswerte über das R-2R Netzwerk ausgeben
  for (int i = 0; i < numSamples; i++) {
    outputToR2R(sinusValues[i]);
    delay(delayTime);
  }
}

// Funktion zur Ausgabe an das R-2R Netzwerk
void outputToR2R(int value) {
  for (int i = 0; i < numPins; i++) {
    int bitValue = (value >> i) & 0x01;   // Einzelnes Bit extrahieren
    digitalWrite(pins[i], bitValue);      // Bit an Pin schreiben
  }
}

