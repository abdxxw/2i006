
#define A (sqrt(5)-1)/2
#define tM 1901

typedef struct cell{
    int clef;
    int num;
    char * titre;
    char * auteur;
    struct cell * suiv;

}cell_t;
typedef struct{
    int nE;
    int m;
    cell_t **T;
}tableHachage_t;

tableHachage_t* initTableHachage(int m);
int fonctionClef(char *nom);
int fonctionHachage(int clef, int m);
cell_t * intialise_cell(int n,char * t,char* a);
void Hlecture_n_entree(char *nomfic, int n,tableHachage_t *B);
cell_t * Hrecherche_auteur(tableHachage_t b,char * name);
cell_t * Hrecherche_titre(tableHachage_t b,char * name);
cell_t * Hrecherche_num(tableHachage_t b,int x);
cell_t * Hrecherche_double(tableHachage_t b);
void Hinserer_livre (tableHachage_t *b,cell_t * l);
void Hsupprimer_livre(tableHachage_t * b,cell_t * l);
void Hafficher(tableHachage_t b);
void Hliberer_biblio(tableHachage_t * b);
void liberer_cell(cell_t * l);
void Hafficher_livres(cell_t * l);
int Hexist(cell_t * b,char * s);
void Hmenu();
