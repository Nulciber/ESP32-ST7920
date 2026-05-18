#include <stdio.h>
#include <stdint.h>

void afficherBitsStandard(uint8_t valeur)
{
    for (int i = 7; i >= 0; i--)
        printf("%d", (valeur >> i) & 1);
    printf("  (0x%02d = %d)", valeur, valeur);
}

int main()
{
    uint8_t a = 0x36; 
    uint8_t b = 0xF0;

    printf("a         = "); afficherBitsStandard(a); printf("\n");
    printf("b         = "); afficherBitsStandard(b); printf("\n");
    printf("a & b     = "); afficherBitsStandard(a & b); printf("\n");
    printf("a | b     = "); afficherBitsStandard(a | b); printf("\n");
    printf("a ^ b     = "); afficherBitsStandard(a ^ b); printf("\n");
    printf("a << 4    = "); afficherBitsStandard(a << 4); printf("\n");
    printf("a >> 4    = "); afficherBitsStandard(a >> 4); printf("\n");
    printf("~a        = "); afficherBitsStandard(~a); printf("\n");

    return 0;
}