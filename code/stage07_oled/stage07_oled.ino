// Stage 7 — OLED Status
// 0,96" SSD1306 I2C-OLED an ESP32.
// Zeigt aktuellen Modus, Frequenz und Step-Zaehler.
// Kombiniert Stage 5 (Frequenzmessung) + Stage 6 (Mode-Controller).
//
// Benoetigte Bibliothek (Bibliotheks-Manager):
//   "Adafruit SSD1306" — zieht Adafruit GFX automatisch mit.
//
// Verdrahtung OLED:
//   VCC -> 3V3
//   GND -> GND
//   SCL -> GPIO 22   (I2C-Clock, ESP32 Default)
//   SDA -> GPIO 21   (I2C-Data,  ESP32 Default)
//
// ACHTUNG: GPIO 21 in diesem Sketch kollidiert mit PIN_LED_SINGLE
// aus Stage 6. Wenn beide Sketches kombiniert werden, muss die
// Single-LED auf einen anderen Pin umziehen (z.B. GPIO 23).

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

constexpr uint8_t OLED_W    = 128;
constexpr uint8_t OLED_H    = 64;
constexpr uint8_t OLED_ADDR = 0x3C;

Adafruit_SSD1306 display(OLED_W, OLED_H, &Wire, -1);

// Frequenzzaehlung (siehe Stage 5)
constexpr uint8_t FREQ_PIN = 34;
volatile uint32_t edgeCount = 0;
uint32_t lastSampleMs = 0;
float    lastFreqHz   = 0.0f;

void IRAM_ATTR onRisingEdge() {
  edgeCount++;
}

// Platzhalter fuer Mode + Step-Count (in der Integration aus Stage 6 uebernehmen)
const char* modeText = "AUTO";
uint32_t stepCount = 0;

void drawDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Mode ");
  display.println(modeText);

  display.setTextSize(1);
  display.setCursor(0, 24);
  display.print("f   = ");
  display.print(lastFreqHz, 2);
  display.println(" Hz");

  display.setCursor(0, 40);
  display.print("step= ");
  display.println(stepCount);

  display.display();
}

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 init fehlgeschlagen");
    while (true) { delay(1000); }
  }
  display.clearDisplay();
  display.display();

  pinMode(FREQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FREQ_PIN), onRisingEdge, RISING);

  Serial.println("Stage 7 OLED sketch ready.");
}

void loop() {
  uint32_t now = millis();
  if (now - lastSampleMs >= 1000) {
    noInterrupts();
    uint32_t count = edgeCount;
    edgeCount = 0;
    interrupts();

    uint32_t elapsedMs = now - lastSampleMs;
    lastSampleMs = now;

    lastFreqHz = (count * 1000.0f) / elapsedMs;
    drawDisplay();
  }
}
