#include "observateur.h"

Etat transition(Etat e, Alphabet a) {
        if (a>2)a=2; //si c'est gauche ou droit pour cet automate n'est pas important. Donc G D sont devenus autre.
        
        Etat transitions[3][3] = {
            { Deuxieme, Erreur, INIT},
            { Deuxieme, INIT,  Deuxieme},
            { Erreur, Erreur, Erreur},
        };
        return transitions[e][a];
    
}

Etat transition2(Etat e, Alphabet a){
        if (a>2)a=2;

        Etat transitions[3][3] = {//Etat: INIT=0 Deuxieme=1 Erreur=2 Alphabet:M=0 A=1 Autre=2 
            { INIT, Deuxieme, INIT},
            { INIT, Erreur,  INIT},
            { Erreur, Erreur, Erreur},
        };
        return transitions[e][a];
}

int est_final(Etat e) {
    return e != Erreur;
}