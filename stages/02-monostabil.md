# Stufe 2 — Monostabil als Single-Step-Baustein

**Ziel:** Einen definierten Einzelpuls pro Tastendruck erzeugen — Basis des späteren `single-step`-Modus.
**Was du lernst:** Trigger-Verhalten (aktiv LOW), Pulsbreite `T ≈ 1,1 · R · C`, Tasterentprellung.
**Voraussetzung:** [Stufe 0](00-grundlagen.md); hilfreich [Stufe 1](01-astabil.md)
**Dauer:** ca. 30 Minuten

## Was bauen wir?

Einen **Taster + LED**, bei dem die LED für eine genau bestimmte Zeit (z. B. eine halbe Sekunde) aufleuchtet — unabhängig davon, wie lange du den Taster drückst. Das ist ein „One-Shot": ein Puls pro Knopfdruck. Aus diesem Baustein wird später im `single-step`-Modus der Einzeltakt.

## Theorie

### Kurz und intuitiv

Im Ruhezustand liegt der Kondensator C auf Masse (entladen). Ein kurzer Tastendruck **kippt** den 555 — OUT geht HIGH, und C beginnt über R zu laden. Sobald C die Schwelle 2/3 V<sub>CC</sub> erreicht, kippt der 555 zurück: OUT geht LOW, C wird wieder auf Masse gezogen. Die Pulslänge hängt also ausschließlich von R und C ab, **nicht** von der Tastendruck-Dauer.

**Daumenregel:** größerer Widerstand oder größere Kapazität → längerer Puls.

### Formell genauer

Im monostabilen Modus hat der 555 einen **Ruhezustand** (OUT = LOW, Kondensator leer) und einen **aktiven Zustand** (OUT = HIGH, Kondensator lädt). Ein externer **LOW-Impuls** an TRIG (Pin 2) kippt den Baustein aus dem Ruhezustand; er kehrt nach einer festen Zeit `T` von alleine zurück.

### Ablauf

1. **Ruhe:** TRIG liegt auf V<sub>CC</sub>, Flip-Flop gelöscht, Pin 7 (DISCH) zieht C gegen Masse.
2. **Trigger:** TRIG fällt unter 1/3 V<sub>CC</sub> → unterer Komparator setzt das Flip-Flop → OUT **HIGH**, Entlade-Transistor **öffnet** → C beginnt über R zu laden.
3. **Schwelle erreicht:** Sobald die Kondensatorspannung über 2/3 V<sub>CC</sub> steigt → oberer Komparator setzt das Flip-Flop zurück → OUT **LOW**, Entlade-Transistor zieht C wieder gegen Masse.

### Pulsbreite

`T = ln(3) · R · C ≈ 1,098 · R · C`

In der Praxis rechnet man mit **`T ≈ 1,1 · R · C`**.

### Wichtig: nicht-retriggerbar

Während der Puls läuft (OUT = HIGH), wird ein weiterer Trigger **ignoriert**. Das ist perfekt für Taster: typisches Prellen im Millisekundenbereich kann dem Baustein nichts anhaben, solange `T` deutlich länger ist als die Prellzeit.

## Schaltungen

### Variante A — Grundschaltung mit Taster

**Verdrahtung:**

| TLC555 Pin | Verbindung |
|------------|------------|
| 1 (GND)    | Masse |
| 2 (TRIG)   | Pull-up 10 kΩ nach V<sub>CC</sub>; Taster nach GND |
| 3 (OUT)    | LED + 330 Ω → Masse |
| 4 (RESET)  | V<sub>CC</sub> (ausser Betrieb) |
| 5 (CTRL)   | 100 nF → Masse |
| 6 (THRES)  | Brücke zu Pin 7 **+** C → Masse |
| 7 (DISCH)  | Brücke zu Pin 6; R nach V<sub>CC</sub> |
| 8 (V<sub>CC</sub>) | +3,3 V |

Als Zweig:

```
VCC ── R ── Pin 7 ═══ Pin 6 ── C ── GND
                                          Pin 2 ── Pull-up 10k ── VCC
                                                 └── Taster ── GND
```

**Startwerte für einen gut sichtbaren Puls (≈ 0,5 s):**

- R = 470 kΩ
- C = 1 µF (Elko, Pluspol zu Pin 6)
- → `T = 1,1 · 470k · 1µF ≈ 517 ms`

### Variante B — Kondensator am Trigger-Eingang

Wenn der Taster **gedrückt bleibt**, liegt TRIG dauerhaft auf LOW. Das verhindert das Zurückkehren in den Ruhezustand: der Puls wird so lange verlängert, wie der Taster unten bleibt. Bei schnellem Drücken nicht relevant, bei längerem Halten schon.

