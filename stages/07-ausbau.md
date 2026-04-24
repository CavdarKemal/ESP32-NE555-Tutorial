# Stufe 7 — Ausbau (optional)

**Ziel:** Das Projekt komfortabler und vorzeigbar machen.
**Was du lernst:** I²C-Display, ADC (Poti als Frequenzregler), ESP32-WLAN, einfache Web-UI.
**Voraussetzung:** Stufe 6

## Ideen (in steigender Schwierigkeit)

### Ausbau A — Status-OLED

_TODO:_ 0,96" OLED (SSD1306) via I²C. Anzeige: aktueller Modus, aktuelle Frequenz (aus Stufe 5), Step-Zähler.

### Ausbau B — Frequenz-Sollwert per Poti

_TODO:_ Poti am ADC des ESP32 → ESP32 kennt das Verhältnis Poti-Stellung ↔ Zielfrequenz. Anzeige von Soll- und Ist-Frequenz.

### Ausbau C — Frequenz softwareseitig einstellen

_TODO:_ Digital-Potentiometer (z. B. MCP41010) ersetzt R2 im 555 A. ESP32 setzt den Wert per SPI → echte Software-steuerbare Frequenz. Alternative: komplett auf ESP32-LEDC/PWM wechseln und den 555 „ablösen" — bewusst didaktischer Endpunkt.

### Ausbau D — Web-UI

_TODO:_ ESP32 als WLAN-Access-Point oder im lokalen WLAN, kleine HTML-Seite mit Mode-Buttons und Frequenz-Slider.

## Abschluss

_TODO:_ Zusammenfassung, Lessons Learned, mögliche nächste Projekte (z. B. einfache Logik-Schaltung ansteuern, CPU-Modell takten, Schrittmotor).
