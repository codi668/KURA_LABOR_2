#include <Arduino.h>
// Pin-Konfiguration
const int pwmPin = 9;          // PWM-Ausgang auf Pin 9
const int amplitude = 127;     // Amplitude des Sinus (0-127 für 50% Duty-Cycle)
const int offset = 128;        // Offset für Mittelwert (0-255)
const int numSamples = 100;    // Anzahl der Schritte für eine Sinusperiode
const int delayTime = 10;      // Zeitverzögerung in ms zwischen den Samples

// Array zur Speicherung der Sinuswerte
int sinusValues[numSamples];

void setup() {
  // Berechne die Sinuswerte für eine Periode und speichere sie im Array
  for (int i = 0; i < numSamples; i++) {
    float angle = (2.0 * PI * i) / numSamples;        // Winkel in Bogenmaß
    sinusValues[i] = offset + amplitude * sin(angle); // Sinuswert berechnen und skalieren
  }
}

void loop() {
  // Sinuswerte an den PWM-Pin ausgeben
  for (int i = 0; i < numSamples; i++) {
    analogWrite(pwmPin, sinusValues[i]); // PWM-Wert setzen
    delay(delayTime);                    // Verzögerung für eine gleichmäßige Ausgabe
  }
}

/*
 * Aufbau des RC-Tiefpassfilters
 * 
 * Verwende einen Widerstand (R) und einen Kondensator (C) als RC-Tiefpassfilter,
 * um das PWM-Signal zu glätten und eine annähernd analoge Sinuskurve zu erzeugen.
 *
 * RC-Tiefpassfilter-Schaltung:
 * 
 * PWM-Pin (Pin 9) ---- R (10kΩ) ----+---- Ausgang (geglättetes Signal)
 *                                   |
 *                                   C (10µF)
 *                                   |
 *                                 Masse (GND)
 * 
 * Schritte:
 * 1. Verbinde den PWM-Pin (Pin 9) des Arduino über einen 10kΩ-Widerstand mit einem Steckbrettpunkt.
 * 2. Verbinde den anderen Anschluss des 10kΩ-Widerstands mit einem 10µF-Kondensator.
 * 3. Schließe den anderen Anschluss des 10µF-Kondensators an Masse (GND) an.
 * 4. Der Verbindungspunkt zwischen dem Widerstand und dem Kondensator ist der geglättete 
 *    Ausgang des Sinussignals.
 *
 * Hinweis:
 * - Die Werte für R und C sind so gewählt, dass die Grenzfrequenz bei etwa 1.6 Hz liegt.
 * - Diese Schaltung hilft, die hochfrequenten Komponenten des PWM-Signals zu entfernen 
 *   und die Sinusform besser abzubilden.
 */
