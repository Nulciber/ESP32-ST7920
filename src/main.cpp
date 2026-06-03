#include "st7920.h"

void setup()
{
  st7920_init();
  effacerEcran();
  // afficherCaractere(0x80, 0, 0);
  afficherTexte("C'est bien la f\x84te\x83", 0, 0);
  // afficherTexte("Ligne 2 du texte", 0, 8);
  // afficherTexte("    Ave Maria,", 0, 8);
  // afficherTexte("  gracia plena", 0, 24);
  // afficherTexte(" Dominus tecum.", 0, 40);
}

void loop() {}