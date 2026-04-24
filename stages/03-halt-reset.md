# Stufe 3 — Halt über Pin 4 (Reset): Vollständiger Taktgenerator in Hardware

**Ziel:** Alle drei Modi `auto` / `single` / `halt` **ohne Mikrocontroller** realisieren.
**Was du lernst:** Reset-Eingang (Pin 4) des 555, Modus-Umschaltung mit Schalter, Kombination zweier 555.
**Voraussetzung:** Stufen 1 und 2

## Theorie

_TODO:_ Pin 4 (Reset) ist aktiv LOW — solange LOW, ist der Ausgang gezwungen LOW. Damit lässt sich der astabile Oszillator still-legen, ohne die Versorgung zu unterbrechen.

## Schaltung

Zwei 555er kombinieren:
- **555 A** astabil aus Stufe 1 → liefert „auto"-Takt
- **555 B** monostabil aus Stufe 2 → liefert „single"-Puls

Ein **Drei-Stellungs-Schalter** (oder drei Taster + RS-Flip-Flop) wählt den Ausgangstakt:

| Stellung | Ausgang |
|----------|---------|
| `halt` | konstant LOW (beide 555 via Reset-Pin stillgelegt) |
| `auto` | Ausgang von 555 A |
| `single` | Ausgang von 555 B (einmaliger Puls bei Trigger) |

### Variante A — Schiebeschalter + Dioden-OR

_TODO:_ Einfachste Lösung, aber Schalter „klickt" durch die Zustände.

### Variante B — Drei Taster mit Latch

_TODO:_ Eleganter: jedes Taster-Ereignis setzt den gewünschten Zustand, bis ein anderer gewählt wird. Hier bietet sich ein weiterer 555 **bistabil** (als RS-FF) oder ein 74HC74 an.

## Aufbau

_TODO:_ Breadboard-Skizze, beide 555 nebeneinander, sauberer Stromversorgungsblock.

## Messen & Beobachten

- LED am Ausgang beobachtet den gewählten Modus.
- Im `single`-Modus Pulsbreite mit dem Multimeter abschätzen (oder Oszi).

## Übergang zur nächsten Stufe

**Wir haben das Ziel bereits rein analog erreicht.** Der ESP32 ist nicht nötig — er wird das Ganze aber komfortabler, messbar und erweiterbar machen. Als Nächstes lernen wir den ESP32 kennen.
