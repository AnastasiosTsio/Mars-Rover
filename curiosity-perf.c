#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "generation_terrains.h"
#include "terrain.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Code basé sur celui de Julien Pierson et Axel Deleuze

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

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}

//Execute le programme
int execution (char *programme, int max_etapes, Environnement *envt) {
  
  Programme prog;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;
  int etapes = 0;

  /* Lecture du programme */
  errp = lire_programme(&prog, programme);
  gestion_erreur_programme(errp);

  /* Initialisation de l'état */
  init_etat(&etat);
  do {
    res = exec_pas(&prog, envt, &etat);
    /* Affichage du terrain et du robot */
    
    max_etapes--;
    etapes++;
  } while (res == OK_ROBOT && max_etapes > 0);

  afficher_envt(envt);

    if (max_etapes < 0) return 1;
    
    /* Retour du résultat */
    switch (res) {
        case SORTIE_ROBOT:
            // printf("Le robot est sorti :-)\n");
            return etapes;
        case ARRET_ROBOT:
            // printf("Robot sur une case libre, programme terminé :-/\n");
            return -1;
        case PLOUF_ROBOT:
            // printf("Le robot est tombé dans l'eau :-(\n");
            return -2;
        case CRASH_ROBOT:
            // printf("Le robot s'est écrasé sur un rocher X-(\n");
            return -3;
        default:
          return 0;
    }

}

Orientation orient_from_char (char O) {
    switch (O)
    {
    case 'N':
        return Nord;
    case 'S':
        return Sud;
    case 'E':
        return Est;
    case 'O':
        return Ouest;
    default:
      return Nord;
    }
}

int bonne_position (int x, int y, Environnement *envt) {
    return envt->r.x == x && envt->r.y == y;
}

int main (int argc, char **argv) {

    /*
    boucle:
      Générer map

      Balancer curiosity dessus

      Voir le résultat:
      -> Nombre de pas jusque sortie
      -> bloqué: -1 (step > max_step)
      -> eau: -2
      -> rocher: -3
    si n < N goto boucle

      Afficher:
      - Nombre et pct de terrains où robot:
        * sorti
        * bloqué
        * trempette
        * bonk
      - Stats:
        * Nb moyen de pas pour sorties
        * Nb sorties/autres
        * Oui.
    */
   
  int N, l, h, graine, nb_step_max;
  int nb_sorti = 0, nb_bloqué = 0, nb_crash = 0, nb_plouf = 0;
  float nb_pas_moyen = 0;
  float dObst;
  FILE *resFile;
  Environnement envt;


  if (argc < 9) {
    printf("Usage: %s <fichier_programme> <N> <L> <H> <d> <graine> <nb_step_max> <fichier_res> \n", argv[0]);
    return 1;
  }

  N = strtol(argv[2], NULL, 10);
  l = strtol(argv[3], NULL, 10);
  h = strtol(argv[4], NULL, 10);
  dObst = strtof(argv[5], NULL);
  graine = strtol(argv[6], NULL, 10);
  nb_step_max = strtol(argv[7], NULL, 10);

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
  resFile = fopen(argv[8], "w");
  // Écriture du nombre de terrains
  fprintf(resFile, "%d\n", N);

  // Initialisation de la fonction random
  srand(graine);

  fprintf(resFile, "%d\n", N);
  for (int j = 0; j < N; j++) {

    //Generer map
    do {
      generation_aleatoire(&envt.t, l, h, dObst);
    } while (!existe_chemin_vers_sortie(&envt.t));

    //Positionner le Thermomix au centre
    envt.r.o = 0;
    envt.r.x = l / 2;
    envt.r.y = h / 2;
    
    //Balancer la sauce
    int out;
    printf("\n> Execution du test n°%d\n", j);
    out = execution (argv[1], nb_step_max, &envt);

    // Voir le résultat
    fprintf(resFile, "%d\n", out);
    switch (out)
    {
      case -1:case 0:
        nb_bloqué++;
        break;
      case -2:
        nb_plouf++;
        break;
      case -3:
        nb_crash++;
        break;    
      default:
        nb_sorti++;
        nb_pas_moyen += out;
        break;
    }
    
  }
    /*
    Afficher:
      - Nombre et pct de terrains où robot:
        * sorti
        * bloqué
        * trempette
        * bonk
      - Stats:
        * Nb moyen de pas pour sorties
        * Nb sorties/autres
        * Oui.
    */

  printf("\n");
  printf("Le robot est sorti:            %d\n", nb_sorti);
  printf("Le robot est bloqué:           %d\n", nb_bloqué);
  printf("Le robot est tombé dans l'eau: %d\n", nb_plouf);
  printf("Le robot est ecrasé:           %d\n", nb_crash);

  if(nb_sorti>0)printf("Nombre pas moyen:              %.2f\n", nb_pas_moyen / nb_sorti);


  return 0;
}