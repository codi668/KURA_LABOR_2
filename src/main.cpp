#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define SIN_STEPS 200  // Anzahl der Schritte für die Sinuswelle
#define F_CPU 16000000UL  // Taktfrequenz des Arduino Uno (16 MHz)
#define AMPLITUDE 63.75  // Amplitude der Sinuswelle (halbe Amplitude von 127.5)

// Sinuswerte vorbereiten
uint8_t sinusWave[SIN_STEPS];

// Initialisierung der Sinuswellen-Daten
void initSinusWave() {
  for (int i = 0; i < SIN_STEPS; i++) {
    // Berechnet Sinuswerte und skaliert auf 8-Bit (0-255)
    sinusWave[i] = (uint8_t)(AMPLITUDE * (1 + sin(2 * M_PI * i / SIN_STEPS)));
  }
}

// Timer initialisieren für die gewünschte Frequenz
void initTimer() {
  // Setze Timer1 auf Normalmodus
  TCCR1A = 0;                 // Normaler Modus
  TCCR1B = (1 << CS10);       // Kein Prescaler (CS10)

  // Timer1 Overflow Interrupt aktivieren
  TIMSK1 = (1 << TOIE1);
}

// Setze die Frequenz des Timers
void setFrequency(uint16_t frequency) {
  // Berechne Vergleichswert für die gewünschte Frequenz
  uint16_t compareValue = F_CPU / (frequency * SIN_STEPS);

  // Setze den Timer1 auf den berechneten Wert
  TCNT1 = 65536 - compareValue;
}

ISR(TIMER1_OVF_vect) {
  static uint16_t index = 0;  // Speichert den aktuellen Index des Sinusarrays

  // Ausgabe des aktuellen Sinuswerts auf Port D (Pins 0-7)
  PORTD = sinusWave[index];
  
  // Index erhöhen und bei SIN_STEPS zurücksetzen
  index = (index + 1) % SIN_STEPS;

  // Setze den Timer1 auf den berechneten Wert zurück
  setFrequency(100);  // Beispiel: 100 Hz
}

int main(void) {
  // Setze Port D (Pins 0-7) als Ausgänge
  DDRD = 0xFF;

  // Initialisiere Sinus-Daten
  initSinusWave();

  // Initialisiere Timer
  initTimer();

  // Setze die gewünschte Frequenz (z.B. 100 Hz)
  setFrequency(300);

  // Globale Interrupts aktivieren
  sei();

  // Hauptschleife (wird nicht benötigt, da der Timer Interrupt alles steuert)
  while (1) {
    // Leerer Loop, da die Sinusausgabe über den Timer-Interrupt läuft
  }
}
