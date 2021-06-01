typedef struct livre{
int num;
char * titre;
char * auteur;
struct livre * suiv;
} s_livre;

typedef struct{
s_livre * L; /*Liste chainee des livres */
int nbliv; /* Nombre de livres dans la liste */
} Biblio;


void lecture_n_entree(char *nomfic, int n,Biblio *B);
Biblio * intialise_biblio();
s_livre * intialise_livre(int n,char * t,char* a);
void liberer_livre(s_livre * l);
void liberer_biblio(Biblio * b);
void inserer_livre (Biblio *b,s_livre * l);
void supprimer_livre(Biblio * b,s_livre* l);
s_livre * recherche_titre(Biblio b,char * name);
s_livre * recherche_auteur(Biblio b,char * name);
s_livre * recherche_num(Biblio b,int x);
s_livre * recherche_double(Biblio b);
void afficher(Biblio b);
void afficher_livres(s_livre * l);
int exist(s_livre * b,char * s);
void menu();
