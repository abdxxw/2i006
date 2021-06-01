#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "entree_sortie.h"
#include "biblioHash.h"




//Cree et initialise une bibliotheque sous forme de table de hachage  
tableHachage_t* initTableHachage(int m)
{
   tableHachage_t *THash=malloc (sizeof(tableHachage_t));
    THash->T=calloc(m,sizeof(cell_t*)*m);
    THash->m=m;
    THash->nE=0;
    return THash;
}

//calcul la cle d'un livre avec son titre 
int fonctionClef(char *nom){
    int x =0,i;
    for(i=0;nom[i];i++){
        x+=nom[i];
    }
    return x;
}

//Calcul l'indice ou doit s'inserer le livre de cle clef
int fonctionHachage(int clef, int m){
    return floor(m*(clef*A-floor(clef*A)));
}


//Cree et initialise un livre
cell_t * intialise_cell(int n,char * t,char* a)
{
    cell_t * l = malloc(sizeof(cell_t));
    l->clef = fonctionClef(a);
    l->num = n;
    l->titre = strdup(t);
    l->auteur = strdup(a);
    l->suiv = NULL;
    return l;
}
 

//Lis n ligne du fichier nomfic et les ajoute dans la biblio B
void Hlecture_n_entree(char *nomfic, int n,tableHachage_t *B)
{
    assert(B);
    assert(nomfic);
    int i,x;
    FILE *f = fopen(nomfic,"r");
    cell_t * l;
    char s1[50],s2[50];
    if(f==NULL)
        fprintf(stderr,"Erreur d'ouverture de fichier\n");
    for(i=0;i<n;i++)
    {
         x = GetEntier(f);
         GetChaine(f,50,s1);
         GetChaine(f,50,s2);
         
         l = intialise_cell(x,s1,s2);
         int x = fonctionHachage(l->clef,tM);


             l->suiv = B->T[x];
             B->T[x] = l;
             B->nE++;
        
    }
    fclose(f);
}

//Recherche dans la biblio tout les livres d'un auteur
cell_t * Hrecherche_auteur(tableHachage_t b,char * name){
     int x = fonctionClef(name);
     x=fonctionHachage(x,tM);
     cell_t * tmp = NULL;
     cell_t * out = NULL;
     cell_t * tmp2 = b.T[x];
     while(tmp2){
         if(strcmp(name,tmp2->auteur)==0){
            tmp = intialise_cell(tmp2->num,tmp2->titre,tmp2->auteur);
            tmp->suiv = out;
            out=tmp;
         }
         tmp2=tmp2->suiv;
     }
     return out;
 }

//Recherche dans la biblio un livre par son titre
 cell_t * Hrecherche_titre(tableHachage_t b,char * name){
     int i;
     for (i=0;i<b.m;i++){
         cell_t * tmp = b.T[i];
         while (tmp){
            if (strcmp(tmp->titre,name)==0) return tmp;
            tmp=tmp->suiv;
         }
     }
     return NULL;
 }

//Recherche dans la biblio un livre par son numéro
cell_t * Hrecherche_num(tableHachage_t b,int x){
     int i;
     for (i=0;i<b.m;i++){
         cell_t * tmp = b.T[i];
         while (tmp){
            if (x==tmp->num) return tmp;
            tmp=tmp->suiv;
         }
     }
     return NULL;
 }

//Insere un livre dans la biblio
 void Hinserer_livre (tableHachage_t *b,cell_t * l)
 {
     assert (b);
     assert (l);
     int x = fonctionClef(l->auteur);
     x=fonctionHachage(x,tM);

    l->suiv=b->T[x]; 
    b->T[x]=l;
     
     b->nE++;
 }

//Supprime un livre dans la biblio
 void Hsupprimer_livre(tableHachage_t * b,cell_t * l)
 {
     assert (l);
     assert (b);
     int x = fonctionClef(l->auteur);
     x=fonctionHachage(x,tM);
     cell_t * liv=b->T[x];

     if(l==liv){
         b->T[x]= b->T[x]->suiv;
         free(liv);
         b->nE--;
         printf ("Le livre a ete supprimé avec succes\n");
         return;
     }
     while (liv->suiv)
     {
         if (liv->suiv == l){
             liv->suiv = l->suiv;
             free(l);
             b->nE--;
            printf ("Le livre a ete supprimé avec succes\n");
         }
         liv = liv->suiv;
     }

 }

//Verifie si le livre ayant comme titre "s" existe dans la liste b
 int Hexist(cell_t * b,char * s){
     if(!b)
        return 0;
     while(b){
         if(strcmp(b->titre,s)==0)
            return 1;
        b=b->suiv;
     }
     return 0;
 }

//Recherche tout les livre qui ont au moins un doublant 
 cell_t * Hrecherche_double(tableHachage_t b){
     int i;
     int x;
     cell_t * out=NULL,*tmp=NULL,*tmp2=NULL;
     for(i=0;i<b.m;i++){
         tmp = b.T[i];
         while(tmp){
            tmp2 = NULL;
            cell_t * l2 = tmp->suiv;
            x=1;
            while((l2)&&(x<2)){

                if((strcmp(tmp->titre,l2->titre)==0)&&(strcmp(tmp->auteur,l2->auteur)==0)){
                    x++;
                }
                if(x>=2){
                    if(Hexist(out,tmp->titre)==0){
                        tmp2 = intialise_cell(l2->num,l2->titre,l2->auteur);
                        tmp2->suiv = out;
                        out = tmp2;
                    }
                    
                }
                l2 = l2->suiv;
            }
            tmp=tmp->suiv;
         }
     }
    return out;

 }


//affiche tout les livre de la biblio
 void Hafficher(tableHachage_t b)
 {
     int i=0;
     for (i=0;i<b.m;i++)
     {
         cell_t * tmp = b.T[i];
        while(tmp){
            printf("Livre num :%d %s par %s\n",tmp->num,tmp->titre,tmp->auteur);
            tmp = tmp->suiv;
        }
     }
 }

//Desallouer l'espace mémoire du livre l
 void liberer_cell(cell_t * l){
     if(l){

     free(l->titre);
     free(l->auteur);
     free(l);
     }
 }

//Desallouer l'espace mémoire de la biblio b
 void Hliberer_biblio(tableHachage_t * b){
    cell_t * tmp;
     int i=0;
     for (i=0;i<b->m;i++)
     {
        cell_t * tmp2 = b->T[i];
        while(tmp2){
          tmp = tmp2;
          tmp2 = tmp2->suiv;
          liberer_cell(tmp);
        }
     }
	 free(b->T);
     free(b);
 }


//Afficher tout les livres de la liste l
 void Hafficher_livres(cell_t * l)
 {
     if(!l)
        printf("Aucun livre ne correspond a cette recherche\n");
    while(l){
         printf("Livre num :%d %s par %s\n",l->num,l->titre,l->auteur);
         l = l->suiv;
     }
 }


//Affiche le menu
 void Hmenu(){

     printf("===========================================\nMenu :\n\n 1. Afficher Tous les livres\n 2. Inserer livre\n 3. Supprimer livre\n 4. Recherche par titre\n 5. Recherche par auteur \n 6. Recherche par numero \n 7. Recherche doublant\n 0. exit\n===========================================\n");


 }

