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

**Roter Faden:** Stufe 3 zeigt, dass der Mikrocontroller Luxus ist, keine Notwendigkeit. Danach wird er schrittweise hinzugenommen — erst als Beobachter (Stufe 5), dann als Steuerung (Stufe 6).

## Toolchain

- **Arduino IDE** (einsteigerfreundlicher als PlatformIO)
- ESP32-DevKit V1 oder vergleichbares Board
- Standard-Breadboard + Steckbrückenkabel
- Multimeter (Pflicht), Oszilloskop / Logic-Analyzer (optional, nice-to-have)

## Stückliste

Siehe [parts-list.md](parts-list.md).

## Projektstruktur

```
ESP32-NE555-Tutorial/
├── README.md                 ← dieses Dokument
├── parts-list.md             ← Stückliste
└── stages/                   ← eine Datei pro Stufe
    ├── 00-grundlagen.md
    ├── 01-astabil.md
    ├── ...
    └── 07-ausbau.md
```

Schaltpläne, Code und Fotos kommen später in eigene Unterordner (`schematics/`, `code/`, `assets/`), sobald wir sie erzeugen.
