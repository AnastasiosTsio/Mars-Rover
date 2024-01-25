#include "robot.h"


/* initialiser le robot r en position (x,y) et orientation o */
void init_robot(Robot *r, int x, int y, Orientation o){
    r->o = o;
    r->x = x;
    r->y = y;
}

/* faire avancer le robot d'une case */
void avancer(Robot *r){
    Orientation or = r->o;
    switch(or){
        case Nord:
            r->y = r->y - 1;
            break;
        case Sud:
            r->y = r->y + 1;
            break;
        case Ouest:
            r->x = r->x - 1;
            break;
        case Est:
            r->x = r->x + 1;
            break;
        default:
            break;
    }
}

/* faire tourner le robot à gauche */
void tourner_a_gauche(Robot *r){
    Orientation or = r->o;
    switch(or){
        case Nord:
            r->o = Ouest;
            break;
        case Sud:
            r->o = Est;
            break;
        case Ouest:
            r->o = Sud;
            break;
        case Est:
            r->o = Nord;
            break;
        default:
            break;
    }

}

/* faire tourner le robot à droite */
void tourner_a_droite(Robot *r){
    Orientation or = r->o;
    switch(or){
        case Nord:
            r->o = Est;
            break;
        case Sud:
            r->o = Ouest;
            break;
        case Ouest:
            r->o = Nord;
            break;
        case Est:
            r->o = Sud;
            break;
        default:
            break;
    }

}

/* recupere la position de la case du robot */
void position(Robot *r, int *x, int *y){
    *x = r->x;
    *y = r->y;
}

/* recupere la position en abscisse de la case du robot */
int abscisse(Robot *r){
    return r->x;
}

/* recupere la position en ordonnee de la case du robot */
int ordonnee(Robot *r){
    return r->y;
}

/* recupere l'orientation du robot */
Orientation orient(Robot *r){
    return r->o;
}

/* recupere la position de la case devant le robot */
void position_devant(Robot *r, int *x, int *y){
    Orientation or = orient(r);
    switch(or){
        case Nord:
            *x = r->x;
            *y = r->y - 1;
            break;
        case Sud:
            *x = r->x;
            *y = r->y + 1;
            break;
        case Ouest:
            *x = r->x - 1;
            *y = r->y;
            break;
        case Est:
            *x = r->x + 1;
            *y = r->y;
            break;
        default:
            break;
    }

}
