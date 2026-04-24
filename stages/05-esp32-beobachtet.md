# Stufe 5 — ESP32 beobachtet den Taktgenerator

**Ziel:** Die Frequenz des 555 aus Stufe 1 mit dem ESP32 messen und im Serial Monitor anzeigen.
**Was du lernst:** Interrupts (`attachInterrupt`), Flankenerkennung, Frequenz aus Pulsen berechnen, `volatile`.
**Voraussetzung:** Stufen 1 und 4

## Konzept

555-Ausgang → ESP32-GPIO (Input). Interrupt zählt steigende Flanken. Einmal pro Sekunde liest `loop()` den Zähler aus → das ist direkt die Frequenz in Hz.

Wichtig: Mikrocontroller **beobachtet** nur. Der 555 läuft autark weiter — Reset-Pin, Versorgung, alles wie in Stufe 1.

## Schaltung

_TODO:_ 555-Ausgang (Pin 3) → ESP32-GPIO. Bei TLC555 auf 3,3 V direkt, keine Pegelwandlung nötig. Gemeinsames Massepotenzial (!).

## Code

_TODO:_ Sketch mit ISR, `volatile uint32_t` Zähler, Abtastintervall per `millis()`.

## Messen & Beobachten

- Serial Monitor zeigt Frequenz.
- Poti aus Stufe 1 Variante C drehen → Frequenz ändert sich in Echtzeit.
- Vergleich mit Multimeter / Oszi — wie genau ist die Messung?

## Diskussion

_TODO:_ Grenzen der Messung — sehr niedrige Frequenzen (<1 Hz) brauchen längeres Abtastintervall, sehr hohe Frequenzen stoßen an ISR-Overhead.

## Übergang zur nächsten Stufe

Jetzt weiß der ESP32, **was der 555 tut**. Im nächsten Schritt sagen wir ihm umgekehrt, **was er tun soll** — Mode-Controller in Software.
