# ESP32-NE555-Tutorial

Anfänger-Tutorial: **Taktgenerator mit den Modi `auto` / `single-step` / `halt`**.
Schrittweiser Aufbau vom reinen 555-Timer-Aufbau bis zur ESP32-gesteuerten Variante.

## Zielgruppe

- Breadboard-Erfahrung vorhanden
- Ohm'sches Gesetz sitzt
- Erste Berührungspunkte mit Mikrocontrollern genügen (kein ESP32-Vorwissen nötig)

## Warum TLC555 / LMC555 statt NE555?

Wir verwenden den **CMOS-Typ (TLC555 oder LMC555)** statt des bipolaren NE555:

- Läuft bei **3,3 V** — direkt mit dem ESP32 koppelbar, ohne Spannungsteiler oder Level-Shifter.
- Sehr geringer Eingangsstrom — unproblematischer mit hohen Widerständen.
- Pin-kompatibel zum NE555 — alle Schaltungen sind übertragbar.

Der klassische NE555 wird in einem Kurz-Exkurs erwähnt (Unterschiede beim Lesen eines Datenblatts, wann man ihn doch wählt).

## Didaktisches Muster (jede Stufe)

1. **Ziel & was du lernst**
2. **Theorie** — gerade so viel wie nötig
3. **Schaltung** — Grundvariante + 1–2 Varianten mit Trade-offs
4. **Aufbau** auf dem Breadboard
5. **Messen & Beobachten** (LED / Multimeter / ggf. Oszi)
6. **Übergang** — was fehlt noch, was kommt als Nächstes

## Stufenplan

| Stufe | Thema | Ergebnis |
|-------|-------|----------|
| [0](stages/00-grundlagen.md) | Grundlagen TLC555 | Pinout & Modi verstanden |
| [1](stages/01-astabil.md) | Astabiler Taktgenerator | Blinkende LED, einstellbare Frequenz |
| [2](stages/02-monostabil.md) | Monostabil als Single-Step-Baustein | One-Shot per Tastendruck |
| [3](stages/03-halt-reset.md) | Halt über Pin 4 (Reset) | **Vollständiger Taktgenerator rein in Hardware** |
| [4](stages/04-esp32-einstieg.md) | ESP32 Einstieg | Toolchain, Blink, GPIO |
| [5](stages/05-esp32-beobachtet.md) | ESP32 misst den Takt | Frequenzanzeige via Serial Monitor |
| [6](stages/06-esp32-steuert.md) | ESP32 steuert den Taktgenerator | **Ziel: Mode-Controller in Software** |
| [7](stages/07-ausbau.md) | Ausbau (optional) | OLED, Poti, Web-UI |
| [Anhang](stages/anhang-wokwi.md) | Wokwi-Simulator | ESP32-Sketches ohne Hardware testen |

**Roter Faden:** Stufe 3 zeigt, dass der Mikrocontroller Luxus ist, keine Notwendigkeit. Danach wird er schrittweise hinzugenommen — erst als Beobachter (Stufe 5), dann als Steuerung (Stufe 6).

## Toolchain

- **Arduino IDE** (einsteigerfreundlicher als PlatformIO)
- ESP32-DevKit V1 oder vergleichbares Board
- Standard-Breadboard + Steckbrückenkabel
- Multimeter (Pflicht), Oszilloskop / Logic-Analyzer (optional, nice-to-have)
- **Optional: Wokwi-Simulator** als Software-Ersatz fürs Breadboard — Setup in [stages/anhang-wokwi.md](stages/anhang-wokwi.md). Jeder Sketch in `code/stageXX_*/` bringt bereits passende `diagram.json` + `wokwi.toml` mit.

## Stückliste

Siehe [parts-list.md](parts-list.md).

## Projektstruktur

```
ESP32-NE555-Tutorial/
├── README.md                 ← dieses Dokument
├── parts-list.md             ← Stückliste
├── stages/                   ← eine Datei pro Stufe + Anhang
│   ├── 00-grundlagen.md
│   ├── ...
│   ├── 07-ausbau.md
│   └── anhang-wokwi.md
├── code/                     ← Arduino-Sketches ab Stufe 4 (je Sketch ein Ordner mit
│                                .ino, diagram.json, wokwi.toml)
└── assets/                   ← Bilder, nach Stufen gegliedert
    ├── stage-04/
    │   ├── wokwi-blink.jpg
    │   └── wokwi-button.jpg
    ├── stage-05/
    ├── stage-06/
    └── stage-07/
```

Präfix `wokwi-` in den Bildnamen reserviert Platz für spätere `aufbau-*.jpg` (echte Hardware-Fotos) und `falstad-*.png` (Schaltungssimulationen für die Stufen 0–3).

## Wie du dieses Tutorial liest

- **Jede Stufe hat denselben Aufbau:** kurzer Ziel-Block, dann **„Was bauen wir?"**, Theorie, Schaltung, Aufbau, Messen, **Checkpoints**, Troubleshooting, **Rückblick**, Übergang zur nächsten Stufe.
- **Baue mit.** Lesen allein bringt nichts — ein Breadboard vor dir und ein Multimeter daneben ist der Kern des Tutorials.
- **Checkpoints** sind kleine Stopps, an denen du etwas sehen oder messen solltest. Wenn dort nichts passt, lohnt es sich, zurückzugehen, bevor du weiterbaust.
- **Troubleshooting-Tabellen** listen die häufigsten Fehlerbilder. Die meisten Probleme im 555-Umfeld sind Klassiker — du bist damit nicht allein.
- **Code-Skizzen liegen in `code/`** und lassen sich direkt in der Arduino-IDE öffnen.

### Hinweis zu Fotos und Schaltplänen

Ab Stufe 4 liegen in `assets/stage-0X/` **Wokwi-Screenshots** der fertigen Simulationen. **Echte Aufbau-Fotos** für die Hardware-Stufen 0–3 und -3 folgen, sobald die Stufen in der Werkstatt aufgebaut werden — echte Bilder sind der letzte Schritt, nicht der erste.
