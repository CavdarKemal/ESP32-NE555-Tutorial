// Stage 6 — Mode Controller
// Drei Modi fuer den Hardware-Taktgenerator aus Stufe 3:
//   HALT   — beide 555 ueber RESET stillgelegt
//   AUTO   — 555 A astabil freigegeben
//   SINGLE — 555 B monostabil freigegeben, Step-Taster triggert
//
// Verdrahtung:
//   GPIO 25 -> 555 A Pin 4 (RESET_A)         OUTPUT
//   GPIO 26 -> 555 B Pin 4 (RESET_B)         OUTPUT
//   GPIO 27 -> 555 B Pin 2 (TRIG_B)          OUTPUT, idle HIGH
//   GPIO 32 <- Taster MODE_HALT   (gg. GND)  INPUT_PULLUP
//   GPIO 33 <- Taster MODE_AUTO   (gg. GND)  INPUT_PULLUP
//   GPIO 14 <- Taster MODE_SINGLE (gg. GND)  INPUT_PULLUP
//   GPIO 13 <- Taster STEP        (gg. GND)  INPUT_PULLUP
//   GPIO 18 -> LED HALT   + 330R
//   GPIO 19 -> LED AUTO   + 330R
//   GPIO 21 -> LED SINGLE + 330R
//   GND     <-> gemeinsame Masse mit 555-Aufbau
//
// Die Pull-downs an RESET_A/B aus Stufe 3 koennen entfernt werden,
// weil der ESP32 die Pegel jetzt aktiv treibt.

#include <Arduino.h>

// ---------- Pins ----------
constexpr uint8_t PIN_RESET_A    = 25;
constexpr uint8_t PIN_RESET_B    = 26;
constexpr uint8_t PIN_TRIG_B     = 27;

constexpr uint8_t PIN_BTN_HALT   = 32;
constexpr uint8_t PIN_BTN_AUTO   = 33;
constexpr uint8_t PIN_BTN_SINGLE = 14;
constexpr uint8_t PIN_BTN_STEP   = 13;

constexpr uint8_t PIN_LED_HALT   = 18;
constexpr uint8_t PIN_LED_AUTO   = 19;
constexpr uint8_t PIN_LED_SINGLE = 21;

// ---------- Konstanten ----------
constexpr uint32_t DEBOUNCE_MS   = 25;
constexpr uint32_t TRIG_PULSE_MS = 150;  // LOW-Puls an TRIG_B. Grosszuegig, damit der
                                         // Puls im Wokwi-Sim an der TRG_B-LED sichtbar
                                         // wird; der echte 555 B triggert ohnehin auf
                                         // die fallende Flanke, nicht auf die Pulsdauer.

// ---------- Mode-State ----------
enum Mode : uint8_t { MODE_HALT = 0, MODE_AUTO = 1, MODE_SINGLE = 2 };
Mode currentMode = MODE_HALT;

// ---------- Button Debouncing ----------
// lastRaw / stable speichern den semantischen "gedrueckt"-Zustand
// (true = Taster unten). Initial "nicht gedrueckt".
struct Button {
  uint8_t  pin;
  bool     lastRaw;      // letzter Rohwert: true = gedrueckt
  bool     stable;       // entprellter Zustand: true = gedrueckt
  uint32_t lastChangeMs; // Zeitpunkt der letzten Rohwert-Aenderung
};

Button btnHalt   = {PIN_BTN_HALT,   false, false, 0};
Button btnAuto   = {PIN_BTN_AUTO,   false, false, 0};
Button btnSingle = {PIN_BTN_SINGLE, false, false, 0};
Button btnStep   = {PIN_BTN_STEP,   false, false, 0};

// Liefert true genau im Moment der Drueck-Flanke (nach Entprellung).
bool detectPressEdge(Button& b) {
  const bool raw = (digitalRead(b.pin) == LOW);
  const uint32_t now = millis();

  if (raw != b.lastRaw) {
    b.lastRaw = raw;
    b.lastChangeMs = now;
    return false;
  }
  if ((now - b.lastChangeMs) >= DEBOUNCE_MS && raw != b.stable) {
    b.stable = raw;
    return raw;  // true = Druecken, false = Loslassen
  }
  return false;
}

// ---------- Mode-Anwendung ----------
const char* modeName(Mode m) {
  switch (m) {
    case MODE_HALT:   return "HALT";
    case MODE_AUTO:   return "AUTO";
    case MODE_SINGLE: return "SINGLE";
  }
  return "?";
}

void updateStatusLeds(Mode m) {
  digitalWrite(PIN_LED_HALT,   m == MODE_HALT   ? HIGH : LOW);
  digitalWrite(PIN_LED_AUTO,   m == MODE_AUTO   ? HIGH : LOW);
  digitalWrite(PIN_LED_SINGLE, m == MODE_SINGLE ? HIGH : LOW);
}

void applyMode(Mode m) {
  currentMode = m;
  switch (m) {
    case MODE_HALT:
      digitalWrite(PIN_RESET_A, LOW);
      digitalWrite(PIN_RESET_B, LOW);
      break;
    case MODE_AUTO:
      digitalWrite(PIN_RESET_A, HIGH);
      digitalWrite(PIN_RESET_B, LOW);
      break;
    case MODE_SINGLE:
      digitalWrite(PIN_RESET_A, LOW);
      digitalWrite(PIN_RESET_B, HIGH);
      break;
  }
  updateStatusLeds(m);
  Serial.print("Mode: ");
  Serial.println(modeName(m));
}

void triggerStep() {
  digitalWrite(PIN_TRIG_B, LOW);
  delay(TRIG_PULSE_MS);
  digitalWrite(PIN_TRIG_B, HIGH);
  Serial.println("Step!");
}

// ---------- Setup / Loop ----------
void setup() {
  Serial.begin(115200);

  pinMode(PIN_RESET_A, OUTPUT);
  pinMode(PIN_RESET_B, OUTPUT);
  pinMode(PIN_TRIG_B,  OUTPUT);
  digitalWrite(PIN_TRIG_B, HIGH);   // idle HIGH — kein Trigger

  pinMode(PIN_LED_HALT,   OUTPUT);
  pinMode(PIN_LED_AUTO,   OUTPUT);
  pinMode(PIN_LED_SINGLE, OUTPUT);

  pinMode(PIN_BTN_HALT,   INPUT_PULLUP);
  pinMode(PIN_BTN_AUTO,   INPUT_PULLUP);
  pinMode(PIN_BTN_SINGLE, INPUT_PULLUP);
  pinMode(PIN_BTN_STEP,   INPUT_PULLUP);

  applyMode(MODE_HALT);
  Serial.println("Stage 6 controller ready.");
}

void loop() {
  if (detectPressEdge(btnHalt))   applyMode(MODE_HALT);
  if (detectPressEdge(btnAuto))   applyMode(MODE_AUTO);
  if (detectPressEdge(btnSingle)) applyMode(MODE_SINGLE);

  if (detectPressEdge(btnStep) && currentMode == MODE_SINGLE) {
    triggerStep();
  }
}
