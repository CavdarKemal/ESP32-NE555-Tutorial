# Stufe 0 — Grundlagen TLC555

**Ziel:** Den 555-Timer verstehen, bevor wir ihn verschalten.
**Was du lernst:** Pinbelegung, interne Funktion (vereinfacht), die drei Grundmodi, Unterschiede zwischen den 555-Varianten.
**Voraussetzung:** keine
**Dauer:** ca. 15 Minuten Lesezeit, kein Aufbau

## Was bauen wir?

Diese Stufe ist die einzige **ohne Breadboard**. Du lernst den TLC555 kennen: wie er aussieht, welche Pins was tun und warum er drei völlig verschiedene Dinge kann. Die Stufe ist bewusst kurz — wenn du ungeduldig bist, überflieg sie und starte mit [Stufe 1](01-astabil.md); du kommst zum Nachschlagen ohnehin hierher zurück.

## Pinbelegung (DIP-8)

Der TLC555 kommt im klassischen 8-poligen DIL-Gehäuse. Die Pinbelegung ist **identisch** zum alten NE555 — jede Schaltung aus einem 555-Kochbuch lässt sich übernehmen.

```
        ┌──────┐
  GND ──│ 1  8 │── VCC
  TRIG ─│ 2  7 │── DISCH
   OUT ─│ 3  6 │── THRES
 RESET ─│ 4  5 │── CTRL
        └──────┘
```

| Pin | Name | Funktion |
|-----|------|----------|
| 1 | GND | Masse |
| 2 | TRIG | Trigger-Eingang (aktiv LOW; unter 1/3 V<sub>CC</sub>) |
| 3 | OUT | Ausgang (Push-Pull) |
| 4 | RESET | Reset (aktiv LOW; LOW zwingt OUT auf LOW) |
| 5 | CTRL | Steuerspannung (im Normalfall nur 100 nF nach GND) |
| 6 | THRES | Schwellwert-Eingang (kippt bei 2/3 V<sub>CC</sub>) |
| 7 | DISCH | Entladepin (Open-Drain gegen GND, vom Flip-Flop gesteuert) |
| 8 | VCC | Versorgung (TLC555: 2–15 V; wir nehmen 3,3 V) |

> **Merksatz:** Pin 2 kippt bei **1/3**, Pin 6 kippt bei **2/3** der Versorgungsspannung.

## Wie funktioniert der 555 innen?

Innen sitzen vier Baugruppen, die zusammenspielen:

1. **Spannungsteiler aus drei gleichen Widerständen** zwischen V<sub>CC</sub> und GND — erzeugt die beiden magischen Schwellen **1/3 V<sub>CC</sub>** und **2/3 V<sub>CC</sub>**. Die 1/3-Schwelle wird intern **TRIG** (Pin 2) angeboten, die 2/3-Schwelle **THRES** (Pin 6). Der Abgriff der oberen Schwelle liegt auch extern auf **Pin 5 (CTRL)** — darüber ließen sich die Schwellen verschieben; wir machen davon keinen Gebrauch und überbrücken Pin 5 nur mit 100 nF nach GND.
2. **Unterer Komparator:** setzt das Flip-Flop (**S**), sobald TRIG (Pin 2) unter 1/3 V<sub>CC</sub> fällt → Ausgang geht auf **HIGH**.
3. **Oberer Komparator:** setzt das Flip-Flop zurück (**R**), sobald THRES (Pin 6) über 2/3 V<sub>CC</sub> steigt → Ausgang geht auf **LOW**.
4. **Entlade-Transistor (Pin 7, DISCH):** wird vom Flip-Flop mitgesteuert. Bei LOW-Ausgang zieht er Pin 7 gegen Masse — damit entladen wir später den Timing-Kondensator.

Mehr muss man am Anfang nicht wissen. Alle Schaltungen, die wir in den nächsten Stufen bauen, setzen an genau diesen vier Punkten an.

## Die drei Betriebsmodi

### Astabil — „läuft von alleine"

Ein externer R/C-Zweig lädt und entlädt den Kondensator zwischen 1/3 und 2/3 V<sub>CC</sub>. Ergebnis: ständige Rechteckschwingung, ohne externen Trigger.
→ Das wird unser **`auto`-Modus**, siehe [Stufe 1](01-astabil.md).

### Monostabil — „ein Puls pro Anstoß"

Ein kurzer LOW-Impuls an Pin 2 startet **einen einzelnen Puls** fester Länge. Danach wartet der Baustein auf den nächsten Trigger.
→ Das wird unser **`single-step`-Modus**, siehe [Stufe 2](02-monostabil.md).

### Bistabil — „Schalter mit Gedächtnis"

Keine Timing-Bauteile; nur TRIG und RESET. Der 555 verhält sich wie ein RS-Flip-Flop. Diese Variante taucht bei uns am Rande auf, wenn wir in [Stufe 3](03-halt-reset.md) einen sauberen Mode-Latch brauchen.

## TLC555 / LMC555 / NE555 — was ist der Unterschied?

Alle drei sind pinkompatibel. Unterschiede spürt man vor allem bei **Versorgungsspannung** und **Eingangsstrom**:

| | NE555 | TLC555 | LMC555 |
|-|-------|--------|--------|
| Technologie | bipolar | CMOS (TI) | CMOS (TI / NatSemi) |
| Versorgung | 4,5 – 16 V | 2 – 15 V | 1,5 – 15 V |
| Ruhestrom (typ.) | 3 – 15 mA | ≈ 170 µA | ≈ 100 µA |
| Max. Ausgangsstrom | ≈ 200 mA | ≈ 100 mA | ≈ 50 mA |
| Eingangsstrom | ~100 nA | pA-Bereich | pA-Bereich |
| Max. Frequenz (typ.) | ≈ 500 kHz | ≈ 2 MHz | ≈ 3 MHz |

**Für uns wichtig:**

- Der TLC555 läuft bei 3,3 V — genau wie der ESP32. Wir brauchen **keinen Pegelwandler**.
- Die extrem hochohmigen CMOS-Eingänge erlauben sehr große R-Werte im Timing-Zweig (bis einige MΩ), ohne dass der Eingangsstrom die Zeitkonstante verfälscht. Praktisch für sehr langsame Takte.
- Der klassische NE555 gehört zum Kulturgut, aber sein höherer Versorgungsstrom und der 5-V-Mindestpegel passen schlechter zum ESP32.

## Rückblick

Was du jetzt weißt:

- Der TLC555 ist ein **8-poliger Timer-Baustein**, pinkompatibel zum NE555.
- Die beiden entscheidenden Spannungen sind **1/3** und **2/3 V<sub>CC</sub>** — alles Schwingverhalten dreht sich um sie.
- Drei Modi: **astabil** (Takt), **monostabil** (Einzelpuls), **bistabil** (Latch).
- Reset (Pin 4) ist ein **Aus-Schalter**, den wir später für den `halt`-Modus brauchen.
- Für uns ist der TLC555 (CMOS, 3,3 V) besser als der klassische NE555 — kein Pegelwandler nötig.

## Übergang zur nächsten Stufe

In [Stufe 1](01-astabil.md) bauen wir den ersten astabilen Oszillator — den freilaufenden Takt, den der ESP32 später beobachten und schalten wird.
