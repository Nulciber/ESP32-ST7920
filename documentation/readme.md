# Projet ESP32-S3 — Affichage ST7920
# Test test
Driver graphique bas-niveau en C++ pour piloter un écran 
SBC-LCD128x64 (contrôleur ST7920) depuis un ESP32-S3, 
sans bibliothèque externe — juste pour apprendre et s'amuser.

## Matériel

| Composant | Modèle |
|-----------|--------|
| Microcontrôleur | ESP32-S3-DevKitC-1-N8R8 |
| Écran | SBC-LCD128x64 (ST7920) |
| Interface | SPI hardware — SPI_MODE3 |

## Ce que fait ce projet

- Initialisation du ST7920 en mode graphique (GDRAM)
- Allumage/extinction de pixels individuels
- Tracé de lignes, rectangles, cercles (algorithme de Bresenham)
- Affichage de texte avec police 8x8 personnalisée
- Support des caractères accentués (é, è, ê, à, ù, ç...)

## Structure

- `src/` — code source Arduino C++
- `docs/` — documentation technique

## Câblage

| Broche LCD | GPIO ESP32-S3 |
|------------|---------------|
| RS  (CS)   | GPIO 10       |
| R/W (MOSI) | GPIO 11       |
| E   (SCK)  | GPIO 12       |
| PSB        | GND           |
| VCC        | 5V            |

## Notes

Projet d'apprentissage — le code est volontairement bas-niveau 
et abondamment commenté dans `docs/`.