# Stufe 5 — ESP32 beobachtet den Taktgenerator

**Ziel:** Die Frequenz des TLC555 aus Stufe 1 mit dem ESP32 messen und im Serial Monitor anzeigen.
**Was du lernst:** Hardware-Interrupts (`attachInterrupt`), Flankenerkennung, `volatile`, `IRAM_ATTR`, kritische Abschnitte.
**Voraussetzung:** [Stufe 1](01-astabil.md) und [Stufe 4](04-esp32-einstieg.md)
**Dauer:** ca. 20–30 Minuten

## Was bauen wir?

Eine **lebende Frequenzanzeige** auf dem Serial Monitor. Der TLC555 aus Stufe 1 blinkt fröhlich vor sich hin, wir hängen zusätzlich eine Leitung an seinen Ausgang und führen sie zum ESP32. Der ESP32 zählt die Flanken und gibt sekündlich eine Zahl aus:

```
f = 1.38 Hz
f = 1.37 Hz
f = 2.84 Hz   ← jetzt hast du am Poti gedreht
```

Wichtig: der ESP32 **greift nicht ein**. Er schaut nur zu. Das ist der natürliche Zwischenschritt vor Stufe 6.

## Konzept

- Der TLC555 läuft als astabiler Oszillator aus Stufe 1 — autark auf seiner Breadboard-Seite.
- Sein Ausgang (Pin 3) wird parallel an einen GPIO des ESP32 geführt.
- Ein **Hardware-Interrupt** am ESP32 zählt jede steigende Flanke.
- Alle 1000 ms liest `loop()` den Zähler aus und gibt die Frequenz auf der seriellen Konsole aus.

Mikrocontroller **beobachtet** nur — er greift nicht in den 555 ein. Der Reset-Pin bleibt auf V<sub>CC</sub>, die Versorgung läuft wie in Stufe 1.

## Schaltung

**Minimale Ergänzung gegenüber Stufe 1:**

| TLC555 Pin | Verbindung | ESP32-Pin |
|------------|------------|-----------|
| 3 (OUT)    | Tastkopf-Draht | **GPIO 34** |
| 1 (GND)    | gemeinsame Masse | **GND** |

Alles andere (R1, R2, C, Pull-up auf Pin 4, CTRL-Cap) bleibt wie in Stufe 1.

**Wichtig:** der TLC555 und der ESP32 brauchen **gemeinsame Masse**. Wenn sie aus getrennten Quellen laufen (Labor-NT für den 555, USB für den ESP32), verbinde die GND-Schienen.

### Warum GPIO 34?

GPIO 34 ist **nur-Eingang** ohne internen Pull-up/down. Da der TLC555 den Pegel aktiv treibt, genügt ein reiner Input. Nebeneffekt: der Pin ist strapping-unkritisch und lässt sich nicht versehentlich als Output-Kurzschluss konfigurieren.

Alternativ gehen auch GPIO 32 oder 33; dann aber `INPUT` statt `INPUT_PULLUP` verwenden.

## Code

Vollständig in [`code/stage05_freqmeter/stage05_freqmeter.ino`](../code/stage05_freqmeter/stage05_freqmeter.ino).

### Das Wesentliche

```cpp
const uint8_t FREQ_PIN   = 34;
const uint32_t SAMPLE_MS = 1000;

volatile uint32_t edgeCount = 0;

void IRAM_ATTR onRisingEdge() {
  edgeCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(FREQ_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(FREQ_PIN), onRisingEdge, RISING);
}
```

Wichtige Details für den Einsteiger:

- **`volatile`** auf `edgeCount`: sagt dem Compiler, dass die Variable von außen (im Interrupt) geändert wird — er darf Zugriffe nicht wegoptimieren.
- **`IRAM_ATTR`** auf der ISR: beim ESP32 muss Interrupt-Code im internen RAM liegen, nicht im Flash — diese Annotation sorgt dafür.
- **ISR so kurz wie möglich:** nur zählen, keine `Serial.print`-Aufrufe, keine `delay`, keine Gleitkomma-Arithmetik.

### Frequenzberechnung

