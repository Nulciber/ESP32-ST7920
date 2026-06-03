# Pour envoyer de l'information depuis l'ESP32 à l'écran (protocole ST7920)

## La fonction sendCommand()

```cpp
void sendCommand(uint8_t cmd)
{
    digitalWrite(CS, HIGH);   // Préparer le ST7920 à recevoir de l'information
    SPI.transfer(0xF8);       // Ce que tu vas recevoir est une commande
    SPI.transfer(cmd & 0xF0); // Voici le premier nibble de l'octet que je t'envoie
    SPI.transfer(cmd << 4);   // Voici le second nibble de l'octet que je t'envoie
    digitalWrite(CS, LOW);    // Le transfert d'information est terminé
    delayMicroseconds(72);    // J'attends que tu aies fini de traiter la commande selon les spécifications de la datasheet
}
```

## L'octet de synchronisation — `SPI.transfer(0xF8)`

Le premier octet envoyé au ST7920 lui indique ce qui va suivre. Il est composé de 8 bits :

| synchronisation | R/W | RS | Padding |
|---|---|---|---|
| 1  1  1  1  1 | 0 | 0 | 0 |
| 11111 indique qu'une information arrive | 0=écrire vers / 1=lire depuis | 0=commande / 1=donnée | remplissage |

**R/W** — Read/Write : en mode série, la lecture est impossible, on écrit toujours → toujours `0`.

**RS** — Register Select : `0` = on envoie une commande, `1` = on envoie une donnée.

**Padding** : remplissage pour arriver à 8 bits. On remplit avec des zéros.

Donc :
- `0xF8 = 11111000` : RS = 0 → envoi d'une **commande**
- `0xFA = 11111010` : RS = 1 → envoi d'une **donnée**

## Envoi de la commande en deux nibbles

Le ST7920 reçoit l'octet de la commande en deux nibbles (4 bits) — c'est une exigence expliquée dans le datasheet. Il faut donc lui envoyer deux fois un octet dont il ne lit que les 4 MSB, les 4 LSB devant être à 0.

```cpp
SPI.transfer(cmd & 0xF0); // Voici le premier nibble de l'octet que je t'envoie
SPI.transfer(cmd << 4);   // Voici le second nibble de l'octet que je t'envoie
```

### Exemple : commande `0x36` (Mode graphique) — `0x36 = 00110110`

**Premier nibble** — `cmd & 0xF0` :

| | b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 |
|---|---|---|---|---|---|---|---|---|
| `0x36` | 0 | 0 | 1 | 1 | 0 | 1 | 1 | 0 |
| `0xF0` | 1 | 1 | 1 | 1 | 0 | 0 | 0 | 0 |
| `AND`  | 0 | 0 | 1 | 1 | 0 | 0 | 0 | 0 |
| | *Premier nibble (MSB)* | | | | *Pas pris en compte (LSB)* | | | |

**Second nibble** — `cmd << 4` :

| | b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 |
|---|---|---|---|---|---|---|---|---|
| `0x36` | 0 | 0 | 1 | 1 | 0 | 1 | 1 | 0 |
| `<< 4` | 0 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| | *Second nibble (MSB)* | | | | *Pas pris en compte (LSB)* | | | |

**Résultat transmis** — le ST7920 recolle les deux nibbles et retrouve l'octet original :

| | b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 |
|---|---|---|---|---|---|---|---|---|
| `0x36` | 0 | 0 | 1 | 1 | 0 | 1 | 1 | 0 |
