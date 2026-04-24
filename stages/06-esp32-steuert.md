# Stufe 6 — ESP32 steuert den Taktgenerator (Zielstufe)

**Ziel:** Die drei Modi `auto` / `single` / `halt` per ESP32 umschalten — das Kernziel des Tutorials.
**Was du lernst:** State-Machine in Arduino, Mode-Auswahl per Taster, Reset-Pin-Steuerung, Trigger-Puls erzeugen.
**Voraussetzung:** Stufen 2, 3, 4 (empfohlen Stufe 5)

## Konzept

Wir übernehmen den Hardware-Aufbau aus **Stufe 3** (zwei 555er A astabil + B monostabil), ersetzen aber den Mode-Schalter durch den ESP32:

| ESP32-Pin | Zielsignal |
|-----------|------------|
| `GPIO_RESET_A` → 555 A Pin 4 | `HIGH` = Oszillator läuft, `LOW` = Halt |
| `GPIO_RESET_B` → 555 B Pin 4 | `LOW` außer im `single`-Modus |
| `GPIO_TRIGGER_B` → 555 B Pin 2 (über Kondensator) | kurzer LOW-Puls startet Single-Step |
| `GPIO_MODE_AUTO`, `GPIO_MODE_SINGLE`, `GPIO_MODE_HALT` | Eingänge mit Pullup, drei Mode-Taster |
| `GPIO_STEP` | Eingang, Step-Taster im Single-Modus |

## Software — State-Machine

Drei Zustände: `HALT`, `AUTO`, `SINGLE`. Transitions:

- Beliebiger Modus-Taster → wechselt direkt in den Zustand.
- Im `SINGLE`: Step-Taster → generiert einen Trigger-Puls an 555 B.

_TODO:_ Code-Skelett mit `enum Mode`, `switch`-basierter State-Machine in `loop()`, Entprellung per `millis()`.

## Schaltung

_TODO:_ Schaltplan auf Basis von Stufe 3, Mode-Schalter entfernt, ESP32 eingefügt. Gemeinsame Masse zwischen ESP32 und 555-Versorgung zwingend.

## Messen & Beobachten

- Status-LEDs: je eine pro Modus + eine am Takt-Ausgang.
- Optional: Frequenzmessung aus Stufe 5 läuft parallel mit — ESP32 beobachtet den Ausgang, den er selbst steuert (schönes Lehrbeispiel).

## Diskussion

- Warum nicht einfach die ganze Taktgenerierung in Software? → Antwort: Wollen wir — aber das Tutorial-Thema ist der 555. Die Hybrid-Lösung zeigt, wie MCU und analoger Timer zusammenspielen.
- Wo sind die Grenzen? _TODO:_ GPIO-Timing-Jitter, RTOS-Tasks etc.

## Übergang zur nächsten Stufe

Funktional sind wir fertig. Als Ausbau: **Anzeige, Bedienkomfort, Web-UI** — Stufe 7.
