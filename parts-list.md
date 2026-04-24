# Stückliste

> Vollständige Bauteilliste für alle Stufen. Preise nur grobe Richtwerte.

## Aktive Bauteile

| Bauteil | Menge | Hinweis |
|---------|-------|---------|
| TLC555 oder LMC555 (DIP-8) | 2 | CMOS-555, läuft bei 3,3 V |
| ESP32-DevKit V1 (oder vergleichbar) | 1 | ab Stufe 4 |
| Standard-LED (rot) | 3–5 | Anzeige-LEDs |
| Widerstand 330 Ω | 3–5 | Vorwiderstand LEDs |
| 1N4148 Diode | 1 | für Stufe 1 Variante B (~50 % Duty) |

## Passive Bauteile

| Bauteil | Menge | Hinweis |
|---------|-------|---------|
| Widerstand 1 kΩ | 5 | |
| Widerstand 10 kΩ | 5 | |
| Widerstand 100 kΩ | 5 | |
| Widerstand 1 MΩ | 2 | für sehr langsame Takte |
| Potentiometer 100 kΩ (linear) | 1 | variable Frequenz |
| Keramik-Kondensator 100 nF | 5 | Entkopplung + Pin-5-Bypass |
| Keramik-Kondensator 10 nF | 2 | |
| Elektrolyt-Kondensator 10 µF | 2 | langsame Takte |
| Elektrolyt-Kondensator 1 µF | 2 | |
| Elektrolyt-Kondensator 100 µF | 1 | Versorgung |

## Schalter & Taster

| Bauteil | Menge | Hinweis |
|---------|-------|---------|
| Taster (Momentary, Breadboard-tauglich) | 4 | Trigger, Mode-Auswahl |
| Schiebeschalter oder DIP-Schalter | 1 | Halt-Mechanismus Stufe 3 |

## Werkzeug & Zubehör

| Teil | Hinweis |
|------|---------|
| Breadboard (830er) | Platz für mehrstufigen Aufbau |
| Steckbrückenkabel-Set | M-M und M-F |
| USB-Kabel zum ESP32 | Typ passend zum Board |
| Multimeter | Pflicht |
| Oszilloskop oder Logic-Analyzer | Optional, stark empfohlen ab Stufe 1 |
| Labornetzteil oder 5 V / 3,3 V Steckernetzteil | Für 555-only-Aufbau (Stufen 1–3) |

## Optional (Stufe 7)

| Bauteil | Hinweis |
|---------|---------|
| OLED 0,96" I²C (SSD1306) | Statusanzeige |
| Digital-Potentiometer (z. B. MCP41010) | Software-steuerbare Frequenz |
| Jumper-Kabel zusätzlich | |
