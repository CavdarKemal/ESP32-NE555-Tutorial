# Stufe 1 — Astabiler Taktgenerator

**Ziel:** Einen freilaufenden Oszillator aufbauen — der Kern des späteren „auto"-Modus.
**Was du lernst:** RC-Zeitkonstante, Duty Cycle, Frequenzformel, Grenzen in der Praxis.
**Voraussetzung:** Stufe 0

## Theorie

_TODO:_ Zeitkonstante τ = R·C. Ladevorgang zwischen 1/3 und 2/3 V<sub>CC</sub>. Formel
`f = 1,44 / ((R1 + 2·R2) · C)`. Warum Duty Cycle standardmäßig > 50 % ist.

## Schaltung

### Variante A — Klassisch (Duty > 50 %)

_TODO:_ Schaltplan mit R1, R2, C zwischen Pin 7, 6/2 und Masse.

### Variante B — Diodentrick für ~50 % Duty

_TODO:_ 1N4148 parallel zu R2, umgeht R2 beim Laden.

### Variante C — Variable Frequenz

_TODO:_ 100 kΩ-Poti als R2.

## Aufbau

_TODO:_ Breadboard-Skizze, Pin-5 Bypass-Cap (100 nF nach GND) nicht vergessen.

## Messen & Beobachten

- Niedrige Frequenz (1–2 Hz) → LED sichtbar blinkend.
- Mittlere Frequenz (ab ~50 Hz) → LED erscheint „dauerhaft an" → Multimeter / Oszi.
- Duty Cycle bei Variante A vs B vergleichen.

## Übergang zur nächsten Stufe

Der astabile Oszillator läuft ununterbrochen. Für „single-step" brauchen wir etwas, das **genau einen** Puls liefert — der Monostabile.
