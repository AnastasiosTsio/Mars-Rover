
#include "terrain.h"
#include "robot.h"
#include <stdio.h>
#include <string.h>

int largeur(Terrain *t){
  return t->largeur;
}

int hauteur(Terrain *t){
  return t->hauteur;
}


erreur_terrain lire_terrain(FILE *f, Terrain *t, int *x, int *y) {
  int l, h;   // Dimensions du terrain
  int rx, ry; // Coordonnées initiales du robot
  char c;
  if (f == NULL) {
    return ERREUR_FICHIER;
  }



  // Lecture de la largeur
    if(feof(f) == 1)return ERREUR_FICHIER;
    fscanf(f,"%d",&l);
    if(feof(f) == 1)return ERREUR_FICHIER;
    fscanf(f,"%c",&c);// lit le '\n'

  // Lecture de la hauteur
    if(feof(f) == 1)return ERREUR_FICHIER;
    fscanf(f,"%d",&h);
    if(feof(f) == 1)return ERREUR_FICHIER;
    fscanf(f,"%c",&c);


    if(l == 0){
      printf("Erreur lecture largeur\n");
      return ERREUR_LECTURE_LARGEUR;}

    if(h == 0){
      printf("Erreur lecture hauter\n");
      return ERREUR_LECTURE_HAUTEUR;}

    t->largeur = l;
    t->hauteur = h;

  // Lecture du terrain
    for(int j = 0; j<h ; j++){ //j ligne
      for(int i = 0 ; i<l ; i++){ // i colonne
            
            if(feof(f) == 1){
              printf("Erreur ligne manquante\n");
              return ERREUR_LIGNE_MANQUANTE;}

            fscanf(f,"%c",&c);
            if(c == '\n'){
              printf("Erreur ligne courte\n");
              return ERREUR_LIGNE_COURTE;}

            switch(c){
                case '.':
                    t->tab[j][i] = LIBRE;
                    break;
                case '#':
                    t->tab[j][i] = ROCHER;
                    break;
                case '~':
                    t->tab[j][i] = EAU;
                    break;
                case 'C':
                    *x = i; //x colonne
                    *y = j; //y ligne
                    t->tab[j][i] = LIBRE;
                    break;
                default:
                    printf("Caracter inconnue\n");
                    return CARACTER_INCONNUE;
                    break;
            }
        }
      fscanf(f,"%c",&c);
      if(c != '\n' && feof(f)!=1){
        printf("c= %c\n",c);
        return ERREUR_LIGNE_LONGUE;
        }
    }
    fscanf(f,"%c",&c);
    if(feof(f) != 1){
      printf("Erreur ligne manquante\n");
      return ERREUR_LIGNE_MANQUANTE;}
  // Initialisation de la position du robot
    rx = *x;
    ry = *y;
    //t->tab[ry][rx] = ROBOT;

  return OK;
}

void afficher_terrain(Terrain *t){
  printf("<Debut affichage avec %d et %d>\n",t->hauteur,t->largeur);
  for(int i=0;i<t->hauteur;i++){ //i ligne
    for (int j=0;j<t->largeur;j++){ //j colonne
      switch ((t->tab[i][j])){
        case LIBRE:
          printf(".");
          break;
        case ROCHER:
          printf("#");
          break;
        case EAU:
          printf("~");
          break;
        default:
          printf("Caracter inconnue\n");
          break;
        }
      }
    printf("\n");
    }
  }

int est_case_libre(Terrain *t, int x, int y){
  if (x<t->largeur && x >=0 && y<t->hauteur && y>=0 && t->tab[y][x] == LIBRE){
    return 1;
  }
  return 0;
}