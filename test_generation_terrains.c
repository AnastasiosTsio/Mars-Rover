#include "generation_terrains.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int nb_cases_occupees(Terrain *T) {
  int occupee = 0;
  int x, y;
  for (x = 0; x < largeur(T); x++) {
    for (y = 0; y < hauteur(T); y++) {
      if (T->tab[x][y] != LIBRE)
        occupee++;
    }
  }
  return occupee;
}

// Test de generation aléatoire de terrains
// Le programme génère n terrains de largeur et hauteur fixes
// avec largeur et hauteur impaires et inférieures a dimension_max de terrain.h
// avec densité d'obstacle dObst
// autre que la case centrale soit occupee
// l'appel du programme se fait avec 5 arguments :
// generation_terrains N largeur hauteur dObstacle fichier_res
// la sortie se fait dans le fichier resultat
int terrain_vide(Terrain *T){
  for(int i=0;i<T->hauteur;i++){
      for(int j=0;j<T->largeur;j++){
        if (T->tab[i][j] != LIBRE)
          return 0;
      }
    }
    return 1;
}

char print_case(int c){
  switch(c){
    case 0:
      printf(".");
      return '.';
    case 1:
      printf("~");
      return '~';
    case 2:
      printf("#");
      return '#';
    case 3:
      printf("C");
      return 'C';
    default:
      printf("?");
      return '?';    
  }
}

int main(int argc, char **argv) {
  int N, l, h; //i
  float dObst;
  FILE *resFile;
  Terrain T;

  if (argc < 6) {
    printf(
        "Usage: %s <N> <largeur> <hauteur> <densite_obstacle> <fichier_res> \n",
        argv[0]);
    return 1;
  }

  N = strtol(argv[1], NULL, 10);
  l = strtol(argv[2], NULL, 10);
  h = strtol(argv[3], NULL, 10);
  dObst = strtof(argv[4], NULL);

  // test de l et h
  if (l > DIM_MAX || l % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (h > DIM_MAX || h % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((dObst > 1) || (dObst < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }

  // Ouverture du fichier résultat
  resFile = fopen(argv[5], "w");
  // Écriture du nombre de terrains
  fprintf(resFile, "%d\n", N);
  fprintf(resFile,"%d\n%d\n", l, h);
  // Initialisation de la fonction random
  srand(time(NULL));

  char poub;
  float nb_case=(h*l);
  float obst;

  for(int k=0;k<N;k++){
    obst = 0;
    generation_aleatoire(&T,l,h,dObst);
    for(int i=0;i<h;i++){
      for(int j=0;j<l;j++){
        poub = print_case(T.tab[i][j]);
        fprintf(resFile,"%c",poub);
        if (T.tab[i][j] == EAU || T.tab[i][j] == ROCHER) obst = obst + 1.0;
      }
      printf("\n");
      fprintf(resFile,"\n");
    }
    fprintf(resFile,"Densite voulue %f\n",dObst);
    fprintf(resFile,"Densite obtenue %f\n",obst/nb_case);
    if (terrain_vide(&T)==1)
      fprintf(resFile,"Terrain Invalide\n");
    else
      fprintf(resFile,"Terrain Valide\n");
    fprintf(resFile,"\n");
    printf("\n");
  }
  fprintf(resFile,"Densite moyenne %f\n",obst/nb_case);

  // A compléter
  // Génération aléatoire des terrains
  // Écriture des terrains générés dans le fichier resFile
  // Écriture/Affichage des statistiques

  // fermeture des fichiers
  fclose(resFile);
  return 0;
}
