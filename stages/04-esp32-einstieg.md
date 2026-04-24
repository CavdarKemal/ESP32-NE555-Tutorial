# Stufe 4 — ESP32 Einstieg

**Ziel:** Den ESP32 in Betrieb nehmen und mit GPIOs arbeiten.
**Was du lernst:** Arduino IDE einrichten, Board-Support, `setup()` / `loop()`, `pinMode`, `digitalWrite`, `digitalRead`, Serial Monitor.
**Voraussetzung:** keine ESP32-Vorkenntnisse

## Theorie (Hardware-Kurzüberblick)

_TODO:_ ESP32-DevKit V1 Pinout (kurz), Betriebsspannung 3,3 V, GPIO-Strombegrenzung, Pins mit Sonderfunktionen (Strapping-Pins, ADC, nur-Eingang).

### 3,3 V / 5 V? In unserem Fall: kein Problem

Weil wir den **TLC555/LMC555** nutzen und bei 3,3 V betreiben, können wir GPIOs direkt koppeln. (Beim klassischen NE555 bei 5 V wäre ein Spannungsteiler am Eingang und ein Level-Shifter am Ausgang nötig — als Randinfo erwähnt.)

## Toolchain

_TODO:_ Arduino IDE installieren, ESP32 Board-Support über Boards Manager (`esp32 by Espressif`), richtigen COM-Port wählen, Treiber (CP2102 / CH340) falls nötig.

## Erste Sketches

### Sketch 1 — Blink

_TODO:_ LED an z. B. GPIO 2, blinkt im 1-Sekunden-Takt. Entspricht funktional dem astabilen 555 — guter Rückbezug auf Stufe 1.

### Sketch 2 — Taster lesen

_TODO:_ Taster an GPIO mit `INPUT_PULLUP`, bei Druck Serial-Ausgabe.

## Messen & Beobachten

- LED blinkt → ESP32 läuft
- Serial Monitor reagiert auf Tastendruck
- Optional: Oszi an den Blink-Pin anschließen und mit 555-Ausgang aus Stufe 1 vergleichen.

## Übergang zur nächsten Stufe

Der ESP32 ist online. Jetzt lassen wir ihn **den 555 beobachten**, nicht steuern — erst messen, dann eingreifen.
