#include <stdio.h>
#include <stdint.h>

void afficherBitsStandard(uint8_t valeur)
{
    for (int i = 7; i >= 0; i--)         // Un octet fait 8 bits (0-7), on les lit du MSB au LSB
        printf("%d", (valeur >> i) & 1); // Affiche une valeur(%) en format décimal(d). 
                                         // Le nombre est la valeur passée décalée de i positions sur la droite AND 00000001
                                         // On fait ainsi pour isoler le bit en cours de lecture
                                             

    printf("  (0%02x = %d)", valeur, valeur);//
}
int main()
{
    uint8_t a = 0x36;
    
    printf("%d",a);
    //printf("Voici le nombre en hexa:");printf("%02x\n", a);
    //printf("Voici le nombre en decimal:");printf("%02d\n", a);
    //printf("Voici le nombre en binaire:");printf("%08b\n", a<<2);
    return 0;
}