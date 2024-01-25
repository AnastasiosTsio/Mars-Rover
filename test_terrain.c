#include "terrain.h"
#include <stdio.h>

int main(int argc, char **argv) {
  FILE *f;
  Terrain t;
  int x, y;

  if (argc < 2) {
    printf("Usage : %s <fichier>\n", argv[0]);
    return 1;
  }
  char fichier[]="FICHIER";
  f = fopen(argv[1], "r");

  erreur_terrain res =lire_terrain(f, &t, &x, &y);
  while(res==ERREUR_FICHIER){
    printf("Erreur Fichier, on veut un autre fichier\n");
    scanf("%s",fichier);
    f = fopen(fichier,"r");
    res = lire_terrain(f, &t, &x, &y);
  }
  
  if (res==OK){
  fclose(f);
  afficher_terrain(&t);
  printf("Position initiale du robot : (%d, %d)\n", x, y);}
}
