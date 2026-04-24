# Stufe 1 — Astabiler Taktgenerator

**Ziel:** Einen freilaufenden Oszillator aufbauen — der Kern des späteren `auto`-Modus.
**Was du lernst:** RC-Zeitkonstante, Duty Cycle, Frequenzformel, drei Schaltungsvarianten.
**Voraussetzung:** [Stufe 0](00-grundlagen.md)

## Theorie

Im astabilen Modus brücken wir TRIG (Pin 2) und THRES (Pin 6) und hängen sie an einen Kondensator `C`. Der Kondensator wird über `R1 + R2` aus V<sub>CC</sub> geladen und über `R2` plus Pin 7 (DISCH) entladen. Sobald die Kondensatorspannung zwischen **1/3 V<sub>CC</sub>** und **2/3 V<sub>CC</sub>** hin- und herpendelt, schwingt der Baustein von alleine.

### Die Formeln

- **Zeit HIGH:** `T_high = ln(2) · (R1 + R2) · C ≈ 0,693 · (R1 + R2) · C`
- **Zeit LOW:**  `T_low  = ln(2) · R2 · C ≈ 0,693 · R2 · C`
- **Frequenz:** `f = 1,44 / ((R1 + 2·R2) · C)`
- **Duty Cycle:** `D = (R1 + R2) / (R1 + 2·R2)`  — **immer > 50 %**

Der Faktor `0,693` ist `ln(2)` und ergibt sich aus der e-Funktion der RC-Ladekurve.

### Warum nie unter 50 % Duty?

Weil der Ladeweg **R1 + R2** länger ist als der Entladeweg **R2 allein**. Für genau 50 % Duty müssen wir die beiden Wege trennen — das macht die Dioden-Variante unten.

## Schaltungen

### Variante A — Klassisch

Die Standardbeschaltung. Duty liegt je nach R1/R2 zwischen ~51 % (R1 ≪ R2) und ~99 % (R1 ≫ R2).

**Verdrahtung:**

| TLC555 Pin | Verbindung |
|------------|------------|
| 1 (GND)    | Masse |
| 2 (TRIG)   | Brücke zu Pin 6 |
| 3 (OUT)    | LED + 330 Ω → Masse |
| 4 (RESET)  | V<sub>CC</sub> (ausser Betrieb) |
| 5 (CTRL)   | 100 nF → Masse |
| 6 (THRES)  | Brücke zu Pin 2 **+** C → Masse |
| 7 (DISCH)  | zwischen R1 (nach V<sub>CC</sub>) und R2 (nach Pin 6) |
| 8 (V<sub>CC</sub>) | +3,3 V |

Kurz als „Zweig":

```
VCC ── R1 ── Pin 7 ── R2 ── Pin 6 ── C ── GND
                                    │
                                   Pin 2  (Brücke)
```

**Startwerte für gut sichtbaren Blink (≈ 1,4 Hz):**

- R1 = 10 kΩ
- R2 = 47 kΩ
- C  = 10 µF (Elko, Pluspol an Pin 6)

Probe: `f = 1,44 / ((10k + 2·47k) · 10µF) = 1,44 / 1,04 s ≈ 1,38 Hz`.
Das sind knapp anderthalb Blinks pro Sekunde, Duty ca. 55 %.

### Variante B — Dioden-Trick für ≈ 50 % Duty

Eine **1N4148** parallel zu R2, Kathode Richtung Pin 7 (DISCH). Beim **Laden** fließt der Strom durch die Diode an R2 vorbei → Ladeweg ist nur R1. Beim **Entladen** sperrt die Diode → Entladung weiter über R2.

Dadurch:

- `T_high ≈ 0,693 · R1 · C`
- `T_low  ≈ 0,693 · R2 · C`
- Für **R1 = R2** → ≈ 50 % Duty Cycle.

Der Ausgangspuls ist jetzt symmetrisch — wichtig für viele digitale Anwendungen und für ein hörbar sauberes Signal.

### Variante C — Variable Frequenz mit Poti

Ersetze R2 durch ein **100 kΩ-Poti in Reihe mit einem kleinen Schutzwiderstand** (z. B. 1 kΩ). Der Schutzwiderstand verhindert, dass R2 in der Nullstellung 0 Ω wird — das würde Pin 7 beim Entladen ungebremst gegen Masse ziehen und den Entlade-Transistor belasten.

Mit R1 = 10 kΩ, C = 10 µF, Poti 1 k … 101 kΩ ergibt sich:

| Poti-Stellung | R2 effektiv | Frequenz |
|--------------:|------------:|---------:|
| ganz links    |   1 kΩ      | ≈ 12 Hz  |
| Mitte         |  50 kΩ      | ≈ 1,3 Hz |
| ganz rechts   | 101 kΩ      | ≈ 0,68 Hz |

Ein schöner, sichtbarer Bereich zum Spielen.

## Aufbau auf dem Breadboard

1. TLC555 mittig ins Breadboard stecken, Pin 1 oben-links markieren.
2. Versorgungsschienen verkabeln: eine Seite **+3,3 V**, andere Seite **GND**.
3. **Immer zuerst die zwei Pflicht-Kondensatoren**:
   - **100 nF** direkt zwischen V<sub>CC</sub> (Pin 8) und GND (Pin 1) — Versorgungsentkopplung.
   - **100 nF** von Pin 5 (CTRL) nach GND — Störunterdrückung der internen Schwellen.
4. Pin 4 (RESET) fest auf V<sub>CC</sub> legen. Wenn du das vergisst, schwingt gar nichts.
5. Timing-Zweig `R1 / R2 / C` aufbauen, Pins 2 und 6 brücken.
6. LED + 330-Ω-Vorwiderstand zwischen Pin 3 und GND.

> **Häufige Fehler:**
> - Pin 4 offen gelassen → Ausgang bleibt LOW.
> - Pin 5 ohne Kondensator → Takt jittert, vor allem bei niedriger Versorgung.
> - Elko falsch gepolt → explodiert bei 3,3 V zwar nicht, verfälscht aber die Zeit deutlich.

## Messen & Beobachten

### Mit bloßem Auge

Bei ~1 Hz blinkt die LED sichtbar. Vergrößere C (oder R2), zähle die Blinks in 10 Sekunden, rechne die Frequenz aus — und vergleiche mit der Formel.

### Mit dem Multimeter

Ab ~50 Hz erscheint die LED durchgehend an. Im **Hz-Modus** direkt an Pin 3 misst das Multimeter die Frequenz.

**Bonus — Mittelspannung:** im **DC-V**-Modus zeigt das Multimeter an Pin 3 den zeitlichen Mittelwert:

`V_mittel ≈ V_CC · Duty`

Daraus Duty zurückrechnen und gegen die Formel prüfen.

### Mit dem Oszilloskop (optional, aber lehrreich)

Klemme den Tastkopf parallel zu C (Pin 6 nach GND). Du siehst die **Sägezahn-Ladekurve** zwischen 1/3 und 2/3 V<sub>CC</sub>. Parallel dazu den Ausgang (Pin 3) beobachten — das Umschalten passiert genau an den Schwellwerten. Das Aha-Bild zum 555.

## Übergang zur nächsten Stufe

Der Oszillator läuft **ununterbrochen**. Für einen `single-step`-Modus brauchen wir etwas, das **genau einen** Puls pro Knopfdruck liefert. Dazu nutzen wir denselben TLC555 in einer anderen Beschaltung — den **monostabilen Modus**, [Stufe 2](02-monostabil.md).
