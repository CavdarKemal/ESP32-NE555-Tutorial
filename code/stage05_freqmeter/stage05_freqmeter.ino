// Stage 5 — Frequenzmesser
// Zaehlt steigende Flanken am TLC555-Ausgang (Stufe 1) und gibt
// die Frequenz einmal pro Sekunde auf Serial aus.
//
// Verdrahtung:
//   TLC555 Pin 3 (OUT) ---- GPIO 34 (input-only)
//   TLC555 GND       ----  ESP32 GND  (gemeinsame Masse!)
//
// GPIO 34 hat keinen internen Pull-up/Pull-down. Der TLC555 treibt
// den Pegel aktiv, daher nicht notwendig.

const uint8_t FREQ_PIN   = 34;
const uint32_t SAMPLE_MS = 1000;

volatile uint32_t edgeCount = 0;
uint32_t lastSampleMs = 0;

void IRAM_ATTR onRisingEdge() {
  edgeCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(FREQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FREQ_PIN), onRisingEdge, RISING);
  Serial.println("Stage 5 frequency meter ready.");
}

void loop() {
  uint32_t now = millis();
  if (now - lastSampleMs >= SAMPLE_MS) {
    noInterrupts();
    uint32_t count = edgeCount;
    edgeCount = 0;
    interrupts();

    uint32_t elapsedMs = now - lastSampleMs;
    lastSampleMs = now;

    // f [Hz] = count / (elapsedMs / 1000)
    float freqHz = (count * 1000.0f) / elapsedMs;
    Serial.print("f = ");
    Serial.print(freqHz, 2);
    Serial.println(" Hz");
  }
}
