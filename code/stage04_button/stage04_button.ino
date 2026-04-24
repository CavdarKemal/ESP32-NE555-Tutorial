// Stage 4 — Button read
// Taster an GPIO 32 gegen GND, INPUT_PULLUP.
// LED auf GPIO 2 leuchtet, solange der Taster gedrueckt ist.
// Serial-Ausgabe bei jeder fallenden Flanke.

const uint8_t BUTTON_PIN = 32;
const uint8_t LED_PIN    = 2;

bool lastPressed = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println("Stage 4 button sketch ready.");
}

void loop() {
  bool pressed = (digitalRead(BUTTON_PIN) == LOW);
  digitalWrite(LED_PIN, pressed ? HIGH : LOW);

  if (pressed && !lastPressed) {
    Serial.println("Button pressed");
  }
  lastPressed = pressed;

  delay(10);
}
