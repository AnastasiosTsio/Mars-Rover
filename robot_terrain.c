#include "robot.h"
#include "terrain.h"
#include <stdio.h>

#define TAILLE_TERRAIN 10

/* Affiche une orientation sur la sortie standard */
void afficher_orientation(Orientation o) {
  switch (o) {
  case Nord:
    printf("Nord\n");
    break;
  case Est:
    printf("Est\n");
    break;
  case Sud:
    printf("Sud\n");
    break;
  case Ouest:
    printf("Ouest\n");
    break;
  }
}

void afficher_infos_robot(Robot *r) {
  int x, y;
  int x1, y1;

  // Récupérer la position du robot
  position(r, &x, &y);
  // Récupérer la case devant le robot
  position_devant(r, &x1, &y1);
  // Afficher la position
  printf("Position : (%d, %d) - Orientation : ", x, y);
  afficher_orientation(orient(r));
  printf("\n");
}

void afficher_terrain_et_robot(Terrain *t, Robot *r) {
  int i, j;
  char c;
  Case st;

  for (j = 0; j < t->hauteur; j++) { //j ligne
    for (i = 0; i < t->largeur; i++) { //i colonne
      if ((i == abscisse(r)) && (j == ordonnee(r))) {
        // Afficher le robot selon son orientation
        switch (orient(r)) {
        case Nord:
          c = '^';
          break;
        case Est:
          c = '>';
          break;
        case Sud:
          c = 'v';
          break;
        case Ouest:
          c = '<';
          break;
        }
      } else {
        // Afficher la case
        st = t->tab[j][i];
        switch(st){
          case LIBRE:
            c = '.';
            break;
          case ROCHER:
            c = '#';
            break;
          case EAU:
            c = '~';
            break;
          default:
            c = '.';
            break;
        }
      }
      printf("%c", c);
    }
    printf("\n");
  }
}

/* Renvoie vrai si le robot r, de sa position et orientation courante,
   peut avancer dans le terrain t */
int robot_peut_avancer(Terrain *t, Robot *r) {
  Orientation o = r->o;
  int peut_avancer = 0;

  switch(o){
    case Nord:
      if(r->y == 0){
        peut_avancer = 0;
        printf("bord\n");
      }else{
        peut_avancer = (t->tab[r->y -1][r->x] == LIBRE);
      }
      break;
    case Est:
      if(r->x == t->largeur - 1){
        peut_avancer = 0;
        printf("bord\n");
      }else{
        peut_avancer = (t->tab[r->y][r->x +1] == LIBRE);
      }
      break;
    case Sud:
      if(r->y == t->hauteur - 1){
        peut_avancer = 0;
        printf("bord\n");
      }else{
        peut_avancer = (t->tab[r->y +1][r->x] == LIBRE);
      }
      break;
    case Ouest:
      if(r->x == 0){
        peut_avancer = 0;
        printf("bord\n");
      }else{
        peut_avancer = (t->tab[r->y][r->x -1] == LIBRE);
      }
      break;
  
  }
  return peut_avancer;
}

int main(int argc, char **argv) {
  Terrain t;
  FILE *f;
  erreur_terrain e;
  int x, y;
  Robot r;
  char c;

  // Lecture du terrain : nom du fichier en ligne de commande
  if (argc < 2) {
    printf("Usage: %s <fichier terrain>\n", argv[0]);
    return 1;
  }

  // Lecture du terrain
  f = fopen(argv[1], "r");
  e = lire_terrain(f, &t, &x, &y);
  fclose(f);

  if (e != OK) {
    printf("Erreur lors de la lecture du terrain\n");
    return 2;
  }

  // Initialisation du robot
  init_robot(&r, x, y, Est);

  afficher_infos_robot(&r);
  afficher_terrain_et_robot(&t, &r);

  printf("Entrer une action ([a]vancer, [g]auche, [d]roite, [f]in : ");
  scanf(" %c", &c);
  while (c != 'f') {
    int x, y; // Position devant
    switch (c) {
    case 'G':
    case 'g':
      tourner_a_gauche(&r);
      break;
    case 'D':
    case 'd':
      tourner_a_droite(&r);
      break;
    case 'A':
    case 'a':
      position_devant(&r, &x, &y);
      if (robot_peut_avancer(&t, &r)) {
        avancer(&r);
      } else {
        printf(" !! Le robot ne peut pas avancer !!\n");
      }
    }

    afficher_infos_robot(&r);
    afficher_terrain_et_robot(&t, &r);

    printf("Entrer une action ([a]vancer, [g]auche, [d]roite, [f]in : ");
    scanf(" %c", &c);
  }
}
