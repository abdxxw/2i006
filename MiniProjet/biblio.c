#include <stdio.h>
#include <stdlib.h>
#include "entree_sortie.h"
#include <assert.h>
#include <string.h>
#include "biblio.h"


//Cree et initialise une bibliotheque 
Biblio * intialise_biblio()
{
    Biblio * b = malloc(sizeof(Biblio));
    b->L = NULL;
    b->nbliv = 0;
    return b;
}
//Cree et initialise un livre
s_livre * intialise_livre(int n,char * t,char* a)
{
    s_livre * l = malloc(sizeof(s_livre));
   
    l->num = n;
    l->titre = strdup(t);
    l->auteur = strdup(a);
    l->suiv = NULL;
    return l;
}
    
//Lis n ligne du fichier nomfic et les ajoute dans la biblio B
void lecture_n_entree(char *nomfic, int n,Biblio *B)
{
    assert(B);
    assert(nomfic);
    int i,x;
    FILE *f = fopen(nomfic,"r");
    s_livre * l;
    char s1[50],s2[50];
    if(f==NULL)
        fprintf(stderr,"Erreur d'ouverture de fichier\n");
    for(i=0;i<n;i++){
         x = GetEntier(f);
         GetChaine(f,50,s1);
         GetChaine(f,50,s2);
         
         l = intialise_livre(x,s1,s2);

         
             l->suiv = B->L;
             B->L = l;
             B->nbliv++;
         
    }
    fclose(f);
}

//Recherche dans la biblio un livre par son titre
 s_livre * recherche_titre(Biblio b,char * name){
     while(b.L){
         if(strcmp(b.L->titre,name)==0)
            return b.L;
         b.L=b.L->suiv;
     }
     return NULL;
 }

//Recherche dans la biblio un livre par son numéro
 s_livre * recherche_num(Biblio b,int x){
     while(b.L){
         if(b.L->num == x)
            return b.L;
         b.L=b.L->suiv;
     }
     return NULL;
 }

//Recherche dans la biblio tout les livres d'un auteur
 s_livre * recherche_auteur(Biblio b,char * name){
        s_livre * tmp=NULL,*aaa;
        
     while(b.L){
         if(strcmp(b.L->auteur,name)==0){
                aaa = intialise_livre(b.L->num,b.L->titre,b.L->auteur);
                aaa->suiv = tmp;
                tmp = aaa;
         }

             b.L=b.L->suiv;
     }
     return tmp;
 }

//Insere un livre dans la biblio
 void inserer_livre (Biblio *b,s_livre * l)
 {
     assert (b);
     assert (l);
     if (!b->L)
     {
        b->L=l;
        
     }
     else{
        l->suiv=b->L; 
        b->L=l;
     }
     b->nbliv++;
 }

//Supprime un livre de la biblio
 void supprimer_livre(Biblio * b,s_livre* l)
 {
     assert (l);
     assert (b);
     s_livre * liv=b->L;
     if(l== liv){
         b->L = b->L->suiv;
         free(liv);
         b->nbliv--;
         printf ("Le livre a ete supprimé avec succes\n");
         return;
     }
     while (liv->suiv)
     {
         if (liv->suiv == l){
             liv->suiv = l->suiv;
             free(l);
             b->nbliv--;
            printf ("Le livre a ete supprimé avec succes\n");
         }
         liv = liv->suiv;
     }

 }

//Verifie si le livre ayant comme titre "s" existe dans la liste b
 int exist(s_livre * b,char * s){
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
s_livre * recherche_double(Biblio b)
{
    int nbliv;
    s_livre * out=NULL,*tmp=NULL;
    while(b.L){
        s_livre * l = recherche_auteur(b,b.L->auteur);
        while(l){
            tmp = NULL;
            s_livre * l2 = l->suiv;
            nbliv=1;
            while((l2)&&(nbliv<2)){

                if(strcmp(l->titre,l2->titre)==0){
                    nbliv++;
                }

                if(nbliv>=2){
                    if(exist(out,l->titre)==0){
                        tmp = intialise_livre(l2->num,l2->titre,l2->auteur);
                        tmp->suiv = out;
                        out = tmp;
                    }
                    
                }
                l2 = l2->suiv;
            }
            
            l = l->suiv;
        }
        b.L = b.L->suiv;
    }
    return out;
}

//affiche tout les livre de la biblio
 void afficher(Biblio b)
 {
    while(b.L){
         printf("Livre num :%d %s par %s\n",b.L->num,b.L->titre,b.L->auteur);
         b.L = b.L->suiv;
     }
 }

//Desallouer l'espace mémoire du livre l
 void liberer_livre(s_livre * l){
     if(l){

     free(l->titre);
     free(l->auteur);
     free(l);
     }
 }

//Desallouer l'espace mémoire de la biblio b
 void liberer_biblio(Biblio * b){
    s_livre * tmp;
     
    while(b->L){
         tmp = b->L;
         b->L = b->L->suiv;
         liberer_livre(tmp);
     }
     free(b);
 }

//Afficher tout les livres de la liste l
 void afficher_livres(s_livre * l)
 {
     if(!l)
        printf("Aucun livre ne correspond a cette recherche\n");
    while(l){
         printf("Livre num :%d %s par %s\n",l->num,l->titre,l->auteur);
         l = l->suiv;
     }
 }

//Affiche le menu
 void menu(){

     printf("===========================================\nMenu :\n\n 1. Afficher Tous les livres\n 2. Inserer livre\n 3. Supprimer livre\n 4. Recherche par titre\n 5. Recherche par auteur \n 6. Recherche par numero \n 7. Recherche doublant\n 0. exit\n===========================================\n");

 }