Abhilfe: ein kleiner Kondensator (z. B. **10 nF**) in Reihe zum Taster. Der Taster erzeugt damit nur noch eine **kurze Flanke** am TRIG-Pin — genug zum Auslösen, nicht genug zum Blockieren.

```
Pin 2 ── Pull-up 10k ── VCC
      └── 10 nF ── Taster ── GND
```

Der Pull-up schließt den Kondensator nach Loslassen wieder auf V<sub>CC</sub>, bereit für den nächsten Trigger.

### Variante C — Logik-Trigger statt Taster

Der Trigger-Eingang lässt sich auch direkt mit einem Logikpegel steuern — fallende Flanke löst aus. Das nutzen wir in [Stufe 6](06-esp32-steuert.md), wenn der ESP32 den Single-Step-Puls auslöst.

## Aufbau

1. TLC555 wie in Stufe 1 auf dem Breadboard.
2. Versorgungsschienen +3,3 V / GND mit den zwei 100-nF-Pflicht-Kondensatoren (V<sub>CC</sub>-Entkopplung und Pin 5 nach GND).
3. Pin 4 (RESET) auf V<sub>CC</sub>.
4. **Umbau gegenüber Stufe 1:**
   - Pin 2 und Pin 6 **nicht** mehr brücken.
   - Stattdessen Pins 6 und 7 brücken (Timing-Knoten).
   - R nur noch **ein** Widerstand von V<sub>CC</sub> zum Timing-Knoten.
   - Pin 2 über Pull-up an V<sub>CC</sub>; Taster von Pin 2 nach GND.
5. LED + 330 Ω von Pin 3 nach GND.

> **Checkpoint:** Versorgung ein. Taster drücken → LED leuchtet etwa 0,5 s auf und geht von alleine aus. Wenn ja: weiter. Wenn LED gar nicht angeht oder dauerhaft an bleibt: Troubleshooting unten.

## Troubleshooting

| Symptom | Mögliche Ursache |
|---------|------------------|
| LED reagiert nicht auf Tastendruck | Pin 2 hat keinen Pull-up → Trigger-Flanke kommt nie sauber zustande. |
| LED bleibt dauerhaft an | Pin 2 bleibt LOW (Taster klemmt, Pull-up fehlt). Oder Pin 4 (RESET) ist nicht auf V<sub>CC</sub>. |
| Puls ist viel zu kurz / viel zu lang | R oder C falsch dimensioniert; Elko vertauscht (+/−). |
| Beim Halten des Tasters bleibt LED länger an | Erwartetes Verhalten in Variante A. Für feste Pulslänge auf Variante B wechseln (10 nF in Reihe zum Taster). |
| Zweiter Tastendruck während des Pulses verlängert nichts | Das ist korrekt — der Baustein ist non-retriggerbar. |

## Messen & Beobachten

- **Taster kurz drücken:** LED leuchtet für ~0,5 s auf.
- **Taster während des Pulses erneut drücken:** LED bleibt einfach an, zweiter Trigger wird ignoriert (Non-Retriggerbarkeit).
- **Taster langer als 0,5 s halten (Variante A, ohne Trigger-Cap):** Puls verlängert sich → das Loslassen beendet ihn, nicht die Zeit.
- **Variante B mit 10 nF in Reihe:** Puls ist fest ~0,5 s, unabhängig von der Druckdauer.

**Mit Multimeter:**

Im **DC-V**-Modus an Pin 3 zeigt der Mittelwert, wie oft du pro Zeiteinheit gedrückt hast. Bei zehn Drücken in 10 s à 0,5 s Puls: `V_mittel ≈ 0,5 · V_CC`.

**Mit Oszilloskop:**

Tastkopf an Pin 6. Du siehst eine schöne RC-Ladekurve, die bei 2/3 V<sub>CC</sub> abbricht und senkrecht auf 0 V zurückfällt — genau das, was die Theorie vorhersagt.

## Rückblick

Was du jetzt kannst:

- Einen **monostabilen 555** als One-Shot-Pulsgenerator aufbauen.
- Die **Pulslänge** gezielt über R und C einstellen.
- Das **Trigger-Verhalten** nutzen (aktiv LOW, non-retriggerbar).
- Den Taster **entprellen**, ohne eine Software zu schreiben (entweder über lange Pulszeit oder über kapazitive AC-Kopplung).

## Übergang zur nächsten Stufe

Jetzt haben wir **Dauerlauf (Stufe 1)** und **Einzelpuls (Stufe 2)**. Was fehlt zum vollständigen Auto/Single/Halt-Taktgenerator? Ein sauberes **Halt** — und eine Mechanik, die zwischen den Modi umschaltet. Genau das machen wir in [Stufe 3](03-halt-reset.md), **ohne Mikrocontroller**.
