// Stage 4 — Blink
// Blinkt die Onboard-LED (GPIO 2) im 1-Hz-Takt.
// Entspricht funktional dem astabilen TLC555 aus Stufe 1.

const uint8_t LED_PIN = 2;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
