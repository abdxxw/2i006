#include "netlist.h"

typedef struct elementListeA Liste;  /* Definition du type liste */

enum TypeSommet {E=0,S}; /* type du sommet  */


typedef struct sommet {

int ind;          // indice dans tab de graphe
int nb ;         //  nb  aretes  incidentes
Liste * L_adj;    //  liste  des  sommets  adjacents a ce  sommet
double x,y ;      //  coordonnees
Extremite * E;      /* NULL si sommet d un segement  */
Segment * S;        /*  NULL si sommet d'une extremite*/
enum TypeSommet type; /*E pour extremite , S pour Segement*/
int status; /*  status du noeud : visité ou non*/
} Sommet ;

typedef struct elementListeA { // liste sommet_extrémités
Sommet * a ;    
struct elementListeA * suiv ;
} Liste ;




typedef struct graphe {
int n ;             //  nb de  sommets
Sommet  ** tab ;    //  tableau  de  pointeurs  sur  sommets segement
} Graphe ;



Liste * allouer_liste(Sommet * a);
Sommet * allouer_sommet(double x, double y,enum TypeSommet type,Extremite * E, Segment * S,int ind);
Graphe * allouer_graphe(int n);

void liberer_liste(Liste * l);
void liberer_sommet(Sommet * s);
void liberer_graphe(Graphe * g);
Graphe * cree_graphe(netlist *n,char * nomFic);
int * getVia(Graphe * g);
void Dessiner_Graphe(netlist * nL,char * nomFic,Graphe * G);
void Dessiner_Graphe_Via(netlist * nL,char * nomFic,Graphe * G,int * tabS);
void afficher_graphe(Graphe g);



int existe_imp(Liste * l, int x, int * M);
int detecte_cycle_impair_rec(Graphe *G,int r,int* M,Liste ** l);
Liste * detecte_cycle_impair(Graphe *G,int * tabS);
int * Ajout_vias_cycle_impair(Graphe *G);
int * bicolore(Graphe * G);
