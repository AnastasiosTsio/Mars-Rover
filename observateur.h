#ifndef __OBSERVATEUR_H__
#define __OBSERVATEUR_H__

typedef enum { M, A,AUTRE, D,G } Alphabet;
typedef enum { INIT, Deuxieme, Erreur } Etat;

Etat transition(Etat e, Alphabet a);
Etat transition2(Etat e, Alphabet a);
int est_final(Etat e);

#endif