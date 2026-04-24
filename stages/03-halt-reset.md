# Stufe 3 — Halt über Pin 4 (Reset): Vollständiger Taktgenerator in Hardware

**Ziel:** Alle drei Modi `auto` / `single` / `halt` **ohne Mikrocontroller** realisieren.
**Was du lernst:** Reset-Eingang (Pin 4) des 555 richtig nutzen, zwei 555er kombinieren, Ausgänge per Dioden-OR verbinden.
**Voraussetzung:** [Stufe 1](01-astabil.md) und [Stufe 2](02-monostabil.md)

## Theorie

Der **Reset-Eingang (Pin 4)** ist aktiv LOW:

- Pin 4 = HIGH → Baustein arbeitet normal.
- Pin 4 = LOW → Flip-Flop wird zwangsweise gelöscht, **OUT = LOW**, Entlade-Transistor zieht C gegen GND.

Damit haben wir einen **Aus-Schalter**, der den Baustein still-legt, ohne die Versorgung zu trennen. Nach Freigabe beginnt im astabilen Betrieb sofort ein neuer Zyklus — ohne Einschwing-Spielereien.

Für den Aufbau kombinieren wir zwei TLC555er:

- **555 A** astabil (aus [Stufe 1](01-astabil.md)) → liefert den freilaufenden `auto`-Takt.
- **555 B** monostabil (aus [Stufe 2](02-monostabil.md)) → liefert den `single`-Einzelpuls.

Die Mode-Auswahl entscheidet, **welcher** Baustein seinen Reset-Pin auf HIGH sieht — der jeweils andere bleibt stumm.

## Wahrheitstabelle

| Modus | 555 A Pin 4 | 555 B Pin 4 | Gemeinsamer OUT |
|-------|-------------|-------------|-----------------|
| `halt`   | LOW  | LOW  | konstant LOW |
| `auto`   | HIGH | LOW  | Rechteck von A |
| `single` | LOW  | HIGH | Einzelpuls von B, per Step-Taster |

## Schaltungen

### Ausgänge kombinieren — Dioden-OR

Zwei Ausgänge auf eine Leitung legen ist normalerweise gefährlich (ein HIGH gegen ein LOW = Kurzschluss). Weil aber immer nur **einer** aktiv ist (der andere über RESET auf LOW gezwungen), können wir die Ausgänge mit Dioden OR-verknüpfen:

```
OUT_A (Pin 3 von 555 A) ──▶|── ┐
                        1N4148  ├── OUT (gemeinsam) ── R (10 kΩ) ── GND
OUT_B (Pin 3 von 555 B) ──▶|── ┘
                        1N4148
```

Die Dioden lassen HIGH durch, der Pull-down zieht OUT auf LOW, wenn beide Ausgänge LOW sind.

> **Nebenwirkung:** die Diode fällt ca. 0,5 V ab. OUT-HIGH erreicht also nicht ganz V<sub>CC</sub>. Für LED-Anzeige und für den ESP32-Input (ab ~2 V erkennt er HIGH sicher) völlig unkritisch.

### Variante A — Mode-Auswahl per SP3T-Schalter

Einfachste Lösung: ein **dreistufiger Dreh- oder Kippschalter** (SP3T).

```
             ┌── RESET_A  (zum Pin 4 von 555 A)
VCC ─┬── Schalter ── RESET_B  (zum Pin 4 von 555 B)
     │       └── (keine Verbindung) = halt
     │
     └── (nicht verwendete Position liegt in der Luft)

RESET_A ── Pull-down 100 kΩ ── GND
RESET_B ── Pull-down 100 kΩ ── GND
```

Die **Pull-downs** sind wichtig: sie stellen sicher, dass ein „schwebender" Reset-Pin definiert LOW ist, der jeweilige Baustein also abgeschaltet bleibt.

In der **`halt`-Stellung** liegt der Schalter auf der unbenutzten Position, beide Pull-downs ziehen die Reset-Pins auf LOW → Gesamtausgang konstant LOW.

### Variante B — Drei Taster mit Latch

Eleganter, weil jede Position rastet: drei **Taster** statt eines Schalters, plus ein Element mit Gedächtnis.

