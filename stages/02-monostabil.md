# Stufe 2 — Monostabil als Single-Step-Baustein

**Ziel:** Einen definierten Einzelpuls pro Tastendruck erzeugen — Basis des „single-step"-Modus.
**Was du lernst:** Trigger-Verhalten (aktiv LOW), Pulsbreite `T = 1,1·R·C`, Tasterentprellung.
**Voraussetzung:** Stufe 0, hilfreich Stufe 1

## Theorie

_TODO:_ Wie unterscheidet sich die Beschaltung von der astabilen? Trigger-Eingang (Pin 2) als aktiv-LOW, Flip-Flop setzt Ausgang HIGH für Zeit `T`.

## Schaltung

### Grundschaltung

_TODO:_ R an Pin 7, C an Pin 6/2, Pin 2 über Pull-up, Taster zieht Pin 2 gegen Masse.

### Variante — saubere Entprellung

_TODO:_ Taster mit kleinem C (10 nF) + R oder Schmitt-Trigger davor.

## Aufbau

_TODO:_ Breadboard-Skizze. Pulsbreite so wählen, dass LED sichtbar aufleuchtet (~500 ms).

## Messen & Beobachten

- Tastendruck → LED leuchtet für exakt `T` auf.
- Nochmaliger Druck während des Pulses: wird der Puls verlängert oder ignoriert? (Retriggerverhalten beobachten.)

## Übergang zur nächsten Stufe

Jetzt haben wir **dauerlaufend (Stufe 1)** und **einzelner Puls (Stufe 2)**. Was fehlt zu einem Auto/Single/Halt-Generator? Ein sauberes „Halt".
