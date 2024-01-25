#include "environnement.h"
#include "interprete.h"
#include "programme.h"

#include <stdio.h>
#include <stdlib.h>

void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case CARACTER_INCONNUE:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNE_MANQUANTE:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
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

int main(int argc, char **argv) {
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;

  if (argc < 2) {
    printf("Usage: %s <fichier_test>\n", argv[0]);
    return 1;
  }

  /* Initialisation de l'environnement : lecture du terrain,
     initialisation de la position du robot */
    FILE* filef = fopen(argv[1],"r+");

    char c[100];
    fscanf(filef,"%s",c);
    errt = initialise_environnement(&envt, c);
    gestion_erreur_terrain(errt);

   /* Lecture du programme */
   fscanf(filef,"%s",c);
   errp = lire_programme(&prog, c);
   gestion_erreur_programme(errp);

    /* Initialisation de l'état */
    int pas_max;
    fscanf(filef,"%d",&pas_max);
    int i=0;
    init_etat(&etat);
    do {
        res = exec_pas(&prog, &envt, &etat);
        /* Affichage du terrain et du robot */
        afficher_envt(&envt);
        i++;
    } while (res == OK_ROBOT && i<pas_max);

    char c2;
    int x,y;
    resultat_inter rs;
    fscanf(filef,"%c",&c2);
    fscanf(filef,"%c",&c2);
    switch (c2){
        case 'O':
            rs = CRASH_ROBOT;
            break;
        case 'S':
            rs = SORTIE_ROBOT;
            break;
        case 'P':
            rs = PLOUF_ROBOT;
            break;
        
        case 'N':
            rs = OK_ROBOT;
            break;
        case 'F':
            rs = ARRET_ROBOT;
            break;

  }

    
    char co;
    Orientation o;
    if (rs == OK_ROBOT || rs == ARRET_ROBOT){
        fscanf(filef,"%c",&c2);//\n
        fscanf(filef,"%d",&x);
        fscanf(filef,"%c",&c2);//espace
        fscanf(filef,"%d",&y);
        fscanf(filef,"%c",&c2);//\n
        fscanf(filef,"%c",&co);

        switch (co){
            case 'N':
                o=Nord;
                break;
            case 'S':
                o=Sud;
                break;
            case 'O':
                o=Ouest;
                break;
            case 'E':
                o=Est;
                break;
        }
        

        if (rs==res && envt.r.x == x && envt.r.y == y && envt.r.o == o){
          printf("tout s'est bien passé\n");
        }else{
          if (rs==res){
            printf("L'evenement obtenue s'est concrétisé.\n");
          }else{
            printf("L'evenement obtenue ne s'est pas concrétisé.\n");
          }

          if ( envt.r.x == x && envt.r.y == y){
              printf("Coordonnees valides.\n");
          }else{
            printf("Coordonnees invalides.\n");
          }

          if (envt.r.o == o){
              printf("Orientation valide.\n");
          }
          else{
              printf("Orientation invalide.\n");
          }
        }
    }else{
      if (rs==res){
        printf("L'evenement obtenue s'est concrétisé.\n");
      }else{
        printf("L'evenement obtenue ne s'est pas concrétisé.\n");
      }
    }


  /* Affichage du résultat */
  /*switch (res) {
  case OK_ROBOT:
    printf("Robot sur une case libre, programme non terminé (ne devrait pas "
           "arriver)\n");
    break;
  case SORTIE_ROBOT:
    printf("Le robot est sorti :-)\n");
    break;
  case ARRET_ROBOT:
    printf("Robot sur une case libre, programme terminé :-/\n");
    break;
  case PLOUF_ROBOT:
    printf("Le robot est tombé dans l'eau :-(\n");
    break;
  case CRASH_ROBOT:
    printf("Le robot s'est écrasé sur un rocher X-(\n");
    break;
  case ERREUR_PILE_VIDE:
    printf("ERREUR : pile vide\n");
    break;
  case ERREUR_ADRESSAGE:
    printf("ERREUR : erreur d'adressage\n");
    break;
  case ERREUR_DIVISION_PAR_ZERO:
    printf("ERREUR : division par 0\n");
    break;
  }*/

  fclose(filef);
}
