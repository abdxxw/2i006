struct segment;

typedef struct cell_segment{
    struct segment * seg;   /* Segement de la cellule */
    struct cell_segment * suiv; /* Cellule suivante */
}Cell_segment;

typedef struct segment {
    int NumRes ;            /* Numero du reseau auquel appartient ce segment*/
    int p1 , p2 ;           /* Numero des points aux extremites du segment */
                            /* En utilisant la numerotation de T_Pt */
                            /* p1 est le point en bas a gauche par rapport a p2*/
    int HouV ;              /* 0 si Horizontal et 1 si Vertical */
    struct cell_segment * Lintersec ;   /* Liste des segments en intersection */
    int added; /* 1 si deja ajouter a lal iste de touts les segement */ 
} Segment ;

typedef struct point {
    int NumPt;				// numero de point 
    double x , y ;          /* Coordonnees du point */
    int NumRes ;           /* Numero du reseau contenant ce point = Index du tableau T_res*/
    Cell_segment * Lincid ; /* Liste des segments incidents a ce point */
} Point ;

typedef struct reseau {
    int NumRes ;        /* Numero du reseau = Indice dans le tableau T_Res */
    int NbSeg;
    int NbPt ;
    Point ** T_Pt ;    /* Tableau de pointeurs sur chaque point de ce reseau */
} Reseau ;

typedef struct netlist {
    int NbRes ; /* Nombre de reseaux */
    Reseau ** T_Res ; /* Tableau*/

} netlist ;

typedef struct extremite{

double x;/* Ordonnee du point */
int VouGouD;/* 0 si segment V / 1 si pt gauche d’un segment H / 2 si pt droit d’un segment H */
Segment * PtrSeg;/* Pointeur sur le segment correspondant a l’extremite */
int NumPt; /* si segment H: numero du point correspondant */

} Extremite;

typedef struct noeud{
double x;/* Ordonnee du point */
Segment * PtrSeg;/* Pointeur sur le segment correspondant a l’extremite */
int h; /* hauteur du noeud */
struct noeud *fg; /* fils gauche du noeud */
struct noeud *fd; /* fils droit du noeud */
} AVL;


typedef struct Noeud_Ext{
  Extremite *ext;
  struct Noeud_Ext *g;
  struct Noeud_Ext *d;
} Echeancier;



netlist * allouer_netlist(int nombreRes);
Reseau * allouer_reseau(int indiceRes,int nombrePts,int nombreSeg);
Point * allouer_point(int indicePt,int indiceRes, int x,int y);
Segment * allouer_segment(int numRes, int p1,int p2,int hv);
Cell_segment * allouer_cellule();
Extremite * allouer_extremite(int x, int VGD,Segment * seg,int nPT);
Echeancier * allouer_echeancier(Extremite * x);
AVL * allouer_AVL(int x, Segment * s);

netlist * init_netlist_fic (char * nomFic);
Cell_segment * liste_seg_res(Reseau * r);
void ecrire_netlist_fic (char * nomFic,netlist * nL);

int intersection(netlist *N, Segment *s1, Segment *s2);
int nb_segment(netlist *N);
Segment ** get_Segements(netlist *N);
void intersect_naif (netlist * N);
void Sauvegarde_intersection(char * nomfic, netlist * N,int methode);
double getMaxCord(netlist * N);

void liberer_cell(Cell_segment * cell);
void liberer_segment(Segment * Seg);
void liberer_reseau(Reseau * r);
void liberer_netlist(netlist * N);
void liberer_point(Point * Pt);
void liberer_extremiter(Extremite * ext);
void liberer_echeancier(Echeancier * E);
void liberer_AVL(AVL * a);

Echeancier * inserer_extremite(netlist *n, Echeancier * E, Extremite * ext);
Echeancier * cree_echeancier(netlist *n);
Cell_segment * indice_recherche (Cell_segment * lseg, double y,netlist *n);
Cell_segment * inserer_segement (Cell_segment * lseg, Segment *seg,netlist * n);
Cell_segment * supprimer_segement (Cell_segment * lseg, Segment *seg);
Segment * Prem_segment_apres(double y,Cell_segment * lseg,netlist * n);
Segment * Au_Dessus(Segment * seg,Cell_segment * lseg,netlist * n);
void intersect_balayage_rec(netlist * n,Echeancier* E,Cell_segment ** T);
void intersect_balayage(netlist * n);


int AVL_hauteur(AVL * a);
void majHauteur(AVL * a);
void inserer_AVL(AVL ** a,int x,Segment * Seg);
AVL * max_AVL(AVL *a,AVL ** pere);
void supprimer_AVL (AVL **a,int x);
void rotationD(AVL ** a);
void rotationG(AVL ** a);
void equilibrer_AVL(AVL ** a);
void Prem_segment_apres_AVL(double y,AVL * a,Segment ** s);
Segment * Au_Dessus_AVL(Segment * seg,AVL * a,netlist * n);
void intersect_balayage_AVL_rec(netlist * n,Echeancier* E,AVL ** a);
void intersect_balayage_AVL(netlist * n);
void afficher_AVL (AVL *a);


void Dessiner_Netlist(netlist * nL,char * nomFic);