Optionen für das Gedächtnis:

1. **74HC74 (dual D-FF):** zwei Flip-Flops kodieren die drei Modi (`00` = halt, `01` = auto, `10` = single). Taster setzen / resetten die FFs. Sauber, aber etwas mehr Logik.
2. **CD4017 Decade-Counter mit Reset:** jeder Taster resettet auf eine bestimmte Stellung. Ein-Chip-Lösung.
3. **Dritter TLC555 als RS-Latch:** der 555 kann als bistabiles Element verwendet werden (TRIG setzt, RESET löscht). Für zwei Zustände (an/aus) sofort brauchbar, für drei Zustände aufwändig. Im Rahmen dieses Tutorials eher ein Kuriosum.

Für einen ersten funktionierenden Aufbau reicht **Variante A** völlig — der Umbau auf Taster-gesteuert kommt ohnehin bei **Stufe 6** über den ESP32. Ein dedizierter Hardware-Latch bleibt damit eine optionale Fingerübung.

### Step-Taster im `single`-Modus

Im Single-Modus muss der Anwender den Puls **auslösen**. Wir behalten einfach den Taster aus [Stufe 2](02-monostabil.md) am TRIG-Pin von 555 B. Wichtig: der Step-Taster muss unabhängig vom Mode-Schalter funktionieren — wenn 555 B keinen Reset hat (Pin 4 LOW), wird der Trigger-Puls ohnehin ignoriert; der Taster ist harmlos.

## Aufbau

Breadboard-Layout: beide 555er **nebeneinander**, gemeinsame Masse- und V<sub>CC</sub>-Schienen, Mode-Schalter und Step-Taster sinnvoll am Rand des Boards.

1. 555 A astabil wie in [Stufe 1](01-astabil.md) (R1, R2, C). **Pin 4 nicht** mehr auf V<sub>CC</sub>, sondern an den Mode-Schalter.
2. 555 B monostabil wie in [Stufe 2](02-monostabil.md) (R, C, Trigger-Taster). **Pin 4 nicht** mehr auf V<sub>CC</sub>, sondern an den Mode-Schalter.
3. Beide Ausgänge über je eine 1N4148 auf einen gemeinsamen OUT-Knoten, dort 10-kΩ-Pull-down nach GND.
4. Eine LED + 330 Ω von OUT nach GND.
5. Zwei 100-kΩ-Pull-downs von RESET_A bzw. RESET_B nach GND.
6. SP3T-Schalter: Mittelkontakt an V<sub>CC</sub>, die drei Ausgangskontakte an RESET_A / (nichts) / RESET_B.

## Messen & Beobachten

- **`halt`:** LED aus, OUT = 0 V (Multimeter DC).
- **`auto`:** LED blinkt im Takt aus Stufe 1; Frequenz und Duty wie dort berechnet.
- **`single`:** LED aus, bis du den Step-Taster drückst — dann einmaliger Puls.
- **Moduswechsel beobachten:** von `auto` nach `halt` schaltet der Ausgang sofort ab, ohne eine halbe Periode zu Ende zu laufen. Vom Oszilloskop aus sehr schön zu erkennen.

**Bonus — Was macht der Timing-Kondensator beim `halt`?**

Bei LOW an Pin 4 zieht der Entlade-Transistor den Timing-Kondensator aktiv auf GND. Wenn du die Spannung an C misst, ist sie bei `halt` praktisch 0 V. Wechsel nach `auto` → C beginnt aus 0 V zu laden, nicht aus 1/3 V<sub>CC</sub>. Das ergibt einen minimal längeren ersten Takt, der in der Praxis aber kaum auffällt.

## Übergang zur nächsten Stufe

**Funktional sind wir am Ziel** — alle drei Modi laufen, ganz ohne Mikrocontroller. Gut zu wissen: der ESP32 wird in den folgenden Stufen nichts Grundlegend-Neues dazulernen, sondern die Steuerung **komfortabler** und **erweiterbar** machen. Zuerst einmal muss der ESP32 selbst laufen — [Stufe 4](04-esp32-einstieg.md).
