#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define SIN_STEPS 100  // Anzahl der Schritte für die Sinuswelle
#define F_CPU 16000000UL  // Taktfrequenz des Arduino Uno (16 MHz)

// Sinuswerte vorbereiten
uint8_t sinusWave[SIN_STEPS];

// Initialisierung der Sinuswellen-Daten
void initSinusWave() {
  for (int i = 0; i < SIN_STEPS; i++) {
    // Berechnet Sinuswerte und skaliert auf 8-Bit (0-255)
    sinusWave[i] = (uint8_t)(127.5 * (1 + sin(2 * M_PI * i / SIN_STEPS)));
  }
}

// Timer initialisieren für die gewünschte Frequenz
void initTimer() {
  // Setze Timer1 auf CTC-Modus (Clear Timer on Compare Match)
  TCCR1A = 0;                 // Normaler Modus
  TCCR1B = (1 << WGM12) | (1 << CS10);  // CTC Modus, kein Prescaler (CS10)

  // Berechne Vergleichswert für ca. 100 Hz Sinusfrequenz (anpassbar)
  OCR1A = F_CPU / (100 * SIN_STEPS) - 1;  

  // Timer1 Compare Match A Interrupt aktivieren
  TIMSK1 = (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  static uint16_t index = 0;  // Speichert den aktuellen Index des Sinusarrays

  // Ausgabe des aktuellen Sinuswerts auf Port D (Pins 0-7)
  PORTD = sinusWave[index];
  
  // Index erhöhen und bei SIN_STEPS zurücksetzen
  index = (index + 1) % SIN_STEPS;
}

int main(void) {
  // Setze Port D (Pins 0-7) als Ausgänge
  DDRD = 0xFF;

  // Initialisiere Sinus-Daten und Timer
  initSinusWave();
  initTimer();

  // Globale Interrupts aktivieren
  sei();

  // Hauptschleife (wird nicht benötigt, da der Timer Interrupt alles steuert)
  while (1) {
    // Leerer Loop, da die Sinusausgabe über den Timer-Interrupt läuft
  }
}
