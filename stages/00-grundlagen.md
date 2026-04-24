# Stufe 0 — Grundlagen TLC555

**Ziel:** Den 555-Timer verstehen, bevor wir ihn verschalten.
**Was du lernst:** Pinbelegung, interne Funktion (vereinfacht), die drei Grundmodi.
**Voraussetzung:** keine

## Theorie

_TODO:_ Pinout TLC555 (DIP-8), intern: zwei Komparatoren + RS-Flip-Flop + Ausgangsstufe, stark vereinfacht. Schwellen bei 1/3 V<sub>CC</sub> und 2/3 V<sub>CC</sub>.

### Die drei Betriebsarten

- **Astabil** — dauerhafter Oszillator (Stufe 1)
- **Monostabil** — ein Puls auf Knopfdruck (Stufe 2)
- **Bistabil** — als RS-Flip-Flop verwendet (am Rande, Stufe 2/3)

## Unterschied TLC555 / LMC555 / NE555

_TODO:_ Kurzexkurs — Versorgungsbereich, CMOS vs bipolar, Ausgangsstrom, wann welcher.

## Übergang zur nächsten Stufe

Wir bauen den ersten astabilen Oszillator.
