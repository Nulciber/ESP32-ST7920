#include "st7920.h"
#include <string.h>
#include <SPI.h>
#include "police_standard.h"
#include "police_accents.h"

uint8_t gdram[64][16];

void sendCommand(uint8_t cmd)
{
    digitalWrite(CS, HIGH);   
    SPI.transfer(0xF8);       
    SPI.transfer(cmd & 0xF0); 
    SPI.transfer(cmd << 4);   
    digitalWrite(CS, LOW);    
    delayMicroseconds(72);    
}

void sendData(uint8_t dat)
{
    digitalWrite(CS, HIGH);
    SPI.transfer(0xFA);
    SPI.transfer(dat & 0xF0);
    SPI.transfer(dat << 4);
    digitalWrite(CS, LOW);
    delayMicroseconds(72);
}

void st7920_init()
{
    memset(gdram, 0, sizeof(gdram));
    SPI.begin(SCK, -1, MOSI, CS);
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
    pinMode(CS, OUTPUT);
    digitalWrite(CS, LOW);
    delay(100);
    sendCommand(0x30); delay(2);
    sendCommand(0x0C); delay(2);
    sendCommand(0x01); delay(10);
    sendCommand(0x30); delay(2);
    sendCommand(0x34); delay(2);
    sendCommand(0x36); delay(2);
}

void effacerEcran()
{
    for (int y = 0; y < 64; y++)
    {
        if (y < 32)
        {
            sendCommand(0x80 | y);
            sendCommand(0x80);
        }
        else
        {
            sendCommand(0x80 | (y - 32));
            sendCommand(0x88);
        }
        for (int x = 0; x < 16; x++)
            sendData(0x00);
    }
}

void setPixel(int x, int y, bool allumer)
{
    int row = y;
    int col = x / 8;
    int bit = 7 - (x % 8);
    if (allumer)
        gdram[row][col] |= (1 << bit);
    else
        gdram[row][col] &= ~(1 << bit);
    if (row < 32)
    {
        sendCommand(0x80 | row);
        sendCommand(0x80 | (col / 2));
    }
    else
    {
        sendCommand(0x80 | (row - 32));
        sendCommand(0x88 | (col / 2));
    }
    sendData(gdram[row][col & ~1]);
    sendData(gdram[row][col | 1]);
}

void allumerPixel(int x, int y) { setPixel(x, y, true); }
void eteindrePixel(int x, int y) { setPixel(x, y, false); }

void tracerLigne(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int erreur = dx - dy;
    while (true)
    {
        allumerPixel(x1, y1);
        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * erreur;
        if (e2 > -dy) { erreur -= dy; x1 += sx; }
        if (e2 <  dx) { erreur += dx; y1 += sy; }
    }
}

void tracerRectangle(int x1, int y1, int x2, int y2)
{
    tracerLigne(x1, y1, x2, y1);
    tracerLigne(x1, y2, x2, y2);
    tracerLigne(x1, y1, x1, y2);
    tracerLigne(x2, y1, x2, y2);
}

void tracerCercle(int cx, int cy, int r)
{
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    while (x <= y)
    {
        allumerPixel(cx + x, cy + y);
        allumerPixel(cx - x, cy + y);
        allumerPixel(cx + x, cy - y);
        allumerPixel(cx - x, cy - y);
        allumerPixel(cx + y, cy + x);
        allumerPixel(cx - y, cy + x);
        allumerPixel(cx + y, cy - x);
        allumerPixel(cx - y, cy - x);
        if (d < 0)
            d += 4 * x + 6;
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void afficherCaractere(uint8_t c, int x, int y)
{
    int nbStandard = sizeof(police_standard) / sizeof(police_standard[0]);
    for (int i = 0; i < nbStandard; i++)
    {
        if (police_standard[i].code == c)
        {
            for (int ligne = 0; ligne < 8; ligne++)
            {
                uint8_t octet = police_standard[i].bitmap[ligne];
                for (int bit = 0; bit < 8; bit++)
                    if (octet & (0x80 >> bit))
                        allumerPixel(x + bit, y + ligne);
            }
            return;
        }
    }
    int nbAccents = sizeof(police_accents) / sizeof(police_accents[0]);
    for (int i = 0; i < nbAccents; i++)
    {
        if (police_accents[i].code == c)
        {
            for (int ligne = 0; ligne < 8; ligne++)
            {
                uint8_t octet = police_accents[i].bitmap[ligne];
                for (int bit = 0; bit < 8; bit++)
                    if (octet & (0x80 >> bit))
                        allumerPixel(x + bit, y + ligne);
            }
            return;
        }
    }
}

void afficherTexte(const char* texte, int x, int y)
{
    int cx = x;
    int cy = y;
    for (int i = 0; texte[i] != '\0'; i++)
    {
        if (cx + 8 > 128) { cx = x; cy += 8; }
        if (cy + 8 > 64) return;
        afficherCaractere((uint8_t)texte[i], cx, cy);
        cx += 8;
    }
}