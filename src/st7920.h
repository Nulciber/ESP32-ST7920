#ifndef ST7920_H
#define ST7920_H
#include <Arduino.h>

#define SCK  12
#define MOSI 11
#define CS   10

struct Caractere
{
    uint8_t code;
    uint8_t bitmap[8];
};

void st7920_init();
void effacerEcran();
void setPixel(int x, int y, bool allumer);
void allumerPixel(int x, int y);
void eteindrePixel(int x, int y);
void tracerLigne(int x1, int y1, int x2, int y2);
void tracerRectangle(int x1, int y1, int x2, int y2);
void tracerCercle(int cx, int cy, int r);
void afficherCaractere(uint8_t c, int x, int y);
void afficherTexte(const char* texte, int x, int y);

#endif