```cpp
void loop() {
  uint32_t now = millis();
  if (now - lastSampleMs >= SAMPLE_MS) {
    noInterrupts();
    uint32_t count = edgeCount;
    edgeCount = 0;
    interrupts();

    uint32_t elapsedMs = now - lastSampleMs;
    lastSampleMs = now;

    float freqHz = (count * 1000.0f) / elapsedMs;
    Serial.print("f = ");
    Serial.print(freqHz, 2);
    Serial.println(" Hz");
  }
}
```

Der Block `noInterrupts() … interrupts()` sorgt dafür, dass das Auslesen und Zurücksetzen atomar passiert — sonst könnte eine Flanke genau zwischen den beiden Operationen verlorengehen.

## Messen & Beobachten

1. Serial Monitor (115200 Baud) öffnen.
2. TLC555 aus Stufe 1 einschalten.
3. Sekündlich erscheint die Frequenz.

**Poti drehen (Variante C aus Stufe 1):** die Anzeige folgt in Echtzeit.
**Gegenprobe:** Multimeter im Hz-Modus parallel an Pin 3 — beide Werte sollten sich auf < 1 Hz decken (Multimeter-Abtastung vorausgesetzt).

> **Checkpoint:** Sekündlich erscheint im Serial Monitor eine Frequenz nahe dem Wert, den du aus Stufe 1 berechnet hast (bei Startwerten R1=10k, R2=47k, C=10µF ≈ 1,4 Hz). Am Poti drehen → Anzeige folgt in wenigen Sekunden.

## Troubleshooting

| Symptom | Mögliche Ursache |
|---------|------------------|
| Serial Monitor zeigt dauerhaft `f = 0.00 Hz` | Signalleitung nicht verbunden, oder ESP32 und 555 haben **keine gemeinsame Masse**. |
| Anzeige stark ungleichmäßig (springt) | Schlechte Masseverbindung, lange fliegende Drähte — kurze Jumper nehmen. |
| Anzeige ≈ doppelt so hoch wie erwartet | ISR zählt beide Flanken statt nur steigender → `RISING` im `attachInterrupt` prüfen. |
| Anzeige ≈ halb so hoch wie erwartet | Multimeter-Gegenprobe erst durchführen — eventuell stimmte deine Erwartung nicht (Formelrechnung noch mal prüfen). |
| Board stürzt ab oder rebootet laufend | `IRAM_ATTR` vor der ISR vergessen, oder in der ISR wird `Serial.print`/`delay`/`float`-Rechnung gemacht. |

### Grenzen der Messung

| Frequenzbereich | Empfehlung |
|-----------------|------------|
| < 1 Hz | Abtastintervall auf 5–10 s erhöhen, sonst Quantisierung grob |
| 1 Hz – 100 kHz | läuft problemlos |
| 100 kHz – 1 MHz | Zählung funktioniert noch, ISR-Overhead wird spürbar |
| > 1 MHz | besser ESP32-PCNT (Pulse Counter) nutzen — Hardware-Zähler ohne ISR-Overhead. Nicht Teil dieses Tutorials. |

## Diskussion

- **Warum Interrupts, nicht Polling?** Ein Polling mit `digitalRead()` im `loop()` würde schnelle Flanken verpassen. Der Interrupt bindet jeden Edge garantiert ein.
- **Warum `float` in der Berechnung?** Damit die Frequenzanzeige auch Bruchteile (0,7 Hz) zeigt. Für ganze Hertz würde `uint32_t` reichen.

## Rückblick

Was du jetzt kannst:

- Einen **ESP32-GPIO an ein externes Signal** anschließen (inklusive gemeinsamer Masse).
- **Hardware-Interrupts** einrichten (`attachInterrupt`, `RISING`, `IRAM_ATTR`).
- Flanken **atomar** auslesen (`noInterrupts` / `interrupts`).
- Eine **Frequenz aus einem Zählwert** berechnen und anzeigen.
- Die Grenzen einer einfachen Zählung einschätzen (zu niedrig / zu hoch, wann man zum PCNT greift).

## Übergang zur nächsten Stufe

Jetzt **weiß** der ESP32, was der 555 tut. In [Stufe 6](06-esp32-steuert.md) sagen wir ihm umgekehrt, **was er tun soll** — der ESP32 wird zum Mode-Controller und übernimmt die Rolle des Schalters aus Stufe 3.
